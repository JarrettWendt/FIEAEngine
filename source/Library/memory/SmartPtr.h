#pragma once
#include <ostream>
#include <stdexcept>

#include "Macros.h"

namespace Library
{
	namespace Memory
	{
		class Manager;
	}
	
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
		friend Memory::Manager;
		
	protected:
		struct Handle final
		{			
			T* ptr;
			uint32_t sharedCount;
			struct
			{
				// 1 << alignment gives us alignof(T)
				// needed by Memory::Manager who has no type information
				uint32_t alignment : 5;
				uint32_t weakCount : 27;
			};
			explicit Handle(T* ptr, const uint32_t alignment) noexcept;						
			
			Handle() noexcept = default;
			~Handle() noexcept;
			MOVE_COPY(Handle, default)

			constexpr bool Used() const noexcept;
			constexpr size_t Alignment() const noexcept;
		};

		Handle* handle{};

		explicit SmartPtr(Handle& handle) noexcept;
		
		SmartPtr() noexcept = default;
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
