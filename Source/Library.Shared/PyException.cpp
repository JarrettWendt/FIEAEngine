#include "pch.h"
#include "PyException.h"

#include <Python.h>

#include "PyUtil.h"

namespace Library::py
{
	void Exception::HandleErrors()
	{
		if (PyObject* error = PyErr_Occurred())
		{
			std::string str;
			py::Exception e{ "caught error, but failed to stringify it" };
			if (Util::FromPyStr(PyObject_Str(error), str))
			{
				e = py::Exception(str);
			}
			PyErr_Clear();
			throw e;
		}
	}
}
