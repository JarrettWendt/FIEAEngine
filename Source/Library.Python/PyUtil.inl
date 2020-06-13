#pragma once
#include "PyUtil.h"

namespace PyUtil
{
	template<typename Object, typename Type>
	Object* Construct(Type& type) noexcept
	{
		return reinterpret_cast<Object*>(PyObject_CallObject(reinterpret_cast<PyObject*>(&type), nullptr));
	}
	
	template<typename Object, typename Type>
	Object* Alloc(Type& type) noexcept
	{
		return Alloc<Object>(&type);
	}
	
	template<typename Object, typename Type>
	Object* Alloc(Type* type) noexcept
	{
		return reinterpret_cast<Object*>(type->tp_alloc(type, 0));
	}
}
