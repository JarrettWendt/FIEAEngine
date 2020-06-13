#pragma once

#include <Python.h>
#include <structmember.h>

#include "Entity.h"

struct PyEntity
{
	PyObject_HEAD

	std::shared_ptr<Library::Entity> e;
};

extern PyTypeObject PyEntityType;

PyMODINIT_FUNC PyInit_Entity();
