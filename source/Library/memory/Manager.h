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

		/**
		 * A single contiguous block of memory from which objects may reserve space.
		 */
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
			/**
			 * Whether or not this Heap is totally empty, free of any allocations.
			 * If this Heap is desperately in need of defragging it's possible this Heap could be free yet the Heap doesn't know it.
			 */
			bool IsEmpty() const noexcept;

			/**
			 * Whether or not this Heap has enough contiguous space to fit an allocation of this size in O(1) time.
			 * Does not take alignment into account, which may require extra pad bytes.
			 * This Heap may in fact have enough space for this allocation yet return false if this Heap is desperately in need of defragging.
			 * Useful for a quick comparison without doing any more expensive operations.
			 */
			bool CanFit(size_t numBytes) const noexcept;

			/**
			 * This Heap has reserved *at most* this many bytes.
			 * Could have less if this Heap is desperately in need of defragging.
			 */
			size_t TotalBytes() const noexcept;

			/**
			 * @returns			the Heap after this one
			 * @returns nullptr	if there is not Heap after this one (might want to call MakeHeap)
			 * 
			 */
			Heap* Next() noexcept;
#pragma endregion

			/**
			 * O(1)
			 *
			 * @returns pointer that satisfies alignment and has at least numBytes reserved after it
			 * @returns nullptr	if no allocation could be made (might need to Defrag or Graduate)
			 */
			Handle* Alloc(size_t numBytes, size_t alignment) noexcept;

			/**
			 * O(n^2)
			 *
			 * Shuffles memory such that all unused memory is at the end.
			 */
			void Defrag() noexcept;

			/**
			 * O(n)
			 *
			 * Migrates memory from this Heap into the next one.
			 * If the next Heap cannot fit this one's memory, it too must Graduate.
			 * If there is no next Heap, one will be created.
			 * This Heap will be empty after this operation.
			 */
			void Graduate() noexcept;

			/**
			 * O(n) where n is the number of Unused Handles at the top of the Heap.
			 *
			 * Defrag without shuffling.
			 * Only frees memory at the top of the Heap.
			 */
			void ShrinkToFit() noexcept;

		private:
#pragma region helpers
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

#pragma region properties
		static bool IsEmpty() noexcept;
		static size_t TotalBytes() noexcept;
#pragma endregion
		
#pragma region alloc
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
#pragma endregion

#pragma region gc
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

		/**
		 * Keeps deleting the last Heap as long as the last Heap is empty.
		 * Will not delete the 0th or 1st Heap.
		 */
		static void ShrinkToFit() noexcept;
#pragma endregion

	private:
		/**
		 * Constructs a new Heap at twice the size of the current largest Heap.
		 */
		static Heap& MakeHeap() noexcept;
	};
}

#include "Manager.inl"
