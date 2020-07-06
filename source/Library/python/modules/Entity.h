// MIT License Copyright(c) 2020 Jarrett Wendt

#pragma once

#include "python/pch.h"
#include "python/shared_ptr.h"
#include "hierarchy/Entity.h"

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
	class EntityBinding : public PyObject
	{		
		struct Vector3Wrapper : public PyObject
		{
			// not necessarily local, we force cast to the value of space
			// not necessarily scale, we force cast to the value of component
			Entity::Vector3Wrapper<CoordinateSpace::Local, Transform::Component::Scale> v;
			CoordinateSpace space;
			Transform::Component component;

			PyFloatObject* GetX();
			PyFloatObject* GetY();
			PyFloatObject* GetZ();

			int SetX(PyFloatObject* value);
			int SetY(PyFloatObject* value);
			int SetZ(PyFloatObject* value);

			static inline PyGetSetDef getset[]
			{
				{ "x", Util::UnionCast<getter>(&Vector3Wrapper::GetX), Util::UnionCast<setter>(&Vector3Wrapper::SetX), "x component", nullptr },
				{ "y", Util::UnionCast<getter>(&Vector3Wrapper::GetY), Util::UnionCast<setter>(&Vector3Wrapper::SetY), "y component", nullptr },
				{ "z", Util::UnionCast<getter>(&Vector3Wrapper::GetZ), Util::UnionCast<setter>(&Vector3Wrapper::SetZ), "z component", nullptr },
				
				{ nullptr }
			};
			
			static PyTypeObject type;

			template<typename Functor>
			auto Do(Functor f)
			{
				if (space == CoordinateSpace::Local)
				{
					return Do<CoordinateSpace::Local>(f);
				}
				else
				{
					return Do<CoordinateSpace::World>(f);
				}
			}

			template<CoordinateSpace Space, typename Functor>
			auto Do(Functor f)
			{
				if (component == Transform::Component::Scale)
				{
					return f(reinterpret_cast<Library::Entity::Vector3Wrapper<Space, Transform::Component::Scale>&>(v));
				}
				else
				{
					return f(reinterpret_cast<Library::Entity::Vector3Wrapper<Space, Transform::Component::Translation>&>(v));
				}
			}
		};
		
		struct QuaternionWrapper : public PyObject
		{
			// not necessarily local, we force cast to the value of space
			Entity::QuaternionWrapper<CoordinateSpace::Local> q;
			CoordinateSpace space;

			PyFloatObject* GetX();
			PyFloatObject* GetY();
			PyFloatObject* GetZ();
			PyFloatObject* GetW();

			int SetX(PyFloatObject* value);
			int SetY(PyFloatObject* value);
			int SetZ(PyFloatObject* value);
			int SetW(PyFloatObject* value);

			static inline PyGetSetDef getset[]
			{
				{ "x", Util::UnionCast<getter>(&QuaternionWrapper::GetX), Util::UnionCast<setter>(&QuaternionWrapper::SetX), "x component", nullptr },
				{ "y", Util::UnionCast<getter>(&QuaternionWrapper::GetY), Util::UnionCast<setter>(&QuaternionWrapper::SetY), "y component", nullptr },
				{ "z", Util::UnionCast<getter>(&QuaternionWrapper::GetZ), Util::UnionCast<setter>(&QuaternionWrapper::SetZ), "z component", nullptr },
				{ "w", Util::UnionCast<getter>(&QuaternionWrapper::GetW), Util::UnionCast<setter>(&QuaternionWrapper::SetW), "w component", nullptr },
				
				{ nullptr }
			};
			
			static PyTypeObject type;

			template<typename Functor>
			auto Do(Functor f)
			{
				if (space == CoordinateSpace::Local)
				{
					return f(reinterpret_cast<Library::Entity::QuaternionWrapper<CoordinateSpace::Local>&>(q));
				}
				else
				{
					return f(reinterpret_cast<Library::Entity::QuaternionWrapper<CoordinateSpace::World>&>(q));
				}
			}
		};
		
		struct TransformWrapper : public PyObject
		{
			// not necessarily local, we force cast to the value of space
			py::Entity::TransformWrapper<CoordinateSpace::Local> t;
			CoordinateSpace space;

			Vector3Wrapper* GetTranslation();
			int SetTranslation(Vector3Wrapper* value);
			Vector3Wrapper* GetScale();
			int SetScale(Vector3Wrapper* value);
			QuaternionWrapper* GetRotation();
			int SetRotation(QuaternionWrapper* value);

			static inline PyGetSetDef getset[]
			{
				{ "translation", Util::UnionCast<getter>(&TransformWrapper::GetTranslation), Util::UnionCast<setter>(&TransformWrapper::SetTranslation), "translation component", nullptr },
				{ "rotation", Util::UnionCast<getter>(&TransformWrapper::GetRotation), Util::UnionCast<setter>(&TransformWrapper::SetRotation), "rotation component", nullptr },
				{ "scale", Util::UnionCast<getter>(&TransformWrapper::GetScale), Util::UnionCast<setter>(&TransformWrapper::SetScale), "scale component", nullptr },

				{ nullptr }
			};
			
			static PyTypeObject type;

			template<typename Functor>
			auto Do(Functor f)
			{
				if (space == CoordinateSpace::Local)
				{
					return f(reinterpret_cast<Library::Entity::TransformWrapper<CoordinateSpace::Local>&>(t));
				}
				else
				{
					return f(reinterpret_cast<Library::Entity::TransformWrapper<CoordinateSpace::World>&>(t));
				}
			}
		};
		
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
		EntityBinding* GetParent();
		int SetParent(EntityBinding* value);
		TransformWrapper* GetLocalTransform();
		int SetLocalTransform(TransformWrapper* value);
		TransformWrapper* GetWorldTransform();
		int SetWorldTransform(TransformWrapper* value);
				
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
			{ "localTransform", Util::UnionCast<getter>(&EntityBinding::GetLocalTransform), Util::UnionCast<setter>(&EntityBinding::SetLocalTransform), "the local transform of this Entity", nullptr },
			{ "worldTransform", Util::UnionCast<getter>(&EntityBinding::GetWorldTransform), Util::UnionCast<setter>(&EntityBinding::SetWorldTransform), "the world transform of this Entity", nullptr },
			
			{ nullptr }
		};

		static inline PyMemberDef members[]
		{
			{ nullptr }
		};

		// can't be inline because it needs sizeof(EntityBinding)
		static PyTypeObject type;

		static inline PyModuleDef module
		{
			PyModuleDef_HEAD_INIT,
			"Entity",
			"Entity module",
			-1,
			nullptr
		};

#pragma region helpers
		template<CoordinateSpace Space>
		TransformWrapper* GetTransform()
		{
			TransformWrapper* ret = Util::New<TransformWrapper>();
			const auto t = e->Transform<Space>();
			std::memcpy(&ret->t, &t, sizeof(decltype(t)));
			ret->space = Space;
			return ret;
		}

	public:
		static EntityBinding* FromEntity(std::shared_ptr<Library::Entity> entity) noexcept;
#pragma endregion
	};
}
