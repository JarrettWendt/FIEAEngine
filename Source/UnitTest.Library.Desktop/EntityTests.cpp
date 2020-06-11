// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;

template<typename T>
using Rand = UnitTests::Random<T>;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EntityTests)
	{
		INITIALIZE_CLEANUP
		
		TEST_METHOD(Destruction)
		{
			std::weak_ptr<Entity> ptr;
			{
				const auto foo = std::make_shared<Entity>();
				ptr = foo;
			}
			Assert::IsTrue(ptr.expired());
		}

#pragma region iterator
		TEST_METHOD(ConstIterator)
		{
			const auto e = std::make_shared<Entity>();

			auto cit = e->cbegin();
			Assert::AreEqual(cit, e->cend());
			
			Assert::IsTrue(cit.IsAtBegin());
			Assert::IsTrue(cit.IsAtEnd());

			Assert::IsFalse(cit);
			Assert::IsTrue(!cit);

			const auto c = e->CreateChild();
			cit = e->begin();

			Assert::AreEqual(c, *cit);
			Assert::AreEqual(e, cit->Parent());

			cit++;
			Assert::IsFalse(cit);
		}
#pragma endregion

#pragma region Properties
		TEST_METHOD(Child)
		{
			const auto p = std::make_shared<Entity>();
			Assert::IsNull(p->Child("bleh"));
			
			const auto c = p->CreateChild();
			const std::shared_ptr<const Entity> cp = p;

			Assert::AreEqual(c, cp->Child(c->GetName()));
		}

		TEST_METHOD(Parent)
		{
			const auto p = std::make_shared<Entity>();
			const auto c = p->CreateChild();
			const std::shared_ptr<const Entity> cc = c;

			Assert::AreEqual(p, cc->Parent());
		}
#pragma endregion

#pragma region Transform
		TEST_METHOD(SetLocalTransformFromParent)
		{			
			const auto p = std::make_shared<Entity>();
			const auto c = p->CreateChild<>();

			Assert::AreEqual(p->GetLocalTransform(), p->GetWorldTransform());
			Assert::AreEqual(p->GetWorldTransform(), c->GetWorldTransform());

			auto t = p->GetLocalTransform();
			t.translation = { 0, 0, 1 };
			p->SetLocalTransform(t);

			Assert::AreEqual(t, p->GetLocalTransform());
			Assert::AreEqual(t, p->GetWorldTransform());
			Assert::AreEqual(t, c->GetWorldTransform());
			Assert::AreEqual(Transform{}, c->GetLocalTransform());
		}

		TEST_METHOD(SetWorldTransformFromParent)
		{
			const auto p = std::make_shared<Entity>();
			const auto c = p->CreateChild<>();

			Assert::AreEqual(p->GetLocalTransform(), p->GetWorldTransform());
			Assert::AreEqual(p->GetWorldTransform(), c->GetWorldTransform());

			auto t = p->GetLocalTransform();
			t.translation = { 0, 0, 1 };
			p->SetWorldTransform(t);

			Assert::AreEqual(t, p->GetLocalTransform());
			Assert::AreEqual(t, p->GetWorldTransform());
			Assert::AreEqual(t, c->GetWorldTransform());
			Assert::AreEqual(Transform{}, c->GetLocalTransform());
		}
		
		TEST_METHOD(SetLocalTransformFromChild)
		{
			const auto p = std::make_shared<Entity>();
			const auto c = p->CreateChild<>();

			Assert::AreEqual(p->GetLocalTransform(), p->GetWorldTransform());
			Assert::AreEqual(p->GetWorldTransform(), c->GetWorldTransform());

			auto t = c->GetLocalTransform();
			t.translation = { 0, 0, 1 };
			c->SetLocalTransform(t);

			Assert::AreEqual(Transform{}, p->GetLocalTransform());
			Assert::AreEqual(Transform{}, p->GetWorldTransform());
			Assert::AreEqual(t, c->GetWorldTransform());
			Assert::AreEqual(t, c->GetLocalTransform());
		}

		TEST_METHOD(SetWorldTransformFromChild)
		{
			const auto p = std::make_shared<Entity>();
			const auto c = p->CreateChild<>();

			Assert::AreEqual(p->GetLocalTransform(), p->GetWorldTransform());
			Assert::AreEqual(p->GetWorldTransform(), c->GetWorldTransform());

			auto t = c->GetLocalTransform();
			t.translation = { 0, 0, 1 };
			c->SetWorldTransform(t);

			Assert::AreEqual(Transform{}, p->GetLocalTransform());
			Assert::AreEqual(Transform{}, p->GetWorldTransform());
			Assert::AreEqual(t, c->GetWorldTransform());
			Assert::AreEqual(t, c->GetLocalTransform());
		}
#pragma endregion

		TEST_METHOD(SetName)
		{
			const auto p = std::make_shared<Entity>();
			const auto c = p->CreateChild<>("child"s);
			c->SetName("Seven");
			Assert::AreEqual("Seven"s, c->GetName());
			Assert::IsTrue(p->Child("Seven"));
		}

		TEST_METHOD(GetName)
		{
			const auto e = std::make_shared<Entity>();
			Assert::AreEqual("Entity"s, e->GetName());
		}
		
#pragma region Insert
		TEST_METHOD(Insert)
		{
			Assert::ExpectException<Entity::InvalidNameException>([] { Entity().CreateChild(""s); });
		}

		TEST_METHOD(InsertScope)
		{
			auto parent = std::make_shared<Entity>();
			const auto child = parent->CreateChild("child"s);

			Assert::AreEqual(parent, child->Parent());
			Assert::AreEqual("child", child->GetName());
		}

		TEST_METHOD(Adopt)
		{
			auto parent = std::make_shared<Entity>();
			auto child = std::make_shared<Entity>();

			Assert::IsFalse(parent->Parent());
			Assert::IsFalse(child->Parent());

			child = parent->Adopt("child", child);

			Assert::IsFalse(parent->Parent());
			Assert::IsTrue(child->Parent());
			Assert::AreEqual(parent, child->Parent());
			Assert::AreEqual("child", child->GetName());

			const auto child2 = parent->Adopt("child", child);

			Assert::AreSame(*child2, *child);
			Assert::AreEqual("child", child2->GetName());

			auto newParent = std::make_shared<Entity>();
			const auto child3 = newParent->Adopt("child", child2);

			Assert::AreSame(*newParent, *child3->Parent());
			Assert::AreEqual("child", child3->GetName());

			Assert::ExpectException<Entity::InvalidNameException>([&] { newParent->CreateChild<>("child"s); });
		}

		TEST_METHOD(Grandchildren)
		{
			auto parent = std::make_shared<Entity>();
			parent->CreateChild<>("child"s)->CreateChild<>("grandChild"s)->CreateChild<>("greatGrandChild"s);
		}
#pragma endregion
		
#pragma region Remove
		TEST_METHOD(Orphan)
		{
			auto s = std::make_shared<Entity>();
			s->Orphan();
			Assert::IsFalse(s->Parent());
			
			auto p = std::make_shared<Entity>();
			p->Adopt(s);
			Assert::IsTrue(s->Parent());
			s->Orphan();
			Assert::IsFalse(s->Parent());
		}

		TEST_METHOD(Remove)
		{
			auto s = std::make_shared<Entity>();
			const auto child = s->CreateChild<>("child"s);
			
			s->RemoveChild("child");
			Assert::IsFalse(child->Parent());
			Assert::IsFalse(s->HasChildren());
		}
#pragma endregion

		TEST_METHOD(Init)
		{
			const auto e = std::make_shared<Entity>();
			e->CreateChild("child"s);
			e->Init();
		}

		TEST_METHOD(Update)
		{
			const auto e = std::make_shared<Entity>();
			e->CreateChild("child"s);
			e->Update();
		}

#pragma region RTTI
		TEST_METHOD(StaticTypeID)
		{
			Assert::AreNotEqual(Entity::typeID, RTTI::typeID);
		}

		TEST_METHOD(VirtualTypeID)
		{
			Assert::AreEqual(Entity().TypeID(), Entity().TypeID());
			Assert::AreNotEqual(Entity().TypeID(), Foo().TypeID());
		}

		TEST_METHOD(Is)
		{
			Assert::IsTrue(Entity().Is<Attributed>());
			Assert::IsFalse(Entity().Is<Foo>());
		}

		TEST_METHOD(As)
		{
			Assert::IsNull(Entity().As<Foo>());
			Assert::IsNotNull(Entity().As<Attributed>());
		}
#pragma endregion
	};
}
