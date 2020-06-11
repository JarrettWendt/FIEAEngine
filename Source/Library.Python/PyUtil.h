#pragma once

#include <Python.h>

namespace PyUtil
{
	PyObject* ToPyStr(const std::string& str) noexcept;
}
