#pragma once
#include "WeakPtr.h"

namespace Library
{
#pragma region special members
	template<typename T>
	WeakPtr<T>::WeakPtr(const SharedPtr<T>& shared) noexcept :
		Base(reinterpret_cast<const WeakPtr&>(shared))
	{
		if (this->handle)
		{
			++this->handle->weakCount;
		}
	}

	template<typename T>
	WeakPtr<T>::WeakPtr(const WeakPtr& other) noexcept :
		Base(other)
	{
		if (this->handle)
		{
			++this->handle->weakCount;
		}
	}

	template<typename T>
	WeakPtr<T>::WeakPtr(WeakPtr&& other) noexcept :
		Base(std::move(other)) {}

	template<typename T>
	WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& other) noexcept
	{
		if (this != &other)
		{
			this->~WeakPtr();
			this->handle = other.handle;
			if (this->handle)
			{
				++this->handle->weakCount;
			}
		}
		return *this;
	}

	template<typename T>
	WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& other) noexcept
	{
		if (this != &other)
		{
			this->~WeakPtr();
			this->handle = other.handle;
			other.handle = nullptr;
		}
		return *this;
	}
	
	template<typename T>
	WeakPtr<T>::~WeakPtr() noexcept
	{
		if (this->handle)
		{
			--this->handle->weakCount;
			// No need to free the handle's memory, that will be done in Memory::Manager
#ifdef _DEBUG
			this->handle = nullptr;
#endif
		}
	}
#pragma endregion
	
	template<typename T>
	WeakPtr<T>::operator SharedPtr<T>() noexcept
	{
		// the copy ctor handles reference counting
		return *reinterpret_cast<SharedPtr<T>*>(this);
	}

	template<typename T>
	bool WeakPtr<T>::Expired() const noexcept
	{
		return !this->handle || this->handle->sharedCount == 0; 
	}
}
