// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Math_module.h"

#include "LibMath.h"

namespace Library::py::Math
{
	PyObject* IsPrime(PyObject*, PyObject* o)
	{
		return PyBool_FromLong(Library::Math::IsPrime(PyLong_AsSize_t(o)));
	}

	PyObject* NextPrime(PyObject*, PyObject* o)
	{
		return PyLong_FromSize_t(Library::Math::NextPrime(PyLong_AsSize_t(o)));
	}

	PyObject* ReMap(PyObject*, PyObject* args)
	{
		const auto arg0 = PyTuple_GetItem(args, 0);
		if (PyLong_Check(arg0))
		{
			int x, inMin, inMax, outMin, outMax;
			if (PyArg_ParseTuple(args, "iiiii", &x, &inMin, &inMax, &outMin, &outMax))
			{
				return PyLong_FromLong(Library::Math::ReMap(x, inMin, inMax, outMin, outMax));
			}
		}
		if (PyFloat_Check(arg0))
		{
			float x, inMin, inMax, outMin, outMax;
			if (PyArg_ParseTuple(args, "fffff", &x, &inMin, &inMax, &outMin, &outMax))
			{
				return PyFloat_FromDouble(Library::Math::ReMap(x, inMin, inMax, outMin, outMax));
			}
		}
		return nullptr;
	}

	PyObject* ReMap01(PyObject*, PyObject* args)
	{
		float x, inMin, inMax;
		if (PyArg_ParseTuple(args, "fff", &x, &inMin, &inMax))
		{
			return PyFloat_FromDouble(Library::Math::ReMap01(x, inMin, inMax));
		}
		return nullptr;
	}

	static PyMethodDef methods[] =
	{
		{ "IsPrime", PyCFunction(IsPrime), METH_O, nullptr },
		{ "NextPrime", PyCFunction(NextPrime), METH_O, nullptr },
		{ "ReMap", PyCFunction(ReMap), METH_VARARGS, nullptr },
		{ "ReMap01", PyCFunction(ReMap01), METH_VARARGS, nullptr },

		{ nullptr, nullptr, 0, nullptr }
	};

	static PyModuleDef module =
	{
		PyModuleDef_HEAD_INIT,

		"Math",
		"Python port of C++ Library::Math",
		0,
		methods
	};

	PyObject* InitModule()
	{
		return PyModule_Create(&module);
	}
}
