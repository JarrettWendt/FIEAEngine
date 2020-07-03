#include "pch.h"
#include "python/Util.h"

namespace Library::py::Util
{
	PyObject* ToPyStr(const std::string& str) noexcept
	{
		return PyUnicode_FromStringAndSize(str.c_str(), str.size());
	}
	
	bool FromPyStr(PyObject* unicode, std::string& str) noexcept
	{
		if (!unicode || !PyUnicode_Check(unicode))
		{
			PyErr_SetString(PyExc_TypeError, "expected unicode 'str'");
			return false;
		}
		
		str = PyUnicode_AsUTF8(unicode);
		return true;
	}

	std::string ToString(PyUnicodeObject* uni) noexcept
	{
		if (uni)
		{
			assert(PyUnicode_Check(uni));
			if (const auto str = PyUnicode_AsUTF8(reinterpret_cast<PyObject*>(uni)))
			{
				return str;
			}
			return "unicode to c-string conversion failed";
		}
		return "nullptr";
	}
	
	std::string ToString(PyObject* obj) noexcept
	{
		if (obj)
		{
			if (PyUnicode_Check(obj))
			{
				return ToString(reinterpret_cast<PyUnicodeObject*>(obj));
			}
			if (const auto uni = PyObject_Str(obj))
			{
				return ToString(reinterpret_cast<PyUnicodeObject*>(uni));
			}
			return "could not convert PyObject to string";
		}
		return "nullptr";
	}
}
