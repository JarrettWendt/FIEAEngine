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

#pragma region Remove
		TEST_METHOD(Orphan)
		{
			auto s = std::make_shared<Entity>();
			s->Orphan();
			Assert::IsFalse(s->Parent());
		}

#pragma region Insert
		TEST_METHOD(Insert)
		{
			Assert::ExpectException<Entity::InvalidNameException>([] { Entity().CreateChild<>(""); });
		}

		TEST_METHOD(InsertScope)
		{
			auto parent = std::make_shared<Entity>();
			const auto child = parent->CreateChild<>("child");

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
		}

		TEST_METHOD(Grandchildren)
		{
			auto parent = std::make_shared<Entity>();
			parent->CreateChild<>("child")->CreateChild<>("grandChild")->CreateChild<>("greatGrandChild");
		}
#pragma endregion

		TEST_METHOD(Remove)
		{
			auto s = std::make_shared<Entity>();
			const auto child = s->CreateChild<>("child");
			
			s->RemoveChild("child");
			Assert::IsFalse(child->Parent());
			Assert::IsFalse(s->HasChildren());
		}
#pragma endregion

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

		TEST_METHOD(Equals)
		{
			Assert::IsTrue(Entity().Equals(Entity()));
			Assert::IsFalse(Entity().Equals(Foo()));
		}
#pragma endregion
	};
}
