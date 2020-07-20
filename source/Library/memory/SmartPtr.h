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
			
			explicit Handle(T* ptr, const uint32_t count) noexcept;						
			
			Handle() noexcept = default;
			~Handle() noexcept;
			MOVE_COPY(Handle, delete)
		};

		Handle* handle;

		explicit SmartPtr(Handle* handle = nullptr) noexcept;
		SmartPtr(const SmartPtr& other) noexcept;		
		SmartPtr(SmartPtr&& other) noexcept;
		SmartPtr& operator=(const SmartPtr& other) noexcept;
		SmartPtr& operator=(SmartPtr&& other) noexcept;
		~SmartPtr() noexcept = default;
		
	public:
		T& operator*();
		const T& operator*() const;

		T* operator->();
		const T* operator->() const;

		explicit operator bool() const noexcept;
		
		size_t ReferenceCount() noexcept;		
		T* Raw() noexcept;

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

#include "SmartPtr.inl"
