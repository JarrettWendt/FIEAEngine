#pragma once

#include <Python.h>
#include <structmember.h>

#include "Entity.h"

struct PyEntity
{
	PyObject_HEAD

	std::shared_ptr<Library::Entity> e;

	static void dealloc(PyEntity* self);
	static PyEntity* _new(PyTypeObject* type, PyObject* args, PyObject* kwds);
	static int init(PyEntity* self, PyObject* args, PyObject* kwds);
	
	PyObject* richcompare(PyEntity* other, int op);
};

extern PyTypeObject PyEntityType;
PyMODINIT_FUNC PyInit_Entity();
