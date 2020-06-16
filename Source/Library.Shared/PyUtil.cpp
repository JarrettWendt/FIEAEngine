#include "pch.h"
#include "PyUtil.h"

namespace Library::py::Util
{
	PyObject* ToPyStr(const std::string& str) noexcept
	{
		return PyUnicode_FromStringAndSize(str.c_str(), str.size());
	}
	
	bool FromPyStr(PyObject* unicode, std::string& str) noexcept
	{
		if (!PyUnicode_Check(unicode))
		{
			PyErr_SetString(PyExc_TypeError, "expected unicode 'str'");
			return false;
		}
		
		str = PyUnicode_AsUTF8(unicode);
		return true;
	}
}
