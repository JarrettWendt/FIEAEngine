#pragma once

#include <Python.h>

namespace PyUtil
{
	PyObject* ToPyStr(const std::string& str) noexcept;

	bool FromPyStr(PyObject* unicode, std::string& str) noexcept;

	template<typename Object, typename Type>
	Object* Construct(Type& type) noexcept;

	template<typename Object, typename Type>
	Object* Alloc(Type& type) noexcept;

	template<typename Object, typename Type>
	Object* Alloc(Type* type) noexcept;
}

#include "PyUtil.inl"
