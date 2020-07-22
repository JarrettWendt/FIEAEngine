#pragma once
#include <deque>
#include <forward_list>

#include "Macros.h"
#include "SmartPtr.h"

namespace Library::Memory
{
	/**
	 * Memory Manager used by SmartPtr 
	 */
	class Manager final
	{
		using Handle = SmartPtr<std::byte>::Handle;
		
		class Heap final
		{		
			constexpr static size_t byteFactor = 1024;
			
			std::byte* begin;
			std::byte* end;
			std::byte* top;
			size_t index;
			
			// TODO: A semi-contiguous array such as std::deque would be better.
			// That way, we have a better chance of things being in the cache.
			// This is assuming that things allocated at roughly the same time are likely to be used together.
			// However, we'd want to be able to guarantee that all nodes are the size of a cache line.
			// We'd also want to only delete a node if all Handles in it are unused.
			std::forward_list<Handle> handles{};

		public:
			Heap() = delete;
			explicit Heap(size_t index) noexcept;
			MOVE_COPY_DTOR(Heap, default)

#pragma region properties
			bool Contains(const std::byte* addr) const noexcept;
			bool Allocated(const std::byte* addr) const noexcept;
			bool CanFit(size_t numBytes) const noexcept;
			size_t NumBytesAllocated() const noexcept;
			Heap* Next() noexcept;
#pragma endregion

			Handle* Alloc(size_t numBytes, size_t alignment) noexcept;

			/**
			 * Shuffles memory such that all unused memory is at the end.
			 */
			void Defrag() noexcept;

			/**
			 * Copies memory to the next Heap.
			 * Resets this Heap.
			 */
			void Graduate() noexcept;

		private:
#pragma region helpers
			void Free(std::byte* from, std::byte* to) noexcept;
			bool Copy(Heap& other) noexcept;
			void Copy(std::byte*& from, std::byte* to, const size_t alignment) noexcept;
			void Reset() noexcept;
			void DebugFill(std::byte* from, std::byte* to) noexcept;
#pragma endregion
		};

		friend Heap;
		
		static inline std::deque<Heap> heaps{ Heap(0), Heap(1) };
		
	public:
		STATIC_CLASS(Manager)

		template<typename T>
		static typename SmartPtr<T>::Handle& Alloc(size_t count = 1) noexcept;
		
		template<typename T, typename... Args>
		static typename SmartPtr<T>::Handle& Emplace(Args... args);
		
		static Handle& Alloc(size_t numBytes, size_t alignment) noexcept;

		static void Defrag() noexcept;
	};
}

#include "Manager.inl"
