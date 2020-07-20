#pragma once
#include "SmartPtr.h"

namespace Library
{
	template <typename T>
	SmartPtr<T>::Handle::Handle(T* ptr, const uint32_t count) noexcept :
		ptr(ptr),
		sharedCount(count),
		weakCount(0) {}

	template <typename T>
	SmartPtr<T>::Handle::~Handle() noexcept
	{
#ifdef _DEBUG
		ptr = nullptr;
		sharedCount = weakCount = 0;
#endif
	}

	template <typename T>
	SmartPtr<T>::SmartPtr(Handle* handle) noexcept :
		handle(handle) {}

	template <typename T>
	SmartPtr<T>::SmartPtr(const SmartPtr& other) noexcept :
		handle(other.handle) {}

	template <typename T>
	SmartPtr<T>::SmartPtr(SmartPtr&& other) noexcept :
		handle(other.handle)
	{
		other.handle = nullptr;
	}

	template <typename T>
	SmartPtr<T>& SmartPtr<T>::operator=(const SmartPtr& other) noexcept
	{
		handle = other.handle;
		return *this;
	}

	template <typename T>
	SmartPtr<T>& SmartPtr<T>::operator=(SmartPtr&& other) noexcept
	{
		if (this != &other)
		{
			handle = other.handle;
			other.handle = nullptr;
		}
		return *this;
	}

	template <typename T>
	T& SmartPtr<T>::operator*()
	{
		if (handle && handle->ptr)
		{
			return *handle->ptr;
		}
		throw NullReferenceException();
	}
	
	template <typename T>
	const T& SmartPtr<T>::operator*() const
	{
		return const_cast<SmartPtr<T>*>(this)->operator*();
	}
	
	template <typename T>
	T* SmartPtr<T>::operator->()
	{
		return &operator*();
	}
	
	template <typename T>
	const T* SmartPtr<T>::operator->() const
	{
		return const_cast<SmartPtr<T>*>(this)->operator->();
	}

	template <typename T>
	SmartPtr<T>::operator bool() const noexcept
	{
		return handle && handle->ptr;
	}

	template <typename T>
	size_t SmartPtr<T>::ReferenceCount() noexcept
	{
		return handle ? handle->sharedCount : 0;
	}

	template <typename T>
	T* SmartPtr<T>::Raw() noexcept
	{
		return handle ? handle->ptr : nullptr;
	}
}
