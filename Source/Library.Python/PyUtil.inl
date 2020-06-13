#pragma once
#include "PyUtil.h"

namespace PyUtil
{
	template<typename T>
	T* Construct(PyTypeObject& type) noexcept
	{
		return reinterpret_cast<T*>(PyObject_CallObject(reinterpret_cast<PyObject*>(&type), nullptr));
	}
	
	template<typename T>
	T* Alloc(PyTypeObject& type) noexcept
	{
		return Alloc<T>(&type);
	}
	
	template<typename T>
	T* Alloc(PyTypeObject* type) noexcept
	{
		return reinterpret_cast<T*>(type->tp_alloc(type, 0));
	}
}
