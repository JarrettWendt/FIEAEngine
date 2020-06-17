#include "pch.h"
#include "PyException.h"

#include <Python.h>

#include "PyUtil.h"

namespace Library::py
{
	void Exception::HandleErrors()
	{
		PyObject* type, *value, *traceback;
		PyErr_Fetch(&type, &value, &traceback);
		if (type)
		{
			std::string str;
			py::Exception e{ "caught error, but failed to stringify it" };
			if (Util::FromPyStr(PyObject_Str(value), str))
			{
				e = py::Exception(str);
			}
			PyErr_Clear();
			throw e;
		}
	}
}
