#include "pch.h"
#include "Manager.h"
#include "Memory.h"
#include "LibMath.h"

namespace Library::Memory
{
#pragma region Heap	
	Manager::Heap::Heap(const size_t index) noexcept :
		begin(Malloc<std::byte>(byteFactor << index)),
		end(begin + (byteFactor << index)),
		top(begin),
		index(index)
	{
		DebugFill(begin, end);
	}

#pragma region properties
	bool Manager::Heap::Contains(const std::byte* addr) const noexcept
	{
		return begin <= addr && addr < end;
	}

	bool Manager::Heap::ContainsAllocated(const std::byte* addr) const noexcept
	{
		return begin <= addr && addr < top;
	}

	bool Manager::Heap::CanFit(const size_t numBytes) const noexcept
	{
		return top + numBytes < end;
	}

	size_t Manager::Heap::AllocatedBytes() const noexcept
	{
		return top - begin;
	}

	size_t Manager::Heap::TotalBytes() const noexcept
	{
		return byteFactor << index;
	}

	Manager::Heap* Manager::Heap::Next() noexcept
	{
		const size_t next = index + 1;
		return next < heaps.size() ? &heaps[next] : nullptr;
	}
#pragma endregion

	Manager::Handle* Manager::Heap::Alloc(const size_t numBytes, const size_t alignment) noexcept
	{
		// calculate alignment offset
		const size_t offset = size_t(top) % alignment;

		// try to reserve bytes
		if (CanFit(offset + numBytes))
		{
#ifdef _DEBUG
			count++;
#endif
			// apply alignment offset
			top += offset;
			// this is our reserved address
			handles.emplace_front(top, std::log2(alignment));
			// reserve space
			top += numBytes;
			// update max alignment
			maxAlignment = std::max(maxAlignment, alignment);
			// success
			return &handles.front();
		}

		// failed to reserve
		return nullptr;
	}

	void Manager::Heap::Defrag() noexcept
	{
		while (!handles.empty() && !handles.front().Used())
		{
			DebugFill(handles.front().ptr, top);
			top -= top - handles.front().ptr;
			handles.pop_front();
			DebugDecCount();
		}
		
		if (handles.empty()) [[unlikely]]
		{
			return;
		}

		// We're going to re-calculate this as we go.
		// This is because the largest alignment required could have been freed.
		maxAlignment = 1;

		// keep in mind the list is in reverse order of allocation
		// so next is the previous node but the next address
		auto curr = handles.begin();
		auto next = curr++;
		while (curr != handles.end())
		{
			if (!curr->Used()) [[unlikely]]
			{
				// how big is this object
				size_t byteCount = next->ptr - curr->ptr;

				// shuffle memory
				Memmove(curr->ptr, next->ptr, top - next->ptr);
				DebugFill(top - byteCount, top);
				
				// adjust the top			
				top -= byteCount;
				maxAlignment = std::max(maxAlignment, next->Alignment());
				const size_t offset = size_t(top) % maxAlignment;
				top += offset;
				byteCount -= offset;

				// done with this handle
				++curr;
				handles.erase_after(next);
				DebugDecCount();

				// update the handles
				for (auto it = handles.begin(); it != curr; ++it)
				{
					it->ptr -= byteCount;
				}
			}
			else [[likely]]
			{
				next = curr++;
			}
		}
	}

	void Manager::Heap::Graduate() noexcept
	{
		if (handles.empty()) [[unlikely]]
		{
			return;
		}
		
		// Get the next Heap.
		Heap* next = Next();
		
		// If there isn't one, make one (acquiring more memory from the system).
		if (!next) [[unlikely]]
		{
			next = &MakeHeap();
		}

		// figure out exactly how many bytes we need
		const size_t numBytes = top - begin;
		size_t offset = size_t(next->top) % maxAlignment;
		
		// Next Heap is too full, it must Graduate too.
		if (!next->CanFit(offset + numBytes))
		{
			next->Graduate();
			offset = size_t(next->top) % maxAlignment;
		}
		next->top += offset;

		// update the handles
		std::byte* addr = next->top;
		auto nextIt = handles.begin();
		auto currIt = nextIt++;
		while (nextIt != handles.end())
		{
			currIt->ptr = addr;
			addr += nextIt->ptr - currIt->ptr;
			currIt = nextIt++;
		}
		currIt->ptr = addr;

		// copy the memory
		Memcpy(next->top, begin, numBytes);
		next->top += numBytes;
		next->maxAlignment = std::max(next->maxAlignment, maxAlignment);
		
		// give up all of our handles
		next->handles.splice_after(next->handles.before_begin(), handles);
#ifdef _DEBUG
		next->count += count;
		count = 0;
#endif
		
		// reset this Heap
		DebugFill(begin, top);
		top = begin;
		maxAlignment = 1;
	}

#pragma region helpers
	void Manager::Heap::DebugFill([[maybe_unused]] std::byte* from, [[maybe_unused]] std::byte* to) noexcept
	{
#ifdef _DEBUG
		const size_t numBytes = to - from;
		const size_t numWords = numBytes / 2;
		Memset(from, uint16_t(0xF1EA), numWords);
		if (numBytes % 2)
		{
			*(to - 1) = std::byte(0xF1);
		}
#endif
	}

	void Manager::Heap::DebugIncCount() noexcept
	{
#ifdef _DEBUG
		count++;
#endif
	}

	void Manager::Heap::DebugDecCount() noexcept
	{
#ifdef _DEBUG
		if (--count == 0)
		{
			assertm(top == begin, "memory leak");
		}
#endif
	}
#pragma endregion
#pragma endregion
	
	Manager::Handle& Manager::Alloc(const size_t numBytes, const size_t alignment) noexcept
	{		
		for (Heap& heap : heaps)
		{
			if (Handle* ret = heap.Alloc(numBytes, alignment))
			{
				return *ret;
			}
		}
		
		for EVER
		{
			if (Handle* ret = MakeHeap().Alloc(numBytes, alignment))
			{
				return *ret;
			}
		}
	}

	void Manager::Defrag() noexcept
	{
		for (Heap& heap : heaps)
		{
			heap.Defrag();
		}
	}

	void Manager::Graduate() noexcept
	{
		heaps.front().Graduate();
	}

	Manager::Heap& Manager::MakeHeap() noexcept
	{
		return heaps.emplace_back(heaps.size());
	}
}
