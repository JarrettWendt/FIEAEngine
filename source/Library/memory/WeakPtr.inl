#pragma once
#include "WeakPtr.h"

namespace Library
{
#pragma region special members
	template<typename T>
	WeakPtr<T>::WeakPtr(const SharedPtr<T>& shared) noexcept :
		Base(reinterpret_cast<const WeakPtr<T>&>(shared).block)
	{
		if (this->block)
		{
			++this->block->weakCount;
		}
	}

	template<typename T>
	WeakPtr<T>::WeakPtr(const WeakPtr& other) noexcept :
		Base(other.block)
	{
		if (this->block)
		{
			++this->block->weakCount;
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
			this->block = other.block;
			if (this->block)
			{
				++this->block->weakCount;
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
			this->block = other.block;
			other.block = nullptr;
		}
		return *this;
	}
	
	template<typename T>
	WeakPtr<T>::~WeakPtr() noexcept
	{
		if (this->block)
		{
			--this->block->weakCount;
			if (this->block->sharedCount == 0 && this->block->weakCount == 0)
			{
				delete this->block;
				this->block = nullptr;
			}
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
		return !this->block || this->block->sharedCount == 0; 
	}
}
