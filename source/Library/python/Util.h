// MIT License Copyright(c) 2020 Jarrett Wendt

#pragma once
#include "python/pch.h"
#include <string>

namespace Library::py::Util
{
	/**
	 * @param str	string to convert to a python string
	 * @returns python unicode string object
	 */
	PyObject* ToPyStr(const std::string& str) noexcept;

	/**
	 * @param unicode	python unicode string object
	 * @param str		string to be populated from unicode
	 * @returns			whether or not the conversion was successful
	 */
	bool FromPyStr(PyObject* unicode, std::string& str) noexcept;

	/**
	 * @param <T>		return type
	 * @param type		type object for this python type
	 * @returns			newly constructed T using type
	 */
	template<typename T>
	T* Construct(PyTypeObject& type) noexcept;

	/**
	 * @param <T>		return type
	 * @param type		type object for this python type
	 * @returns			newly allocated T using type
	 */
	template<typename T>
	T* Alloc(PyTypeObject& type) noexcept;

	/**
	 * @param <T>		return type
	 * @param type		type object for this python type
	 * @returns			newly allocated T using type
	 */
	template<typename T>
	T* Alloc(PyTypeObject* type) noexcept;

	/**
	 * @param <T>		PyObject with T::type
	 * @returns			newly allocated T using type
	 */
	template<typename T>
	T* Alloc() noexcept
	{
		return Alloc<T>(T::type);
	}

	template<std::derived_from<PyObject> T>
	T* New() noexcept
	{
		T* ret = PyObject_NEW(T, &T::type);
		PyObject_Init(ret, &T::type);
		Py_INCREF(ret);
		return ret;
	}

	std::string ToString(PyObject* obj) noexcept;

	/**
	 * @param uni		A PyObject that we already know succeeds PyUnicode_Check
	 *
	 * @asserts	PyUnicode_Check
	 */
	std::string ToString(PyUnicodeObject* uni) noexcept;
}

#define Py_RETURN_BOOL(b)	\
	if (b)					\
	{						\
		Py_RETURN_TRUE;		\
	}						\
	else					\
	{						\
		Py_RETURN_FALSE;	\
	}

#include "python/Util.inl"
