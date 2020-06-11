#pragma once

#include <Python.h>

#include "Entity.h"

struct PyEntity
{
	PyObject_HEAD

	Library::Entity e;
};

static inline PyTypeObject PyEntityType
{
	.ob_base = { PyObject_HEAD_INIT(nullptr) 0 },
	.tp_name = "Entity",
	.tp_basicsize = sizeof(PyEntity),
	.tp_itemsize = 0,
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_doc = "Python port of C++ Entity",
	.tp_new = PyType_GenericNew
};

//PyMODINIT_FUNC PyInit_Entity();
