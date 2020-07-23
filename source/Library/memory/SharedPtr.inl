#pragma once
#include "SharedPtr.h"

namespace Library
{
#pragma region special members
	template <typename T>
	SharedPtr<T>::SharedPtr(typename Base::Handle& handle) noexcept :
		Base(handle)
	{
		if constexpr (std::derived_from<T, EnableSharedFromThis<T>>)
		{
			reinterpret_cast<SharedPtr&>(handle.ptr->weakThis).handle = &handle;
			++handle.weakCount;
		}
		++handle.sharedCount;
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(nullptr_t) noexcept :
		Base() {}

	template<typename T>
	template<Concept::Related<T> U>
	SharedPtr<T>::SharedPtr(const SharedPtr<U>& other) noexcept :
		Base(reinterpret_cast<const SharedPtr&>(other))
	{
		if (this->handle)
		{
			++this->handle->sharedCount;
		}
	}

	template<typename T>
	template<Concept::Related<T> U>
	SharedPtr<T>::SharedPtr(SharedPtr<U>&& other) noexcept :
		Base(std::move(reinterpret_cast<SharedPtr&&>(other))) {}
	
	template<typename T>
	template<Concept::Related<T> U>
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
	template<Concept::Related<T> U>
	SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<U>&& other) noexcept
	{
		this->~SharedPtr();
		auto& handle = reinterpret_cast<SharedPtr&>(other).handle;		
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
				// No need to free the handle's memory, that will be done in Memory::Manager
				this->handle->ptr->~T();
#ifdef _DEBUG
				this->handle = nullptr;
#endif
			}
		}		
	}
#pragma endregion
}
