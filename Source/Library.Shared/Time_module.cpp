#include "pch.h"
#include "Time_module.h"
#include "EngineTime.h"

namespace Library::py::Time
{
	PyObject* Delta()
	{
		return PyFloat_FromDouble(Library::Time::DeltaTime().count());
	}
	
	PyObject* Total()
	{
		return PyFloat_FromDouble(Library::Time::TotalTime().count());
	}

	static PyMethodDef methods[] =
	{
		{ "Delta", PyCFunction(Delta), METH_NOARGS, nullptr },
		{ "Total", PyCFunction(Total), METH_NOARGS, nullptr },

		{ nullptr, nullptr, 0, nullptr }
	};

	static PyModuleDef module =
	{
		PyModuleDef_HEAD_INIT,

		"Time",
		"Python port of C++ Library::Time",
		0,
		methods
	};
	
	PyObject* InitModule()
	{
		return PyModule_Create(&module);
	}
}