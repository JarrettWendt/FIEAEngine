#include "pch.h"
#include "Entity_module.h"
#include "Entity.h"
#include "PyUtil.h"

#pragma region special members
void PyEntity::dealloc(PyEntity* self)
{
	self->e = nullptr;
	Py_TYPE(self)->tp_free(reinterpret_cast<PyObject*>(self));
}

PyEntity* PyEntity::_new(PyTypeObject* type, [[maybe_unused]] PyObject* args, [[maybe_unused]] PyObject* kwds)
{
	if (PyEntity* self = PyUtil::Alloc<PyEntity>(type))
	{
		self->e = std::make_shared<Library::Entity>();
		return self;
	}
	return nullptr;
}

int PyEntity::init(PyEntity* self, PyObject* args, PyObject* kwds)
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

PyObject* PyEntity::richcompare(PyEntity* other, const int op)
{
	bool b;
	switch (op)
	{
	case Py_EQ: b = e == other->e || e && *e == *other->e; break;
	case Py_NE: b = e != other->e || e && *e != *other->e; break;
	default: Py_RETURN_NONE;
	}
	Py_RETURN_BOOL(b);
}
#pragma endregion

#pragma region getters/setters
PyObject* PyEntity::GetName([[maybe_unused]] void* closure)
{
	return PyUtil::ToPyStr(e->GetName());
}

int PyEntity::SetName(PyObject* value, [[maybe_unused]] void* closure)
{
	std::string name;
	if (!PyUtil::FromPyStr(value, name))
	{
		return -1;
	}
	e->SetName(name);
	return 0;
}

PyObject* PyEntity::GetEnabled([[maybe_unused]] void* closure)
{
	return PyBool_FromLong(e->Enabled());
}

int PyEntity::SetEnabled(PyObject* value, [[maybe_unused]] void* closure)
{
	e->Enabled() = PyObject_IsTrue(value);
	return 0;
}

PyObject* PyEntity::GetParent([[maybe_unused]] void* closure)
{
	if (const auto parent = e->Parent())
	{
		PyEntity* ret = PyUtil::Alloc<PyEntity>(PyEntityType);
		ret->e = parent;
		Py_INCREF(ret);
		return reinterpret_cast<PyObject*>(ret);
	}
	Py_RETURN_NONE;
}

int PyEntity::SetParent(PyEntity* value, [[maybe_unused]] void* closure)
{
	value->e->Adopt(e);
	return 0;
}
#pragma endregion

#pragma region methods
PyObject* PyEntity::NumChildren()
{
	return PyLong_FromSize_t(e->NumChildren());
}

PyObject* PyEntity::HasChildren()
{
	return PyBool_FromLong(e->HasChildren());
}

PyObject* PyEntity::Child(PyObject* arg)
{
	std::string childName;
	if (PyUtil::FromPyStr(arg, childName))
	{
		if (const auto child = e->Child(childName))
		{
			PyEntity* ret = PyUtil::Alloc<PyEntity>(PyEntityType);
			ret->e = e->Child(childName);
			return reinterpret_cast<PyObject*>(ret);
		}
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity::Adopt(PyObject* args, PyObject* kwds)
{
	static const char* kwlist[] = { "name", "child", nullptr };
	char* name{ nullptr };
	PyEntity* child{ nullptr };
	
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "zO", const_cast<char**>(kwlist), &name, &child))
	{
		if (PyArg_ParseTuple(args, "O", &child))
		{
			PyErr_Clear();
		}
	}
	
	if (child)
	{
		child->e = name ? e->Adopt(name, child->e) : e->Adopt(child->e);
		Py_INCREF(child);
		return reinterpret_cast<PyObject*>(child);
	}
	
	Py_RETURN_NONE;
}

PyObject* PyEntity::Orphan()
{
	e->Orphan();
	Py_RETURN_NONE;
}

PyObject* PyEntity::RemoveChild(PyObject* arg)
{
	std::string childName;
	if (PyUtil::FromPyStr(arg, childName))
	{
		e->RemoveChild(childName);
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity::Init()
{
	e->Init();
	Py_RETURN_NONE;
}

PyObject* PyEntity::Update()
{
	e->Update();
	Py_RETURN_NONE;
}
#pragma endregion

#pragma region structs
static inline PyMethodDef PyEntity_methods[]
{
	{ "NumChildren", Library::Util::ForceCast<PyCFunction>(&PyEntity::NumChildren), METH_NOARGS, "how many children this Entity has" },
	{ "HasChildren", Library::Util::ForceCast<PyCFunction>(&PyEntity::HasChildren), METH_NOARGS, "how many children this Entity has" },
	{ "Child", Library::Util::ForceCast<PyCFunction>(&PyEntity::Child), METH_O, "get child by name" },

	{ "Adopt", Library::Util::ForceCast<PyCFunction>(&PyEntity::Adopt), METH_VARARGS | METH_KEYWORDS, "make the passed Entity a child of this one" },

	{ "Orphan", Library::Util::ForceCast<PyCFunction>(&PyEntity::Orphan), METH_NOARGS, "orphans this Entity from its parent" },
	{ "RemoveChild", Library::Util::ForceCast<PyCFunction>(&PyEntity::RemoveChild), METH_O, "removes child by name" },

	{ "_Init", Library::Util::ForceCast<PyCFunction>(&PyEntity::Init), METH_NOARGS, "initialization ran after construction before the first Update" },
	{ "_Update", Library::Util::ForceCast<PyCFunction>(&PyEntity::Update), METH_NOARGS, "initialization ran after construction before the first Update" },
	
	{ nullptr }
};

static PyGetSetDef PyEntity_getset[]
{
	{ "name", Library::Util::ForceCast<getter>(&PyEntity::GetName), Library::Util::ForceCast<setter>(&PyEntity::SetName), "name of this Entity", nullptr },
	{ "enabled", Library::Util::ForceCast<getter>(&PyEntity::GetEnabled), Library::Util::ForceCast<setter>(&PyEntity::SetEnabled), "whether or not this Entity is enabled", nullptr },
	{ "parent", Library::Util::ForceCast<getter>(&PyEntity::GetParent), Library::Util::ForceCast<setter>(&PyEntity::SetParent), "this Entity's parent", nullptr },
	
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

	.tp_dealloc = destructor(PyEntity::dealloc),

	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,

	.tp_doc = "Python port of C++ Entity",

	.tp_richcompare = Library::Util::ForceCast<richcmpfunc>(&PyEntity::richcompare),

	.tp_methods = PyEntity_methods,
	.tp_members = PyEntity_members,
	.tp_getset = PyEntity_getset,

	.tp_init = initproc(PyEntity::init),
	.tp_new = newfunc(PyEntity::_new),
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
