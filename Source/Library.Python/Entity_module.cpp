#include "pch.h"
#include "Entity_module.h"
#include "Entity.h"
#include "PyUtil.h"

namespace Library::py
{
#pragma region special members
	void Entity::dealloc(Entity* self)
	{
		self->e = nullptr;
		Py_TYPE(self)->tp_free(reinterpret_cast<PyObject*>(self));
	}

	Entity* Entity::_new(PyTypeObject* t, [[maybe_unused]] PyObject* args, [[maybe_unused]] PyObject* kwds)
	{
		if (Entity* self = PyUtil::Alloc<Entity>(t))
		{
			self->e = std::make_shared<Library::Entity>();
			return self;
		}
		return nullptr;
	}

	int Entity::init(Entity* self, PyObject* args, PyObject* kwds)
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

	PyObject* Entity::richcompare(Entity* other, const int op)
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
	PyObject* Entity::GetName([[maybe_unused]] void* closure)
	{
		return PyUtil::ToPyStr(e->GetName());
	}

	int Entity::SetName(PyObject* value, [[maybe_unused]] void* closure)
	{
		std::string name;
		if (!PyUtil::FromPyStr(value, name))
		{
			return -1;
		}
		e->SetName(name);
		return 0;
	}

	PyObject* Entity::GetEnabled([[maybe_unused]] void* closure)
	{
		return PyBool_FromLong(e->Enabled());
	}

	int Entity::SetEnabled(PyObject* value, [[maybe_unused]] void* closure)
	{
		e->Enabled() = PyObject_IsTrue(value);
		return 0;
	}

	PyObject* Entity::GetParent([[maybe_unused]] void* closure)
	{
		if (const auto parent = e->Parent())
		{
			Entity* ret = PyUtil::Alloc<Entity>(type);
			ret->e = parent;
			Py_INCREF(ret);
			return reinterpret_cast<PyObject*>(ret);
		}
		Py_RETURN_NONE;
	}

	int Entity::SetParent(Entity* value, [[maybe_unused]] void* closure)
	{
		value->e->Adopt(e);
		return 0;
	}
#pragma endregion

#pragma region methods
	PyObject* Entity::NumChildren()
	{
		return PyLong_FromSize_t(e->NumChildren());
	}

	PyObject* Entity::HasChildren()
	{
		return PyBool_FromLong(e->HasChildren());
	}

	PyObject* Entity::Child(PyObject* arg)
	{
		std::string childName;
		if (PyUtil::FromPyStr(arg, childName))
		{
			if (const auto child = e->Child(childName))
			{
				Entity* ret = PyUtil::Alloc<Entity>(type);
				ret->e = e->Child(childName);
				return reinterpret_cast<PyObject*>(ret);
			}
		}
		Py_RETURN_NONE;
	}

	PyObject* Entity::Adopt(PyObject* args, PyObject* kwds)
	{
		static const char* kwlist[] = { "name", "child", nullptr };
		char* name{ nullptr };
		Entity* child{ nullptr };

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

	PyObject* Entity::Orphan()
	{
		e->Orphan();
		Py_RETURN_NONE;
	}

	PyObject* Entity::RemoveChild(PyObject* arg)
	{
		std::string childName;
		if (PyUtil::FromPyStr(arg, childName))
		{
			e->RemoveChild(childName);
		}
		Py_RETURN_NONE;
	}

	PyObject* Entity::Init()
	{
		e->Init();
		Py_RETURN_NONE;
	}

	PyObject* Entity::Update()
	{
		e->Update();
		Py_RETURN_NONE;
	}
#pragma endregion

	PyTypeObject Entity::type
	{
		.ob_base = { PyObject_HEAD_INIT(nullptr) 0 },

		.tp_name = "Entity",

		.tp_basicsize = sizeof(Entity),
		.tp_itemsize = 0,

		.tp_dealloc = destructor(dealloc),

		.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,

		.tp_doc = "Python port of C++ Entity",

		.tp_richcompare = Library::Util::ForceCast<richcmpfunc>(&richcompare),

		.tp_methods = methods,
		.tp_members = members,
		.tp_getset = getset,

		.tp_init = initproc(init),
		.tp_new = newfunc(_new),
	};
}

PyMODINIT_FUNC PyInit_Entity()
{
	using namespace Library::py;
	
	if (PyType_Ready(&Entity::type) < 0) [[unlikely]]
	{
		return nullptr;
	}

	PyObject* m = PyModule_Create(&Entity::module);
	if (!m) [[unlikely]]
	{
		return nullptr;
	}

	Py_INCREF(&Entity::type);
	if (PyModule_AddObject(m, "Entity", reinterpret_cast<PyObject*>(&Entity::type)) < 0) [[unlikely]]
	{
		Py_DECREF(&Entity::type);
		Py_DECREF(m);
		return nullptr;
	}
	
	return m;
}
