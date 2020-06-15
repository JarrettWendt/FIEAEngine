// MIT License Copyright(c) 2020 Jarrett Wendt

#include "pch.h"
#include "Entity_module.h"
#include "Entity.h"
#include "PyUtil.h"

namespace Library::py
{
#pragma region py::Entity
	void Entity::Init()
	{
		// Don't invoke Base::Init, that's handled in EntityBinding::Init
		PyObject* method = PyObject_GetAttrString(&*pyObject, "_Init");
		PyObject_CallObject(method, nullptr);
	}
	
	void Entity::Update()
	{
		// Don't invoke Base::Update, that's handled in EntityBinding::Update
		PyObject* method = PyObject_GetAttrString(&*pyObject, "_Update");
		PyObject_CallObject(method, nullptr);
	}
#pragma endregion
	
#pragma region special members
	void EntityBinding::dealloc(EntityBinding* self)
	{
		self->e = nullptr;
		Py_TYPE(self)->tp_free(reinterpret_cast<PyObject*>(self));
	}

	EntityBinding* EntityBinding::_new(PyTypeObject* t, [[maybe_unused]] PyObject* args, [[maybe_unused]] PyObject* kwds)
	{
		if (EntityBinding* self = Util::Alloc<EntityBinding>(t))
		{
			auto e = std::make_shared<Entity>();
			e->pyObject = self;
			self->e = e;
			return self;
		}
		return nullptr;
	}

	int EntityBinding::init(EntityBinding* self, PyObject* args, PyObject* kwds)
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

	PyObject* EntityBinding::richcompare(EntityBinding* other, const int op)
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
	PyObject* EntityBinding::GetName()
	{
		return Util::ToPyStr(e->GetName());
	}

	int EntityBinding::SetName(PyObject* value)
	{
		std::string name;
		if (!Util::FromPyStr(value, name))
		{
			return -1;
		}
		e->SetName(name);
		return 0;
	}

	PyObject* EntityBinding::GetEnabled()
	{
		return PyBool_FromLong(e->Enabled());
	}

	int EntityBinding::SetEnabled(PyObject* value)
	{
		e->Enabled() = PyObject_IsTrue(value);
		return 0;
	}

	PyObject* EntityBinding::GetParent()
	{
		if (const auto parent = e->Parent())
		{
			EntityBinding* ret = Util::Alloc<EntityBinding>(type);
			ret->e = parent;
			Py_INCREF(ret);
			return reinterpret_cast<PyObject*>(ret);
		}
		Py_RETURN_NONE;
	}

	int EntityBinding::SetParent(EntityBinding* value)
	{
		value->e->Adopt(e);
		return 0;
	}
#pragma endregion
	
#pragma region methods
	PyObject* EntityBinding::NumChildren()
	{
		return PyLong_FromSize_t(e->NumChildren());
	}

	PyObject* EntityBinding::HasChildren()
	{
		return PyBool_FromLong(e->HasChildren());
	}

	PyObject* EntityBinding::Child(PyObject* arg)
	{
		std::string childName;
		if (Util::FromPyStr(arg, childName))
		{
			if (const auto child = e->Child(childName))
			{
				EntityBinding* ret = Util::Alloc<EntityBinding>(type);
				ret->e = e->Child(childName);
				return ret;
			}
		}
		Py_RETURN_NONE;
	}

	PyObject* EntityBinding::Adopt(PyObject* args, PyObject* kwds)
	{
		static const char* kwlist[] = { "name", "child", nullptr };
		char* name{ nullptr };
		EntityBinding* child{ nullptr };

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
			return child;
		}

		Py_RETURN_NONE;
	}

	PyObject* EntityBinding::Orphan()
	{
		e->Orphan();
		Py_RETURN_NONE;
	}

	PyObject* EntityBinding::RemoveChild(PyObject* arg)
	{
		std::string childName;
		if (Util::FromPyStr(arg, childName))
		{
			e->RemoveChild(childName);
		}
		Py_RETURN_NONE;
	}

	PyObject* EntityBinding::Init()
	{
		// We never want to invoke py::Entity::Init from here because that
		// invokes the python _Init which will cause infinite recursion.
		if (e->Is<py::Entity>())
		{
			e->Library::Entity::Init();
		}
		else
		{
			e->Init();
		}
		Py_RETURN_NONE;
	}

	PyObject* EntityBinding::Update()
	{
		// We never want to invoke py::Entity::Update from here because that
		// invokes the python _Update which will cause infinite recursion.
		if (e->Is<py::Entity>())
		{
			e->Library::Entity::Update();
		}
		else
		{
			e->Update();
		}
		Py_RETURN_NONE;
	}
#pragma endregion

	PyTypeObject EntityBinding::type
	{
		.ob_base = { PyObject_HEAD_INIT(nullptr) 0 },

		.tp_name = "Entity",

		.tp_basicsize = sizeof(EntityBinding),
		.tp_itemsize = 0,

		.tp_dealloc = destructor(dealloc),

		.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,

		.tp_doc = "Python port of C++ Entity",

		.tp_richcompare = Library::Util::UnionCast<richcmpfunc>(&richcompare),

		.tp_methods = methods,
		.tp_members = members,
		.tp_getset = getset,

		.tp_init = initproc(init),
		.tp_new = newfunc(_new),
	};

	PyMODINIT_FUNC Init_Entity()
	{		
		if (PyType_Ready(&EntityBinding::type) < 0) [[unlikely]]
		{
			return nullptr;
		}

		PyObject* m = PyModule_Create(&EntityBinding::module);
		if (!m) [[unlikely]]
		{
			return nullptr;
		}

		Py_INCREF(&EntityBinding::type);
		if (PyModule_AddObject(m, "Entity", reinterpret_cast<PyObject*>(&EntityBinding::type)) < 0) [[unlikely]]
		{
			Py_DECREF(&EntityBinding::type);
			Py_DECREF(m);
			return nullptr;
		}
		
		return m;
	}
}
