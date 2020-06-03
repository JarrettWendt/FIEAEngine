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
	TEST_CLASS(AttributedTests)
	{
		INITIALIZE_CLEANUP

		TEST_METHOD(Destruction)
		{
			std::weak_ptr<AttributedFoo> ptr;
			{
				const auto foo = std::make_shared<AttributedFoo>();
				ptr = foo;
			}
			Assert::IsTrue(ptr.expired());
		}

#pragma region AttributedFoo
		TEST_METHOD(Integers)
		{
			auto foo = std::make_shared<AttributedFoo>();
			auto& integer = foo->Attribute("integer").Front<int>();
			auto& integers = foo->Attribute("integers").Front<int>();
			
			Assert::AreSame(foo->integer, integer);
			Assert::AreSame(foo->integers[0], integers);

			for (int i = 0; i < 10; i++)
			{
				Assert::AreEqual(foo->integers[i], foo->Attribute("integers").Get<int>(i));
			}

			integer = 9;
			Assert::AreEqual(9, foo->integer);
		}

		TEST_METHOD(Floats)
		{
			auto foo = std::make_shared<AttributedFoo>();
			auto& Float = foo->Attribute("Float").Front<float>();
			auto& floats = foo->Attribute("floats").Front<float>();

			Assert::AreSame(foo->Float, Float);
			Assert::AreSame(foo->floats[0], floats);
			
			for (int i = 0; i < 10; i++)
			{
				Assert::AreEqual(foo->floats[i], foo->Attribute("floats").Get<float>(i));
			}

			Float = 9.f;
			Assert::AreEqual(9, foo->Float);
		}

		TEST_METHOD(Strings)
		{
			auto foo = std::make_shared<AttributedFoo>();
			auto& string = foo->Attribute("string").Front<std::string>();
			auto& strings = foo->Attribute("strings").Front<std::string>();

			Assert::AreSame(foo->string, string);
			Assert::AreSame(foo->strings[0], strings);

			for (int i = 0; i < 10; i++)
			{
				Assert::AreEqual(foo->strings[i], foo->Attribute("strings").Get<std::string>(i));
			}

			string = "hi";
			Assert::AreEqual("hi", foo->string);
		}

		TEST_METHOD(Foos)
		{
			const auto afoo = std::make_shared<AttributedFoo>();
			auto& foo = afoo->Attribute("foo").Front<std::shared_ptr<Foo>>();
			const auto foos = afoo->Attribute("foos").Front<std::shared_ptr<Foo>>();

			Assert::AreSame(*afoo->foo, *foo);
			Assert::AreSame(*afoo->foos[0], *foos);

			for (int i = 0; i < 10; i++)
			{
				Assert::AreSame(afoo->foos[i], afoo->Attribute("foos").Get<std::shared_ptr<Foo>>(i));
			}

			foo = std::make_shared<Foo>(9);
			Assert::AreEqual(Foo(9), *afoo->foo);
		}

		TEST_METHOD(NoDataMembers)
		{
			const auto foo = std::make_shared<AttributedFoo>();
			Assert::IsTrue(foo->HasAttribute("intWithoutMember"s));
			Assert::IsTrue(foo->HasAttribute("floatWithoutmember"s));
			Assert::IsTrue(foo->HasAttribute("stringWithoutMember"s));
			Assert::IsTrue(foo->HasAttribute("fooWithoutMember"s));
			Assert::IsTrue(foo->HasAttribute("recursive"s));

			Assert::IsTrue(foo->Attribute("fooWithoutMember").IsEmpty(), "it indeed has a Datum of that type with that name, but it's empty");
			// It's a good thing these aren't default constructed or else "recursive" would indeed be its namesake.
		}
#pragma endregion

#pragma region Properties
		TEST_METHOD(NumAttributes)
		{
			auto s = std::make_shared<AttributedFoo>();
			Assert::AreEqual(13, s->NumAttributes());
			s->AddAttribute("hi");
			Assert::AreEqual(14, s->NumAttributes());
		}

		TEST_METHOD(HasAttributes)
		{
			auto s = std::make_shared<AttributedFoo>();
			Assert::IsTrue(s->HasAttributes());
		}
#pragma endregion

#pragma region Remove
		TEST_METHOD(RemoveAttribute)
		{
			auto s = std::make_shared<AttributedFoo>();
			const auto count = s->NumAttributes();
			s->AddAttribute("a");
			Assert::AreEqual(count + 1, s->NumAttributes());
			s->RemoveAttribute("a");
			Assert::AreEqual(count, s->NumAttributes());
		}
#pragma endregion

#pragma region Operators
		TEST_METHOD(Equivalence)
		{
			AttributedFoo a, b;
			a.AddAttribute("1", Datum{ 1 });
			b.AddAttribute("1", Datum{ 1 });

			Assert::IsTrue(a == a, "Newly filled a == a");
			Assert::IsTrue(a == b, "Newly filled a == b");
			Assert::IsFalse(a != b, "Newly filled !(a != b)");

			b.AddAttribute("2.0", Datum{ 2.0 });
			Assert::IsTrue(a != b, "a != b");
			Assert::IsFalse(a == b, "!(a == b)");

			a.AddAttribute("str", Datum{ "hi" });
			Assert::IsTrue(a != b, "a != b");
			Assert::IsFalse(a == b, "!(a == b)");
		}

		TEST_METHOD(Subscript)
		{
			AttributedFoo s;
			Datum d = RandomDatum<int>();
			
			s.AddAttribute("i", d);
			Assert::AreEqual(d, s["i"]);

			const AttributedFoo cs = s;
			Assert::AreEqual(d, cs["i"]);

			d = RandomDatum<float>();
			s["f"] = d;
			Assert::AreEqual(d, s["f"]);
		}
#pragma endregion

#pragma region RTTI
		TEST_METHOD(StaticTypeID)
		{
			Assert::AreNotEqual(Attributed::typeID, RTTI::typeID);
		}

		TEST_METHOD(VirtualTypeID)
		{
			Assert::AreEqual(AttributedFoo().TypeID(), AttributedFoo().TypeID());
			Assert::AreNotEqual(AttributedFoo().TypeID(), Foo().TypeID());
		}

		TEST_METHOD(Is)
		{
			Assert::IsTrue(AttributedFoo().Is<Attributed>());
			Assert::IsFalse(AttributedFoo().Is<Foo>());
		}

		TEST_METHOD(As)
		{
			Assert::IsNull(AttributedFoo().As<Foo>());
			Assert::IsNotNull(AttributedFoo().As<Attributed>());
		}

		TEST_METHOD(Equals)
		{
			Assert::IsTrue(AttributedFoo().Equals(AttributedFoo()));
			Assert::IsFalse(AttributedFoo().Equals(Foo()));
		}
#pragma endregion
	};
}
