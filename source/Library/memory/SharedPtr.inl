#pragma once
#include "SharedPtr.h"

namespace Library
{
#pragma region special members	
	template<typename T>
	SharedPtr<T>::SharedPtr(T* ptr) noexcept :
		Base(new typename Base::Handle(ptr, 1)) {}

	template <typename T>
	SharedPtr<T>::SharedPtr(const SharedPtr& other) noexcept :
		Base(other.handle)
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
				if (this->handle->weakCount == 0)
				{
					delete this->handle;
					this->handle = nullptr;
				}
				else
				{
					delete this->handle->ptr;
					this->handle->ptr = nullptr;
				}
			}
		}		
	}
#pragma endregion
}
