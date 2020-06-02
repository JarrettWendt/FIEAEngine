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
		template<typename T>
		using Container = SList<T>;

		INITIALIZE_CLEANUP

		TEST_METHOD(Ctor)
		{
			Scope s(11);
		}
		
#pragma region Properties
		TEST_METHOD(Size)
		{
			Scope s;
			Assert::AreEqual(0, s.Size());
			s.Insert("hi");
			Assert::AreEqual(1, s.Size());
		}

		TEST_METHOD(IsEmpty)
		{
			Scope s;
			Assert::IsTrue(s.IsEmpty());
			s.Insert("hi");
			Assert::IsFalse(s.IsEmpty());
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
			Scope parent;
			const auto child = parent.InsertScope("child");

			Assert::AreSame(parent, *child->Parent());
			Assert::AreEqual("child", child->NameInParent());
		}
		
		TEST_METHOD(Adopt)
		{
			Scope parent;
			const auto child = std::make_shared<Scope>();

			Assert::IsNull(parent.Parent());
			Assert::IsNull(child->Parent());

			parent.Adopt("child", child);

			Assert::IsNull(parent.Parent());
			Assert::IsNotNull(child->Parent());
			Assert::AreSame(parent, *child->Parent());
			Assert::AreEqual("child", child->NameInParent());

			const auto child2 = parent.Adopt("child", child);

			Assert::AreSame(*child2, *child);
			Assert::AreEqual("child", child2->NameInParent());

			Scope newParent;
			const auto child3 = newParent.Adopt("child", child);
			
			Assert::AreSame(newParent, *child->Parent());
			Assert::AreSame(*child3, *child);
			Assert::AreEqual("child", child3->NameInParent());
		}
#pragma endregion

#pragma region Remove
		TEST_METHOD(Orphan)
		{
			Scope s;
			s.Orphan();
			Assert::IsNull(s.Parent());
		}

		TEST_METHOD(Remove)
		{
			Scope s;
			const auto child = s.InsertScope("child");
			s.Insert("datum");
			
			s.Remove("child");
			Assert::IsNull(child->Parent());
			Assert::IsTrue(child->NameInParent().empty());

			s.Remove("datum");

			Assert::IsTrue(s.IsEmpty());
		}
#pragma endregion
		
#pragma region Operators
		TEST_METHOD(Equivalence)
		{
			Scope a, b;
			a.Insert("1", Datum{ 1 });
			b.Insert("1", Datum{ 1 });

			Assert::IsTrue(a == a, "Newly filled a == a");
			Assert::IsTrue(a == b, "Newly filled a == b");
			Assert::IsFalse(a != b, "Newly filled !(a != b)");

			b.Insert("2.0", Datum{ 2.0 });
			Assert::IsTrue(a != b, "a != b");
			Assert::IsFalse(a == b, "!(a == b)");

			a.Insert("str", Datum{ "hi" });
			Assert::IsTrue(a != b, "a != b");
			Assert::IsFalse(a == b, "!(a == b)");
		}

		TEST_METHOD(Subscript)
		{
			Scope s;
			Datum d = RandomDatum<int>();
			
			s.Insert("i", d);
			Assert::AreEqual(d, s["i"]);

			const Scope cs = s;
			Assert::AreEqual(d, cs["i"]);

			d = RandomDatum<float>();
			s["f"] = d;
			Assert::AreEqual(d, s["f"]);
		}
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
