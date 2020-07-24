#pragma once
#include "SmartPtr.h"
#include "SharedPtr.h"

namespace Library
{
	/**
	 * Non-owning reference to an existing SharedPtr. 
	 */
	template<typename T>
	class WeakPtr : public SmartPtr<T>
	{
		using Base = SmartPtr<T>;

	public:
		WeakPtr(const SharedPtr<T>& shared) noexcept;
		
		WeakPtr() noexcept = default;
		WeakPtr(const WeakPtr& other) noexcept;
		WeakPtr(WeakPtr&& other) noexcept;
		WeakPtr& operator=(const WeakPtr& other) noexcept;
		WeakPtr& operator=(WeakPtr&& other) noexcept;
		~WeakPtr() noexcept;

		operator SharedPtr<T>() noexcept;

		bool Expired() const noexcept;
	};
}

#include "WeakPtr.inl"
