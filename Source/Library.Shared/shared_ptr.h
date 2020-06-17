// MIT License Copyright(c) 2020 Jarrett Wendt

#pragma once
#include "PythonPCH.h"

namespace Library::py
{
	/**
	 * A smart pointer for the already reference counted PyObject.
	 */
	struct shared_ptr final
	{
	private:
		PyObject* obj{ nullptr };

	public:
		shared_ptr() = default;
		shared_ptr(PyObject* obj) noexcept;
		shared_ptr(const shared_ptr& other) noexcept;
		shared_ptr(shared_ptr&& other) noexcept;
		shared_ptr& operator=(const shared_ptr& other) noexcept;
		shared_ptr& operator=(shared_ptr&& other) noexcept;
		~shared_ptr() noexcept;

		PyObject& operator*() noexcept;
		const PyObject& operator*() const noexcept;

		PyObject* operator->() noexcept;
		const PyObject* operator->() const noexcept;

		operator bool() const noexcept;

		friend bool operator==(const shared_ptr& a, const shared_ptr& b) noexcept;
		friend bool operator!=(const shared_ptr& a, const shared_ptr& b) noexcept;
	};
}
