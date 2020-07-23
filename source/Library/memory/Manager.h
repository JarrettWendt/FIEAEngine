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

			void DebugIncCount() noexcept;
			void DebugDecCount() noexcept;
#pragma endregion
		};

		friend Heap;
		
		static inline std::deque<Heap> heaps{ Heap(0), Heap(1) };
		
	public:
		STATIC_CLASS(Manager)

		/**
		 * Let C++'s compile-time type system figure out numBytes and alignment for you
		 */
		template<typename T>
		static typename SmartPtr<T>::Handle& Alloc(size_t count = 1) noexcept;

		/**
		 * Allocate and construct in-place.
		 */
		template<typename T, typename... Args>
		static typename SmartPtr<T>::Handle& Emplace(Args... args);

		/**
		 * Allocate space.
		 * Will end up reserving potentially more than numBytes due to padding to satisfy alignment.
		 * Guarantees no other managed memory will be touched.
		 *
		 * @param numBytes		minimum number of bytes to reserve
		 * @param alignment		desired alignment of the bytes
		 */
		static Handle& Alloc(size_t numBytes, size_t alignment) noexcept;

		/**
		 * Removes fragments from all managed Heaps.
		 */
		static void Defrag() noexcept;

		/**
		 * Graduates the smallest Heap to the next.
		 * If the next Heap doesn't have enough space, it must Graduate too, starting a chain reaction.
		 * Will potentially allocate a new Heap if there's not enough space to Graduate all the way up.
		 */
		static void Graduate() noexcept;

	private:
		/**
		 * Constructs a new Heap at twice the size of the current largest Heap.
		 */
		static Heap& MakeHeap() noexcept;
	};
}

#include "Manager.inl"
