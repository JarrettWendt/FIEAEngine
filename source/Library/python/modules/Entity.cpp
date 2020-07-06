// MIT License Copyright(c) 2020 Jarrett Wendt

#include "python/pch.h"
#include "python/modules/Entity.h"
#include "python/Util.h"
#include "python/Exception.h"

#include <Entity.h>

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
		Exception::HandleErrors();
	}
#pragma endregion

#pragma region wrappers	
#pragma region vector
	PyFloatObject* EntityBinding::Vector3Wrapper::GetX()
	{
		const float f = Do([](auto& v)->float { return v.X(); });
		return reinterpret_cast<PyFloatObject*>(PyFloat_FromDouble(f));
	}
	
	PyFloatObject* EntityBinding::Vector3Wrapper::GetY()
	{
		const float f = Do([](auto& v)->float { return v.Y(); });
		return reinterpret_cast<PyFloatObject*>(PyFloat_FromDouble(f));
	}
	
	PyFloatObject* EntityBinding::Vector3Wrapper::GetZ()
	{
		const float f = Do([](auto& v)->float { return v.Z(); });
		return reinterpret_cast<PyFloatObject*>(PyFloat_FromDouble(f));
	}

	int EntityBinding::Vector3Wrapper::SetX(PyFloatObject* value)
	{
		Do([value](auto& v) { v.X() = PyFloat_AsDouble(reinterpret_cast<PyObject*>(value)); });
		return 0;
	}

	int EntityBinding::Vector3Wrapper::SetY(PyFloatObject* value)
	{
		Do([value](auto& v) { v.Y() = PyFloat_AsDouble(reinterpret_cast<PyObject*>(value)); });
		return 0;
	}

	int EntityBinding::Vector3Wrapper::SetZ(PyFloatObject* value)
	{
		Do([value](auto& v) { v.Z() = PyFloat_AsDouble(reinterpret_cast<PyObject*>(value)); });
		return 0;
	}
	
	PyTypeObject EntityBinding::Vector3Wrapper::type
	{
		.ob_base = { PyObject_HEAD_INIT(nullptr) 0 },

		.tp_name = "Vector3Wrapper",

		.tp_basicsize = sizeof(EntityBinding::Vector3Wrapper),
		.tp_itemsize = 0,

		.tp_flags = Py_TPFLAGS_DEFAULT,

		.tp_doc = "Python port of C++ Entity::Vector3Wrapper",
		
		.tp_getset = getset,

		.tp_new = PyType_GenericNew,
	};
#pragma endregion
	
#pragma region quaternion
	PyFloatObject* EntityBinding::QuaternionWrapper::GetX()
	{
		const float f = Do([](auto& v)->float { return v.X(); });
		return reinterpret_cast<PyFloatObject*>(PyFloat_FromDouble(f));
	}
	
	PyFloatObject* EntityBinding::QuaternionWrapper::GetY()
	{
		const float f = Do([](auto& v)->float { return v.Y(); });
		return reinterpret_cast<PyFloatObject*>(PyFloat_FromDouble(f));
	}
	
	PyFloatObject* EntityBinding::QuaternionWrapper::GetZ()
	{
		const float f = Do([](auto& v)->float { return v.Z(); });
		return reinterpret_cast<PyFloatObject*>(PyFloat_FromDouble(f));
	}
	
	PyFloatObject* EntityBinding::QuaternionWrapper::GetW()
	{
		const float f = Do([](auto& v)->float { return v.W(); });
		return reinterpret_cast<PyFloatObject*>(PyFloat_FromDouble(f));
	}

	int EntityBinding::QuaternionWrapper::SetX(PyFloatObject* value)
	{
		Do([value](auto& v) { v.X() = PyFloat_AsDouble(reinterpret_cast<PyObject*>(value)); });
		return 0;
	}

	int EntityBinding::QuaternionWrapper::SetY(PyFloatObject* value)
	{
		Do([value](auto& v) { v.Y() = PyFloat_AsDouble(reinterpret_cast<PyObject*>(value)); });
		return 0;
	}

	int EntityBinding::QuaternionWrapper::SetZ(PyFloatObject* value)
	{
		Do([value](auto& v) { v.Z() = PyFloat_AsDouble(reinterpret_cast<PyObject*>(value)); });
		return 0;
	}

	int EntityBinding::QuaternionWrapper::SetW(PyFloatObject* value)
	{
		Do([value](auto& v) { v.W() = PyFloat_AsDouble(reinterpret_cast<PyObject*>(value)); });
		return 0;
	}
	
	PyTypeObject EntityBinding::QuaternionWrapper::type
	{
		.ob_base = { PyObject_HEAD_INIT(nullptr) 0 },

		.tp_name = "QuaternionWrapper",

		.tp_basicsize = sizeof(EntityBinding::QuaternionWrapper),
		.tp_itemsize = 0,

		.tp_flags = Py_TPFLAGS_DEFAULT,

		.tp_doc = "Python port of C++ Entity::QuaternionWrapper",
		
		.tp_getset = getset,

		.tp_new = PyType_GenericNew,
	};
#pragma endregion
	
#pragma region transform
	EntityBinding::Vector3Wrapper* EntityBinding::TransformWrapper::GetTranslation()
	{
		Vector3Wrapper* ret = Util::New<Vector3Wrapper>();
		Do([ret](auto& t)
		{
			const auto v = t.Translation();
			std::memcpy(&ret->v, &v, sizeof(decltype(v)));
		});
		ret->space = space;
		ret->component = Transform::Component::Translation;
		return ret;
	}

	int EntityBinding::TransformWrapper::SetTranslation(Vector3Wrapper* value)
	{
		Do([value](auto& t)
		{
			t.Translation() = static_cast<Vector3>(value->v);
		});
		return 0;
	}

	EntityBinding::Vector3Wrapper* EntityBinding::TransformWrapper::GetScale()
	{
		Vector3Wrapper* ret = Util::New<Vector3Wrapper>();
		Do([ret](auto& t)
		{
			const auto v = t.Scale();
			std::memcpy(&ret->v, &v, sizeof(decltype(v)));
		});
		ret->space = space;
		ret->component = Transform::Component::Scale;
		return ret;
	}
	
	int EntityBinding::TransformWrapper::SetScale(Vector3Wrapper* value)
	{
		Do([value](auto& t)
		{
			t.Scale() = value->v.operator Vector3();
		});
		return 0;
	}

	EntityBinding::QuaternionWrapper* EntityBinding::TransformWrapper::GetRotation()
	{
		QuaternionWrapper* ret = Util::New<QuaternionWrapper>();
		Do([ret](auto& t)
		{
			const auto q = t.Rotation();
			std::memcpy(&ret->q, &q, sizeof(decltype(q)));
		});
		ret->space = space;
		return ret;
	}
	
	int EntityBinding::TransformWrapper::SetRotation(QuaternionWrapper* value)
	{
		Do([value](auto& t)
		{
			t.Rotation() = value->q.operator Quaternion();
		});
		return 0;
	}

	PyTypeObject EntityBinding::TransformWrapper::type
	{
		.ob_base = { PyObject_HEAD_INIT(nullptr) 0 },

		.tp_name = "TransformWrapper",

		.tp_basicsize = sizeof(EntityBinding::TransformWrapper),
		.tp_itemsize = 0,

		.tp_flags = Py_TPFLAGS_DEFAULT,

		.tp_doc = "Python port of C++ Entity::TransformWrapper",
		
		.tp_getset = getset,

		.tp_new = PyType_GenericNew,
	};
#pragma endregion
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
			e->pyObject = reinterpret_cast<PyObject*>(self);
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

	EntityBinding* EntityBinding::GetParent()
	{
		if (const auto parent = e->Parent())
		{
			return FromEntity(parent);
		}
		Py_INCREF(Py_None);
		return reinterpret_cast<EntityBinding*>(Py_None);
	}

	int EntityBinding::SetParent(EntityBinding* value)
	{
		value->e->Adopt(e);
		return 0;
	}
	
	EntityBinding::TransformWrapper* EntityBinding::GetLocalTransform()
	{
		return GetTransform<CoordinateSpace::Local>();
	}

	int EntityBinding::SetLocalTransform(TransformWrapper* value)
	{
		e->SetTransform<CoordinateSpace::Local>(value->t);
		return 0;
	}

	EntityBinding::TransformWrapper* EntityBinding::GetWorldTransform()
	{
		return GetTransform<CoordinateSpace::World>();
	}
	
	int EntityBinding::SetWorldTransform(TransformWrapper* value)
	{
		e->SetTransform<CoordinateSpace::World>(value->t);
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
				return reinterpret_cast<PyObject*>(ret);
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
			return reinterpret_cast<PyObject*>(child);
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
	
	EntityBinding* EntityBinding::FromEntity(std::shared_ptr<Library::Entity> entity) noexcept
	{
		EntityBinding* ret = Util::Alloc<EntityBinding>(type);
		ret->e = std::move(entity);
		Py_INCREF(ret);
		return ret;
	}

	PyTypeObject EntityBinding::type
	{
		.ob_base = { PyObject_HEAD_INIT(nullptr) 0 },

		.tp_name = "Entity",

		.tp_basicsize = sizeof(EntityBinding),
		.tp_itemsize = 0,

		.tp_dealloc = destructor(dealloc),

		.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,

		.tp_doc = "Python port of C++ Entity",

		.tp_richcompare = Library::Util::UnionCast<richcmpfunc>(&EntityBinding::richcompare),

		.tp_methods = methods,
		.tp_members = members,
		.tp_getset = getset,

		.tp_init = initproc(init),
		.tp_new = newfunc(_new),
	};

	PyObject* InitEntityModule()
	{
		// PyType_Ready returns -1 on error and 0 on success
		int failed = 0;
		failed += PyType_Ready(&EntityBinding::type);
		failed += PyType_Ready(&EntityBinding::TransformWrapper::type);
		failed += PyType_Ready(&EntityBinding::QuaternionWrapper::type);
		failed += PyType_Ready(&EntityBinding::Vector3Wrapper::type);
		
		if (failed) [[unlikely]]
		{
			return nullptr;
		}

		PyObject* m = PyModule_Create(&EntityBinding::module);
		if (!m) [[unlikely]]
		{
			return nullptr;
		}

		Py_INCREF(&EntityBinding::type);
		Py_INCREF(&EntityBinding::TransformWrapper::type);
		Py_INCREF(&EntityBinding::Vector3Wrapper::type);
		Py_INCREF(&EntityBinding::QuaternionWrapper::type);

		// PyModule_AddObject returns -1 on error and 0 on success
		failed = 0;
		failed += PyModule_AddObject(m, "Entity", reinterpret_cast<PyObject*>(&EntityBinding::type));
		failed += PyModule_AddObject(m, "TransformWrapper", reinterpret_cast<PyObject*>(&EntityBinding::TransformWrapper::type));
		failed += PyModule_AddObject(m, "Vector3Wrapper", reinterpret_cast<PyObject*>(&EntityBinding::Vector3Wrapper::type));
		failed += PyModule_AddObject(m, "QuaternionWrapper", reinterpret_cast<PyObject*>(&EntityBinding::QuaternionWrapper::type));

		if (failed) [[unlikely]]
		{
			Py_DECREF(&EntityBinding::QuaternionWrapper::type);
			Py_DECREF(&EntityBinding::Vector3Wrapper::type);
			Py_DECREF(&EntityBinding::TransformWrapper::type);
			Py_DECREF(&EntityBinding::type);
			Py_DECREF(m);
			return nullptr;
		}
		
		return m;
	}
}
