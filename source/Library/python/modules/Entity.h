// MIT License Copyright(c) 2020 Jarrett Wendt

#pragma once

#include "python/pch.h"
#include "python/shared_ptr.h"

#include <Entity.h>

namespace Library::py
{	
	PyObject* InitEntityModule();

	/**
	 * All Python Entities will inherit from py::Entity.
	 * So they won't have their own unique RTTI::IDType, but they will at least share py::Entity::typeID.
	 */
	[[Reflectable]];
	class Entity : public Library::Entity
	{
		shared_ptr pyObject;
		ATTRIBUTED_DECLARATIONS(Library::Entity)
		ATTRIBUTED_SPECIAL_MEMBERS(Entity, default)
		friend class EntityBinding;

	protected:
		virtual void Init() override;
		virtual void Update() override;
	};

	/**
	 * The actual class that will be instantiated for every python Entity.
	 * References a Library::Entity, not necessarily a py::Entity.
	 */
	class EntityBinding
	{
		PyObject_HEAD
		
		friend PyObject* InitEntityModule();

		std::shared_ptr<Library::Entity> e;
		
		static void dealloc(EntityBinding* self);
		static EntityBinding* _new(PyTypeObject* t, PyObject* args, PyObject* kwds);
		static int init(EntityBinding* self, PyObject* args, PyObject* kwds);

		PyObject* richcompare(EntityBinding* other, int op);

		PyObject* GetName();
		int SetName(PyObject* value);
		PyObject* GetEnabled();
		int SetEnabled(PyObject* value);
		PyObject* GetParent();
		int SetParent(EntityBinding* value);
				
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
			{ "numChildren", Util::UnionCast<getter>(&EntityBinding::NumChildren), nullptr, "how many children this Entity has", nullptr },
			{ "hasChildren", Util::UnionCast<getter>(&EntityBinding::HasChildren), nullptr, "whether or not this Entity has any children", nullptr },

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

	public:
		static EntityBinding* FromEntity(std::shared_ptr<Library::Entity> entity) noexcept;
	};
}
