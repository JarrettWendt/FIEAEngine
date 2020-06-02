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
	TEST_CLASS(ScopeTests)
	{
		INITIALIZE_CLEANUP

		TEST_METHOD(Ctor)
		{
			auto s = std::make_shared<Scope>(11);
		}
		
#pragma region Properties
		TEST_METHOD(Size)
		{
			auto s = std::make_shared<Scope>();
			Assert::AreEqual(0, s->Size());
			s->Insert("hi");
			Assert::AreEqual(1, s->Size());
		}

		TEST_METHOD(IsEmpty)
		{
			auto s = std::make_shared<Scope>();
			Assert::IsTrue(s->IsEmpty());
			s->Insert("hi");
			Assert::IsFalse(s->IsEmpty());
		}
#pragma endregion
		
		TEST_METHOD(At)
		{
			Assert::ExpectException<std::out_of_range>([] { const Scope s; s.At("bleh"); });
		}

#pragma region Insert
		TEST_METHOD(Insert)
		{
			Assert::ExpectException<Scope::InvalidNameException>([] { Scope().Insert(""); });
		}
		
		TEST_METHOD(InsertScope)
		{
			auto parent = std::make_shared<Scope>();
			const auto child = parent->InsertScope("child");

			Assert::AreEqual(parent, child->Parent());
			Assert::AreEqual("child", child->NameInParent());
		}
		
		TEST_METHOD(Adopt)
		{
			auto parent = std::make_shared<Scope>();
			const auto child = std::make_shared<Scope>();

			Assert::IsFalse(parent->Parent());
			Assert::IsFalse(child->Parent());

			parent->Adopt("child", child);

			Assert::IsFalse(parent->Parent());
			Assert::IsTrue(child->Parent());
			Assert::AreEqual(parent, child->Parent());
			Assert::AreEqual("child", child->NameInParent());

			const auto child2 = parent->Adopt("child", child);

			Assert::AreSame(*child2, *child);
			Assert::AreEqual("child", child2->NameInParent());

			auto newParent = std::make_shared<Scope>();
			const auto child3 = newParent->Adopt("child", child);
			
			Assert::AreEqual(newParent, child->Parent());
			Assert::AreEqual(child3, child);
			Assert::AreEqual("child", child3->NameInParent());
		}

		TEST_METHOD(Grandchildren)
		{
			auto parent = std::make_shared<Scope>();
			parent->InsertScope("child")->InsertScope("grandChild")->InsertScope("greatGrandChild");
		}
#pragma endregion

#pragma region Remove
		TEST_METHOD(Orphan)
		{
			auto s = std::make_shared<Scope>();
			s->Orphan();
			Assert::IsFalse(s->Parent());
		}

		TEST_METHOD(Remove)
		{
			auto s = std::make_shared<Scope>();
			const auto child = s->InsertScope("child");
			s->Insert("datum");
			
			s->Remove("child");
			Assert::IsFalse(child->Parent());
			Assert::IsTrue(child->NameInParent().empty());

			s->Remove("datum");

			Assert::IsTrue(s->IsEmpty());
		}
#pragma endregion
		
#pragma region Operators
		TEST_METHOD(Equivalence)
		{
			auto a = std::make_shared<Scope>();
			auto b = std::make_shared<Scope>();
			a->Insert("1", Datum{ 1 });
			b->Insert("1", Datum{ 1 });

			Assert::IsTrue(*a == *a, "Newly filled a == a");
			Assert::IsTrue(*a == *b, "Newly filled a == b");
			Assert::IsFalse(*a != *b, "Newly filled !(a != b)");

			b->Insert("2.0", Datum{ 2.0 });
			Assert::IsTrue(*a != *b, "a != b");
			Assert::IsFalse(*a == *b, "!(a == b)");

			a->Insert("str", Datum{ "hi" });
			Assert::IsTrue(*a != *b, "a != b");
			Assert::IsFalse(*a == *b, "!(a == b)");
		}

		// TODO: no point of a subscript operator if there's only ever shared_ptrs of Scopes
		//TEST_METHOD(Subscript)
		//{
		//	Scope s;
		//	Datum d = RandomDatum<int>();
		//	
		//	s.Insert("i", d);
		//	Assert::AreEqual(d, s["i"]);

		//	const Scope cs = s;
		//	Assert::AreEqual(d, cs["i"]);

		//	d = RandomDatum<float>();
		//	s["f"] = d;
		//	Assert::AreEqual(d, s["f"]);
		//}
#pragma endregion

#pragma region RTTI
		TEST_METHOD(StaticTypeID)
		{
			Assert::AreNotEqual(Scope::typeID, RTTI::typeID);
		}

		TEST_METHOD(VirtualTypeID)
		{
			Assert::AreEqual(Scope().TypeID(), Scope().TypeID());
			Assert::AreNotEqual(Scope().TypeID(), Foo().TypeID());
		}

		TEST_METHOD(Is)
		{
			Assert::IsTrue(Scope().Is<Scope>());
			Assert::IsFalse(Scope().Is<Foo>());
		}

		TEST_METHOD(As)
		{
			Assert::IsNull(Scope().As<Foo>());
			Assert::IsNotNull(Scope().As<Scope>());
		}

		TEST_METHOD(Equals)
		{
			Assert::IsTrue(Scope().Equals(Scope()));
			Assert::IsFalse(Scope().Equals(Foo()));
		}
#pragma endregion
	};
}
