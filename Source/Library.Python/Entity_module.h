#pragma once

#include <Python.h>
#include <structmember.h>

#include "Entity.h"

namespace Library::py
{
	[[Reflectable]];
	class Entity : public Library::Entity
	{
		ATTRIBUTED_DECLARATIONS(Library::Entity)
		ATTRIBUTED_SPECIAL_MEMBERS(Entity, default)
		friend struct EntityBinding;
	};
	
	struct EntityBinding
	{
		PyObject_HEAD

		std::shared_ptr<Library::Entity> e;

		static void dealloc(EntityBinding* self);
		static EntityBinding* _new(PyTypeObject* t, PyObject* args, PyObject* kwds);
		static int init(EntityBinding* self, PyObject* args, PyObject* kwds);

		PyObject* richcompare(EntityBinding* other, int op);

		PyObject* GetName(void* closure);
		int SetName(PyObject* value, void* closure);
		PyObject* GetEnabled(void* closure);
		int SetEnabled(PyObject* value, void* closure);
		PyObject* GetParent(void* closure);
		int SetParent(EntityBinding* value, void* closure);

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
			{ "NumChildren", Util::UnionCast<PyCFunction>(&EntityBinding::NumChildren), METH_NOARGS, "how many children this EntityBinding has" },
			{ "HasChildren", Util::UnionCast<PyCFunction>(&EntityBinding::HasChildren), METH_NOARGS, "how many children this EntityBinding has" },
			{ "Child", Util::UnionCast<PyCFunction>(&EntityBinding::Child), METH_O, "get child by name" },

			{ "Adopt", Util::UnionCast<PyCFunction>(&EntityBinding::Adopt), METH_VARARGS | METH_KEYWORDS, "make the passed EntityBinding a child of this one" },

			{ "Orphan", Util::UnionCast<PyCFunction>(&EntityBinding::Orphan), METH_NOARGS, "orphans this EntityBinding from its parent" },
			{ "RemoveChild", Util::UnionCast<PyCFunction>(&EntityBinding::RemoveChild), METH_O, "removes child by name" },

			{ "_Init", Util::UnionCast<PyCFunction>(&EntityBinding::Init), METH_NOARGS, "initialization ran after construction before the first Update" },
			{ "_Update", Util::UnionCast<PyCFunction>(&EntityBinding::Update), METH_NOARGS, "initialization ran after construction before the first Update" },

			{ nullptr }
		};

		static inline PyGetSetDef getset[]
		{
			{ "name", Util::UnionCast<getter>(&EntityBinding::GetName), Util::UnionCast<setter>(&EntityBinding::SetName), "name of this EntityBinding", nullptr },
			{ "enabled", Util::UnionCast<getter>(&EntityBinding::GetEnabled), Util::UnionCast<setter>(&EntityBinding::SetEnabled), "whether or not this EntityBinding is enabled", nullptr },
			{ "parent", Util::UnionCast<getter>(&EntityBinding::GetParent), Util::UnionCast<setter>(&EntityBinding::SetParent), "this EntityBinding's parent", nullptr },

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
