#include "pch.h"
#include "PyUtil.h"

namespace PyUtil
{
	PyObject* ToPyStr(const std::string& str) noexcept
	{
		return PyUnicode_FromStringAndSize(str.c_str(), str.size());
	}
}
