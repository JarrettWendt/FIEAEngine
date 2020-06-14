#pragma once

#include <Python.h>
#include <structmember.h>

#include "Entity.h"

namespace Library::py
{
	struct Entity
	{
		PyObject_HEAD

		std::shared_ptr<Library::Entity> e;

		static void dealloc(Entity* self);
		static Entity* _new(PyTypeObject* t, PyObject* args, PyObject* kwds);
		static int init(Entity* self, PyObject* args, PyObject* kwds);

		PyObject* richcompare(Entity* other, int op);

		PyObject* GetName(void* closure);
		int SetName(PyObject* value, void* closure);
		PyObject* GetEnabled(void* closure);
		int SetEnabled(PyObject* value, void* closure);
		PyObject* GetParent(void* closure);
		int SetParent(Entity* value, void* closure);

		PyObject* NumChildren();
		PyObject* HasChildren();
		PyObject* Child(PyObject* arg);
		PyObject* Adopt(PyObject* args, PyObject* kwds);
		PyObject* Orphan();
		PyObject* RemoveChild(PyObject* arg);
		PyObject* Init();
		PyObject* Update();

		static inline PyMethodDef methods[]
		{
			{ "NumChildren", Library::Util::ForceCast<PyCFunction>(&Entity::NumChildren), METH_NOARGS, "how many children this Entity has" },
			{ "HasChildren", Library::Util::ForceCast<PyCFunction>(&Entity::HasChildren), METH_NOARGS, "how many children this Entity has" },
			{ "Child", Library::Util::ForceCast<PyCFunction>(&Entity::Child), METH_O, "get child by name" },

			{ "Adopt", Library::Util::ForceCast<PyCFunction>(&Entity::Adopt), METH_VARARGS | METH_KEYWORDS, "make the passed Entity a child of this one" },

			{ "Orphan", Library::Util::ForceCast<PyCFunction>(&Entity::Orphan), METH_NOARGS, "orphans this Entity from its parent" },
			{ "RemoveChild", Library::Util::ForceCast<PyCFunction>(&Entity::RemoveChild), METH_O, "removes child by name" },

			{ "_Init", Library::Util::ForceCast<PyCFunction>(&Entity::Init), METH_NOARGS, "initialization ran after construction before the first Update" },
			{ "_Update", Library::Util::ForceCast<PyCFunction>(&Entity::Update), METH_NOARGS, "initialization ran after construction before the first Update" },

			{ nullptr }
		};

		static inline PyGetSetDef getset[]
		{
			{ "name", Library::Util::ForceCast<getter>(&Entity::GetName), Library::Util::ForceCast<setter>(&Entity::SetName), "name of this Entity", nullptr },
			{ "enabled", Library::Util::ForceCast<getter>(&Entity::GetEnabled), Library::Util::ForceCast<setter>(&Entity::SetEnabled), "whether or not this Entity is enabled", nullptr },
			{ "parent", Library::Util::ForceCast<getter>(&Entity::GetParent), Library::Util::ForceCast<setter>(&Entity::SetParent), "this Entity's parent", nullptr },

			{ nullptr }
		};

		static inline PyMemberDef members[]
		{
			{ nullptr }
		};

		static PyTypeObject type;

		static inline PyModuleDef module
		{
			PyModuleDef_HEAD_INIT,
			"Entity",
			"Entity module",
			-1,
			nullptr
		};
	};
}

PyMODINIT_FUNC PyInit_Entity();
