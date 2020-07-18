#pragma once
#include <ostream>
#include <stdexcept>

#include "Macros.h"

namespace Library
{
	/**
	 * exception thrown when trying to dereference an uninitialized SmartPtr.
	 */
	struct NullReferenceException : public std::runtime_error
	{
		using Base = std::runtime_error;
		using Base::Base;
		NullReferenceException() : Base("") {}
	};
	
	/**
	 * Base class for SharedPtr and WeakPtr.
	 *
	 * @param <T>	type this pointer references
	 */
	template<typename T>
	class SmartPtr
	{
	protected:
		struct Block final
		{
			T* ptr;
			uint32_t sharedCount;
			uint32_t weakCount;
			
			explicit Block(T* ptr, const uint32_t count) noexcept :
				ptr(ptr),
				sharedCount(count),
				weakCount(0) {}
						
			~Block()
			{
				delete ptr;
				ptr = nullptr;
				sharedCount = 0;
			}
			
			Block() noexcept = default;
			MOVE_COPY(Block, delete)
		};

		Block* block;

		explicit SmartPtr(Block* block = nullptr) noexcept :
			block(block) {}
		
	public:
		T& operator*()
		{
			if (block && block->ptr)
			{
				return *block->ptr;
			}
			throw NullReferenceException();
		}
		const T& operator*() const
		{
			return const_cast<SmartPtr<T>*>(this)->operator*();
		}

		T* operator->()
		{
			return &operator*();
		}
		const T* operator->() const
		{
			return const_cast<SmartPtr<T>*>(this)->operator->();
		}

		operator bool() const noexcept
		{
			return block && block->ptr;
		}
		
		size_t ReferenceCount() noexcept
		{
			return block ? block->sharedCount : 0;
		}
		
		T* Raw() noexcept
		{
			return block ? block->ptr : nullptr;
		}

		friend std::ostream& operator<<(std::ostream& stream, const SmartPtr& smart)
		{
			if (smart)
			{
				return stream << *smart;
			}
			return stream << nullptr;
		}
	};
}

#include "SharedPtr.h"
#include "WeakPtr.h"
