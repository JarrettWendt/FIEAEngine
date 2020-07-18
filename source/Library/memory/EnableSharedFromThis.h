#pragma once
#include "SmartPtr.h"

namespace Library
{
	template<typename Derived>
	class EnableSharedFromThis
	{
		WeakPtr<Derived> weakThis{};

	public:
		constexpr EnableSharedFromThis() noexcept = default;
		EnableSharedFromThis(const EnableSharedFromThis&) noexcept;
		EnableSharedFromThis& operator=(const EnableSharedFromThis&) noexcept;
		~EnableSharedFromThis() noexcept = default;
		MOVE_SEMANTICS(EnableSharedFromThis, default)
		
		SharedPtr<Derived> SharedFromThis();
		SharedPtr<const Derived> SharedFromThis() const;

		WeakPtr<Derived> WeakFromThis();
		WeakPtr<const Derived> WeakFromThis() const;
	};
}

#include "EnableSharedFromThis.inl"
