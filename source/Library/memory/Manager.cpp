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

	bool Manager::Heap::Allocated(const std::byte* addr) const noexcept
	{
		return begin <= addr && addr < top;
	}

	bool Manager::Heap::CanFit(const size_t numBytes) const noexcept
	{
		return top + numBytes < end;
	}

	size_t Manager::Heap::NumBytesAllocated() const noexcept
	{
		return top - begin;
	}

	Manager::Heap* Manager::Heap::Next() noexcept
	{
		if (index < heaps.size())
		{
			return &heaps[index];
		}
		return nullptr;
	}
#pragma endregion

	Manager::Handle* Manager::Heap::Alloc(const size_t numBytes, const size_t alignment) noexcept
	{
		// calculate alignment offset
		const size_t offset = size_t(top) % alignment;

		// try to reserve bytes
		if (CanFit(offset + numBytes))
		{
			// apply alignment offset
			top += offset;
			// this is our reserved address
			handles.emplace_front(top, std::log2(alignment));
			// reserve space
			top += numBytes;
			return &handles.front();
		}

		// failed to reserve
		return nullptr;
	}

	void Manager::Heap::Defrag() noexcept
	{
		if (handles.empty()) [[unlikely]]
		{
			return;
		}
		
		auto curr = handles.begin();
		auto prev = curr++;

		while (curr != handles.end())
		{
			if (!curr->Used()) [[unlikely]]
			{
				size_t byteCount = prev->ptr - curr->ptr;

				// shuffle the memory
				Free(curr->ptr, prev->ptr);
				++curr;
				handles.erase_after(prev);

				// update the handles
				for (auto it = handles.begin(); it != curr; ++it)
				{
					it->ptr -= byteCount;
					const size_t offset = size_t(it->ptr) % it->Alignment();
					it->ptr += offset;
					byteCount -= offset;
				}
			}
			else [[likely]]
			{
				prev = curr++;
			}
		}

		// loop checked all except the first handle
		Handle& handle = handles.front();
		if (!handle.Used())
		{
			Free(handle.ptr, top);
			handles.pop_front();
		}
	}

	void Manager::Heap::Graduate() noexcept
	{
		Heap* next = Next();
		if (!next)
		{
			next = &heaps.emplace_back(heaps.size());
		}

		if (!next->Copy(*this))
		{
			next->Graduate();
			const bool copied = next->Copy(*this);
			assertm(copied, "failed to graduate heap");
		}
		
		Reset();
	}

#pragma region helpers
	void Manager::Heap::Free(std::byte* from, std::byte* to) noexcept
	{
		if (to > from && Allocated(from) && Allocated(to - 1))
		{
			const size_t numBytes = top - to;
			Memmove(from, to, numBytes);
			DebugFill(top - numBytes, top);
			top -= numBytes;
		}
	}
	
	bool Manager::Heap::Copy(Heap& other) noexcept
	{
		// this isn't the exact amount due to padding, but we can guess
		if (CanFit(other.top - other.begin))
		{
			other.handles.reverse();
			
			auto next = other.handles.begin();
			auto curr = next++;
			
			while (next != other.handles.end())
			{
				Copy(curr->ptr, next->ptr, curr->Alignment());

				// take the handle
				handles.splice_after(handles.before_begin(), other.handles, other.handles.before_begin(), curr);
				curr = next++;
			}

			Handle& handle = *other.handles.begin();
			Copy(handle.ptr, other.top, curr->Alignment());
			handles.splice_after(handles.before_begin(), other.handles);
			
			return true;
		}
		return false;
	}

	void Manager::Heap::Copy(std::byte*& from, std::byte* to, const size_t alignment) noexcept
	{
		const size_t numBytes = to - from;
		const size_t offset = size_t(top) % alignment;

		// move the memory
		top += offset;
		Memmove(top, from, numBytes);
		from = top;
		top += numBytes;
	}

	void Manager::Heap::Reset() noexcept
	{
		DebugFill(begin, top);
		top = begin;
	}

	void Manager::Heap::DebugFill(std::byte* from, std::byte* to) noexcept
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
#pragma endregion
#pragma endregion
	
	Manager::Handle& Manager::Alloc(const size_t numBytes, const size_t alignment) noexcept
	{
		// reserve space
		Handle* ret = heaps.front().Alloc(numBytes, alignment);
		
		if (!ret)
		{
			// failed to reserve, defrag and try again
			Defrag();
			ret = heaps.front().Alloc(numBytes, alignment);
			assertm(ret, "failed to allocate memory");
		}

		// successful
		return *ret;
	}

	void Manager::Defrag() noexcept
	{
		for (Heap& heap : heaps)
		{
			heap.Defrag();
		}
		heaps.front().Graduate();
	}
}
