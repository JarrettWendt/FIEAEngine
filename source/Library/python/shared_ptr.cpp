#include "pch.h"
#include "shared_ptr.h"

namespace Library::py
{
#pragma region special members
	shared_ptr::shared_ptr(PyObject* obj) noexcept :
		obj(obj)
	{
		Py_INCREF(obj);
	}
	
	shared_ptr::shared_ptr(const shared_ptr& other) noexcept :
		obj(other.obj)
	{
		Py_INCREF(obj);
	}
	
	shared_ptr::shared_ptr(shared_ptr&& other) noexcept :
		obj(other.obj)
	{
		other.obj = nullptr;
	}

	shared_ptr& shared_ptr::operator=(const shared_ptr& other) noexcept
	{
		if (this != &other)
		{
			obj = other.obj;
			Py_INCREF(obj);
		}
		return *this;
	}

	shared_ptr& shared_ptr::operator=(shared_ptr&& other) noexcept
	{
		if (this != &other)
		{
			obj = other.obj;
			other.obj = nullptr;
		}
		return *this;
	}

	shared_ptr::~shared_ptr() noexcept
	{
		if (obj)
		{
			Py_DECREF(obj);
			obj = nullptr;
		}
	}
#pragma endregion

#pragma region operators
	PyObject& shared_ptr::operator*() noexcept
	{
		return *obj;
	}
	
	const PyObject& shared_ptr::operator*() const noexcept
	{
		return const_cast<shared_ptr*>(this)->operator*();
	}
	
	PyObject* shared_ptr::operator->() noexcept
	{
		return &operator*();
	}

	const PyObject* shared_ptr::operator->() const noexcept
	{
		return const_cast<shared_ptr*>(this)->operator->();
	}
	
	shared_ptr::operator bool() const noexcept
	{
		return obj;
	}
	
	bool operator==(const shared_ptr& a, const shared_ptr& b) noexcept
	{
		return a.obj == b.obj;
	}

	bool operator!=(const shared_ptr& a, const shared_ptr& b) noexcept
	{
		return !operator==(a, b);
	}
#pragma endregion
}
