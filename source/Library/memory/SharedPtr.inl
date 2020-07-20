#pragma once
#include "SharedPtr.h"

namespace Library
{
#pragma region special members	
	template<typename T>
	SharedPtr<T>::SharedPtr(T* ptr) noexcept :
		Base(new typename Base::Handle(ptr, 1))
	{
		if constexpr (std::derived_from<T, EnableSharedFromThis<T>>)
		{
			reinterpret_cast<SharedPtr&>(ptr->weakThis).handle = this->handle;
			++this->handle->weakCount;
		}
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(nullptr_t) noexcept :
		Base(nullptr) {}

	template<typename T>
	template<typename U>
	SharedPtr<T>::SharedPtr(const SharedPtr<U>& other) noexcept :
		Base(reinterpret_cast<const SharedPtr&>(other))
	{
		if (this->handle)
		{
			++this->handle->sharedCount;
		}
	}

	template<typename T>
	template<typename U>
	SharedPtr<T>::SharedPtr(SharedPtr<U>&& other) noexcept :
		Base(std::move(reinterpret_cast<SharedPtr&&>(other))) {}
	
	template<typename T>
	template<typename U>
	SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<U>& other) noexcept
	{
		this->~SharedPtr();
		this->handle = reinterpret_cast<const SharedPtr&>(other).handle;
		if (this->handle)
		{
			++this->handle->sharedCount;
		}
		return *this;
	}

	template<typename T>
	template<typename U>
	SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<U>&& other) noexcept
	{
		this->~SharedPtr();
		auto& handle = reinterpret_cast<const SharedPtr&>(other).handle;		
		this->handle = handle;
		handle = nullptr;
		return *this;
	}
	
	template<typename T>
	SharedPtr<T>::SharedPtr(const SharedPtr& other) noexcept :
		Base(other)
	{
		if (this->handle)
		{
			++this->handle->sharedCount;
		}
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept :
		Base(std::move(other)) {}

	template<typename T>
	SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other) noexcept
	{
		if (this != &other)
		{
			this->~SharedPtr();
			this->handle = other.handle;
			if (this->handle)
			{
				++this->handle->sharedCount;
			}
		}
		return *this;
	}

	template<typename T>
	SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other) noexcept
	{
		if (this != &other)
		{
			this->~SharedPtr();
			this->handle = other.handle;
			other.handle = nullptr;
		}
		return *this;
	}

	template<typename T>
	SharedPtr<T>::~SharedPtr()
	{
		if (this->handle)
		{
			--this->handle->sharedCount;
			if (this->handle->sharedCount == 0)
			{
				delete this->handle->ptr;
				if (this->handle->weakCount == 0)
				{
					delete this->handle;
				}
				else
				{
					this->handle->ptr = nullptr;
				}
#ifdef _DEBUG
				this->handle = nullptr;
#endif
			}
		}		
	}
#pragma endregion
}
