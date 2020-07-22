#pragma once
#include "Manager.h"

namespace Library::Memory
{
	template<typename T>
	inline typename SmartPtr<T>::Handle& Manager::Alloc(const size_t count) noexcept
	{
		return reinterpret_cast<typename SmartPtr<T>::Handle&>(Alloc(count * sizeof(T), alignof(T)));
	}
	
	template<typename T, typename ...Args>
	inline typename SmartPtr<T>::Handle& Manager::Emplace(Args ...args)
	{
		auto& ret = Alloc<T>();
		new (ret.ptr) T(std::forward<Args>(args)...);
		return ret;
	}
}
