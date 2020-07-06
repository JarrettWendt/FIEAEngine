#pragma once
#include "python/Util.h"

namespace Library::py::Util
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

	template <typename T>
	T* Util::Alloc() noexcept
	{
		return Alloc<T>(T::type);
	}

	template<std::derived_from<PyObject> T>
	T* Util::New() noexcept
	{
		T* ret = PyObject_NEW(T, &T::type);
		PyObject_Init(ret, &T::type);
		Py_INCREF(ret);
		return ret;
	}
}
