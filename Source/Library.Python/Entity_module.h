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

	PyObject* GetName(void* closure);
	int SetName(PyObject* value, void* closure);
	PyObject* GetEnabled(void* closure);
	int SetEnabled(PyObject* value, void* closure);
	PyObject* GetParent(void* closure);
	int SetParent(PyEntity* value, void* closure);

	PyObject* NumChildren();
	PyObject* HasChildren();
	PyObject* Child(PyObject* arg);
	PyObject* Adopt(PyObject* args, PyObject* kwds);
	PyObject* Orphan();
	PyObject* RemoveChild(PyObject* arg);
	PyObject* Init();
	PyObject* Update();
};

extern PyTypeObject PyEntityType;
PyMODINIT_FUNC PyInit_Entity();
