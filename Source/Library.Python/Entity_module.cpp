#include "pch.h"
#include "Entity_module.h"
#include "Entity.h"
#include "PyUtil.h"

#pragma region special members
void PyEntity_dealloc(PyEntity* self)
{
	self->e = nullptr;
	Py_TYPE(self)->tp_free(reinterpret_cast<PyObject*>(self));
}

PyObject* PyEntity_new(PyTypeObject* type, [[maybe_unused]] PyObject* args, [[maybe_unused]] PyObject* kwds)
{
	if (PyEntity* self = PyUtil::Alloc<PyEntity>(type))
	{
		self->e = std::make_shared<Library::Entity>();
		return reinterpret_cast<PyObject*>(self);
	}
	return nullptr;
}

int PyEntity_init(PyEntity* self, PyObject* args, PyObject* kwds)
{
	static const char* kwlist[] = { "name", nullptr };
	char* name{ nullptr };

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|z", const_cast<char**>(kwlist), &name)) [[unlikely]]
	{
		return -1;
	}

	if (name) [[unlikely]]
	{
		self->e->SetName(name);
	}
	
	return 0;
}
#pragma endregion

#pragma region getters/setters
PyObject* PyEntity_GetName(PyEntity* self, [[maybe_unused]] void* closure)
{
	return PyUtil::ToPyStr(self->e->GetName());
}

int PyEntity_SetName(PyEntity* self, PyObject* value, [[maybe_unused]] void* closure)
{
	std::string name;
	if (!PyUtil::FromPyStr(value, name))
	{
		return -1;
	}
	self->e->SetName(name);
	return 0;
}

PyObject* PyEntity_GetEnabled(PyEntity* self, [[maybe_unused]] void* closure)
{
	return PyBool_FromLong(self->e->Enabled());
}

int PyEntity_SetEnabled(PyEntity* self, PyObject* value, [[maybe_unused]] void* closure)
{
	self->e->Enabled() = PyObject_IsTrue(value);
	return 0;
}
#pragma endregion

#pragma region methods
PyObject* PyEntity_NumChildren(PyEntity* self, PyObject*)
{
	return PyLong_FromSize_t(self->e->NumChildren());
}

PyObject* PyEntity_HasChildren(PyEntity* self, PyObject*)
{
	return PyBool_FromLong(self->e->HasChildren());
}

PyObject* PyEntity_Child(PyEntity* self, PyObject* arg)
{
	std::string childName;
	if (PyUtil::FromPyStr(arg, childName))
	{
		if (const auto child = self->e->Child(childName))
		{
			PyEntity* ret = PyUtil::Alloc<PyEntity>(PyEntityType);
			ret->e = self->e->Child(childName);
			return reinterpret_cast<PyObject*>(ret);
		}
	}
	Py_RETURN_NONE;
}
#pragma endregion

#pragma region structs
static inline PyMethodDef PyEntity_methods[]
{
	{ "NumChildren", PyCFunction(PyEntity_NumChildren), METH_NOARGS, "how many children this Entity has" },
	{ "HasChildren", PyCFunction(PyEntity_HasChildren), METH_NOARGS, "how many children this Entity has" },
	{ "Child", PyCFunction(PyEntity_Child), METH_O, "get child by name" },
	
	{ nullptr }
};

static PyGetSetDef PyEntity_getset[]
{
	{ "name", getter(PyEntity_GetName), setter(PyEntity_SetName), "name of this Entity", nullptr },
	{ "enabled", getter(PyEntity_GetEnabled), setter(PyEntity_SetEnabled), "whether or not this Entity is enabled", nullptr },
	
	{ nullptr }
};

static PyMemberDef PyEntity_members[]
{
	{ nullptr }
};

PyTypeObject PyEntityType
{
	.ob_base = { PyObject_HEAD_INIT(nullptr) 0 },

	.tp_name = "Entity",

	.tp_basicsize = sizeof(PyEntity),
	.tp_itemsize = 0,

	.tp_dealloc = destructor(PyEntity_dealloc),

	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,

	.tp_doc = "Python port of C++ Entity",

	.tp_methods = PyEntity_methods,
	.tp_members = PyEntity_members,
	.tp_getset = PyEntity_getset,

	.tp_init = initproc(PyEntity_init),
	.tp_new = PyEntity_new,
};

static inline PyModuleDef PyEntity_module
{
	PyModuleDef_HEAD_INIT,
	"Entity",
	"Entity module",
	-1,
	nullptr
};
#pragma endregion

PyMODINIT_FUNC PyInit_Entity()
{
	if (PyType_Ready(&PyEntityType) < 0) [[unlikely]]
	{
		return nullptr;
	}

	PyObject* m = PyModule_Create(&PyEntity_module);
	if (!m) [[unlikely]]
	{
		return nullptr;
	}

	Py_INCREF(&PyEntityType);
	if (PyModule_AddObject(m, "Entity", reinterpret_cast<PyObject*>(&PyEntityType)) < 0) [[unlikely]]
	{
		Py_DECREF(&PyEntityType);
		Py_DECREF(m);
		return nullptr;
	}
	
	return m;
}
