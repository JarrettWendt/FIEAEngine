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
			// Heap i will be `byteFactor << i` bytes big.
			constexpr static size_t byteFactor = 1024;
			
			// TODO: A semi-contiguous array such as std::deque would be better.
			// That way, we have a better chance of things being in the cache.
			// This is assuming that things allocated at roughly the same time are likely to be used together.
			// However, we'd want to be able to guarantee that all nodes are the size of a cache line.
			// We'd also want to only delete a node if all Handles in it are unused.
			using Handles = std::forward_list<Handle>;
			Handles handles{};

			// these pointers make up the "stack" of our heap
			std::byte* const begin;
			std::byte* const end;
			std::byte* top;

#ifdef _DEBUG
			size_t count{ 0 };
#endif
			
			/**
			 * Size of this heap can be calculated from `byteFactor << index`
			 * Also used to get the next Heap from the Manager.
			 */
			const size_t index;
			/**
			 * The overall alignment of this Heap is that of the largest allocation within it.
			 * Used when Graduating.
			 */
			size_t maxAlignment{ 1 };

		public:
			Heap() = delete;
			explicit Heap(size_t index) noexcept;
			MOVE_COPY_DTOR(Heap, default)

#pragma region properties
			bool Contains(const std::byte* addr) const noexcept;
			bool ContainsAllocated(const std::byte* addr) const noexcept;
			
			bool CanFit(size_t numBytes) const noexcept;
			
			size_t AllocatedBytes() const noexcept;
			size_t TotalBytes() const noexcept;
			
			Heap* Next() noexcept;
#pragma endregion

			Handle* Alloc(size_t numBytes, size_t alignment) noexcept;

			/**
			 * Shuffles memory such that all unused memory is at the end.
			 */
			void Defrag() noexcept;

			/**
			 * Migrates memory from this Heap into the next one.
			 * If the next Heap cannot fit this one's memory, it too must Graduate.
			 * If there is no next Heap, one will be created.
			 * This Heap will be empty after this operation.
			 */
			void Graduate() noexcept;

		private:
#pragma region helpers
			void Free(std::byte* from, std::byte* to) noexcept;

			/**
			 * Fill memory with debug values (only in debug compilations).
			 * 
			 * @param from		address to start debug values at
			 * @param to		address to end debug values at
			 */
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

	private:
		static Heap& MakeHeap() noexcept;
	};
}

#include "Manager.inl"
