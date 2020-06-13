#include "pch.h"
#include "Math_module.h"

#include "LibMath.h"

using namespace Library;

PyObject* IsPrime(PyObject*, PyObject* o)
{
	return PyBool_FromLong(Math::IsPrime(PyLong_AsSize_t(o)));
}

PyObject* NextPrime(PyObject*, PyObject* o)
{
	return PyLong_FromSize_t(Math::NextPrime(PyLong_AsSize_t(o)));
}

PyObject* ReMap(PyObject*, PyObject* args)
{
	const auto arg0 = PyTuple_GetItem(args, 0);
	if (PyLong_Check(arg0))
	{		
		int x, inMin, inMax, outMin, outMax;
		if (PyArg_ParseTuple(args, "iiiii", &x, &inMin, &inMax, &outMin, &outMax))
		{
			return PyLong_FromLong(Math::ReMap(x, inMin, inMax, outMin, outMax));
		}
	}
	if (PyFloat_Check(arg0))
	{
		float x, inMin, inMax, outMin, outMax;
		if (PyArg_ParseTuple(args, "fffff", &x, &inMin, &inMax, &outMin, &outMax))
		{
			return PyFloat_FromDouble(Math::ReMap(x, inMin, inMax, outMin, outMax));
		}
	}
	return nullptr;
}

PyObject* ReMap01(PyObject*, PyObject* args)
{
	float x, inMin, inMax;
	if (PyArg_ParseTuple(args, "fff", &x, &inMin, &inMax)) [[likely]]
	{
		return PyFloat_FromDouble(Math::ReMap01(x, inMin, inMax));
	}
	return nullptr;
}

static PyMethodDef Math_methods[] =
{
	{ "IsPrime", PyCFunction(IsPrime), METH_O, nullptr },
	{ "NextPrime", PyCFunction(NextPrime), METH_O, nullptr },
	{ "ReMap", PyCFunction(ReMap), METH_VARARGS, nullptr },
	{ "ReMap01", PyCFunction(ReMap01), METH_VARARGS, nullptr },

	{ nullptr, nullptr, 0, nullptr }
};

static PyModuleDef Math_module =
{
	PyModuleDef_HEAD_INIT,

	"Math",	
	"Python port of C++ Library::Math",
	0,
	Math_methods
};

PyMODINIT_FUNC PyInit_Math()
{
	return PyModule_Create(&Math_module);
}
