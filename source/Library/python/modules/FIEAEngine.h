// MIT License Copyright(c) 2020 Jarrett Wendt

#pragma once
#include "python/pch.h"

static inline PyModuleDef FIEAEngine_module
{
	PyModuleDef_HEAD_INIT,

	// Module name to use with Python import statements
	// Must match Project Properties > Configuration Properties > General > Target Name
	"FIEAEngine",
	"Python port of C++ FIEAEngine",
	-1,
	nullptr
};

PyObject* PyInit_FIEAEngine() noexcept;
