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
		struct Handle final
		{
			T* ptr;
			uint32_t sharedCount;
			uint32_t weakCount;
			
			explicit Handle(T* ptr, const uint32_t count) noexcept :
				ptr(ptr),
				sharedCount(count),
				weakCount(0) {}
						
			~Handle()
			{
				delete ptr;
				ptr = nullptr;
				sharedCount = weakCount = 0;
			}
			
			Handle() noexcept = default;
			MOVE_COPY(Handle, delete)
		};

		Handle* handle;

		explicit SmartPtr(Handle* block = nullptr) noexcept :
			handle(block) {}
		
	public:
		T& operator*()
		{
			if (handle && handle->ptr)
			{
				return *handle->ptr;
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
			return handle && handle->ptr;
		}
		
		size_t ReferenceCount() noexcept
		{
			return handle ? handle->sharedCount : 0;
		}
		
		T* Raw() noexcept
		{
			return handle ? handle->ptr : nullptr;
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
