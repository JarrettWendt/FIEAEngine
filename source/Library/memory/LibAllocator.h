#pragma once
#include <cstddef>
#include <limits>

#include "Macros.h"
#include "Memory.h"

namespace Library
{
	template<typename T>
	class Allocator
	{		
	public:
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using value_type = T;
		using propagate_on_container_move_assignment = std::true_type;
		using is_always_equal = std::true_type;

	private:
		inline static size_type numAllocations{};

	public:
		SPECIAL_MEMBERS(Allocator, default)
		
		template<typename U>
		constexpr Allocator(const Allocator<U>&) noexcept {}
		
		[[nodiscard]] T* allocate(const size_type n)
		{
#ifdef _DEBUG
			numAllocations++;
#endif
			return Memory::Malloc<T>(n);
		}

		void deallocate(T* p, size_type)
		{
#ifdef _DEBUG
			numAllocations--;
			assertm(numAllocations != std::numeric_limits<size_type>::max(), "underflow");
#endif
			Memory::Free(p);
		}

		static constexpr size_type NumAllocations() noexcept
		{
			return numAllocations;
		}
	};
}
