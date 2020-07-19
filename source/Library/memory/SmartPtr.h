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
						
			
			Handle() noexcept = default;
#ifdef _DEBUG			
			~Handle() noexcept
			{
				ptr = nullptr;
				sharedCount = weakCount = 0;
			}
#else
			~Handle() noexcept = default;
#endif
			MOVE_COPY(Handle, delete)
		};

		Handle* handle;

		explicit SmartPtr(Handle* block = nullptr) noexcept :
			handle(block) {}

		SmartPtr(const SmartPtr& other) noexcept :
			handle(other.handle) {}
		
		SmartPtr(SmartPtr&& other) noexcept :
			handle(other.handle)
		{
			other.handle = nullptr;
		}

		SmartPtr& operator=(const SmartPtr& other) noexcept
		{
			handle = other.handle;
			return *this;
		}

		SmartPtr& operator=(SmartPtr&& other) noexcept
		{
			if (this != &other)
			{
				handle = other.handle;
				other.handle = nullptr;
			}
			return *this;
		}

		~SmartPtr() noexcept = default;
		
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

		explicit operator bool() const noexcept
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
			if constexpr (Concept::Ostreamable<T>)
			{				
				if (smart)
				{
					return stream << *smart;
				}
				return stream << nullptr;
			}
			else
			{
				return stream << smart.handle;
			}
		}

		template<typename U>
		friend bool operator==(const SmartPtr& a, const SmartPtr<U>& b) noexcept
		{
			return a.handle == reinterpret_cast<const SmartPtr&>(b).handle;
		}

		template<typename U>
		friend bool operator!=(const SmartPtr& a, const SmartPtr<U>& b) noexcept
		{
			return !operator==(a, b);
		}
	};
}
