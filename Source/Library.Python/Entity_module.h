#pragma once

#include <Python.h>

#include "Entity.h"

struct PyEntity
{
	PyObject_HEAD

	std::shared_ptr<Library::Entity> e;
};

PyMODINIT_FUNC PyInit_Entity();
