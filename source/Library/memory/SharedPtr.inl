#pragma once
#include "SharedPtr.h"

namespace Library
{
#pragma region special members	
	template<typename T>
	SharedPtr<T>::SharedPtr(T* ptr) noexcept :
		Base(new typename Base::Block(ptr, 1)) {}

	template <typename T>
	SharedPtr<T>::SharedPtr(const SharedPtr& other) noexcept :
		Base(other.block)
	{
		if (this->block)
		{
			++this->block->sharedCount;
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
			this->block = other.block;
			if (this->block)
			{
				++this->block->sharedCount;
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
			this->block = other.block;
			other.block = nullptr;
		}
		return *this;
	}

	template<typename T>
	SharedPtr<T>::~SharedPtr()
	{
		if (this->block)
		{
			--this->block->sharedCount;
			if (this->block->sharedCount == 0)
			{
				if (this->block->weakCount == 0)
				{
					delete this->block;
					this->block = nullptr;
				}
				else
				{
					delete this->block->ptr;
					this->block->ptr = nullptr;
				}
			}
		}		
	}
#pragma endregion
}
