#pragma once
#include "EnableSharedFromThis.h"

namespace Library
{
	template<typename Derived>
	inline EnableSharedFromThis<Derived>::EnableSharedFromThis(const EnableSharedFromThis&) noexcept :
		weakThis() {} // SmartPtr ctor will initialize weakThis
	
	template<typename Derived>
	inline EnableSharedFromThis<Derived>& EnableSharedFromThis<Derived>::operator=(const EnableSharedFromThis&) noexcept
	{
		// assign must not change weakThis
		return *this;
	}

	template<typename Derived>
	inline SharedPtr<Derived> EnableSharedFromThis<Derived>::SharedFromThis()
	{
		return SharedPtr<Derived>(WeakFromThis());
	}
	
	template<typename Derived>
	inline SharedPtr<const Derived> EnableSharedFromThis<Derived>::SharedFromThis() const
	{
		return const_cast<EnableSharedFromThis*>(this)->SharedFromThis();
	}
	
	template<typename Derived>
	inline WeakPtr<Derived> EnableSharedFromThis<Derived>::WeakFromThis()
	{
		return weakThis;
	}
	
	template<typename Derived>
	inline WeakPtr<const Derived> EnableSharedFromThis<Derived>::WeakFromThis() const
	{
		return const_cast<EnableSharedFromThis*>(this)->WeakFromThis();
	}
}
