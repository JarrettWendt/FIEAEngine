#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Attributed::" #name, "[Attributed]")

namespace UnitTests
{
#pragma region AttributedFoo
	TEST(Integers)
	{
		AttributedFoo foo;
		auto& integer = foo.Attribute("integer").Front<int>();
		auto& integers = foo.Attribute("integers").Front<int>();
		
		REQUIRE(&foo.integer == &integer);
		REQUIRE(&foo.integers[0] == &integers);

		for (int i = 0; i < 10; i++)
		{
			REQUIRE(foo.integers[i] == foo.Attribute("integers").Get<int>(i));
		}

		integer = 9;
		REQUIRE(9 == foo.integer);
	}

	TEST(Floats)
	{
		AttributedFoo foo;
		auto& Float = foo.Attribute("Float").Front<float>();
		auto& floats = foo.Attribute("floats").Front<float>();

		REQUIRE(&foo.Float == &Float);
		REQUIRE(&foo.floats[0] == &floats);
		
		for (int i = 0; i < 10; i++)
		{
			REQUIRE(foo.floats[i] == foo.Attribute("floats").Get<float>(i));
		}

		Float = 9.f;
		REQUIRE(9 == foo.Float);
	}

	TEST(Strings)
	{
		AttributedFoo foo;
		auto& string = foo.Attribute("string").Front<std::string>();
		auto& strings = foo.Attribute("strings").Front<std::string>();

		REQUIRE(&foo.string == &string);
		REQUIRE(&foo.strings[0] == &strings);

		for (int i = 0; i < 10; i++)
		{
			REQUIRE(foo.strings[i] == foo.Attribute("strings").Get<std::string>(i));
		}

		string = "hi";
		REQUIRE("hi" == foo.string);
	}

	TEST(Foos)
	{
		AttributedFoo afoo;
		auto& foo = afoo.Attribute("foo").Front<std::shared_ptr<Foo>>();
		const auto foos = afoo.Attribute("foos").Front<std::shared_ptr<Foo>>();

		REQUIRE(afoo.foo == foo);
		REQUIRE(afoo.foos[0] == foos);

		for (int i = 0; i < 10; i++)
		{
			REQUIRE(&afoo.foos[i] == &afoo.Attribute("foos").Get<std::shared_ptr<Foo>>(i));
		}

		foo = std::make_shared<Foo>(9);
		REQUIRE(Foo(9) == *afoo.foo);
	}

	TEST(NoDataMembers)
	{
		AttributedFoo foo;
		REQUIRE(foo.HasAttribute("intWithoutMember"s));
		REQUIRE(foo.HasAttribute("floatWithoutmember"s));
		REQUIRE(foo.HasAttribute("stringWithoutMember"s));
		REQUIRE(foo.HasAttribute("fooWithoutMember"s));
		REQUIRE(foo.HasAttribute("recursive"s));

		REQUIRE(foo.Attribute("fooWithoutMember").IsEmpty());
		// It's a good thing these aren't default constructed or else "recursive" would indeed be its namesake.
	}
#pragma endregion

#pragma region Special Members
	TEST(MoveCtor)
	{
		AttributedFoo moved;
		AttributedFoo foo{ std::move(moved) };
		REQUIRE(9 == foo["integers"].Get<int>(9));
		REQUIRE(!moved.HasAttributes());
	}

	TEST(MoveAssign)
	{
		AttributedFoo moved;
		AttributedFoo foo;
		foo = std::move(moved);
		REQUIRE(9 == foo["integers"].Get<int>(9));
		REQUIRE(!moved.HasAttributes());
	}

	TEST(CopyCtor)
	{
		AttributedFoo copied;
		AttributedFoo foo = copied;
		REQUIRE(9 == foo["integers"].Get<int>(9));
		REQUIRE(9 == copied["integers"].Get<int>(9));
	}

	TEST(CopyAssign)
	{
		AttributedFoo copied;
		AttributedFoo foo;
		foo = copied;
		REQUIRE(9 == foo["integers"].Get<int>(9));
		REQUIRE(9 == copied["integers"].Get<int>(9));
	}
#pragma endregion

#pragma region iterator
	TEST(iterator)
	{
		const AttributedFoo foo;
		size_t count = 0;
		for (auto it = foo.begin(); it != foo.end(); ++it)
		{
			count++;
		}
		REQUIRE(count == foo.NumAttributes());
	}
#pragma endregion
		
#pragma region Properties
	TEST(HasAttributes)
	{
		AttributedFoo foo;
		REQUIRE(foo.HasAttributes());
	}
#pragma endregion

#pragma region Accessors
	TEST(Attribute)
	{
		AttributedFoo foo;
		const AttributedFoo cfoo = foo;
		REQUIRE(foo.Attribute("integer") == cfoo.Attribute("integer"));
	}
#pragma endregion

#pragma region Query
	TEST(Find)
	{
		AttributedFoo foo;
		const AttributedFoo cfoo = foo;
		REQUIRE(*foo.Find("integer") == *cfoo.Find("integer"));
	}
#pragma endregion
		
#pragma region Remove
	TEST(RemoveAttribute)
	{
		AttributedFoo foo;
		const auto count = foo.NumAttributes();
		foo.AddAttribute("a");
		REQUIRE(count + 1 == foo.NumAttributes());
		foo.RemoveAttribute("a");
		REQUIRE(count == foo.NumAttributes());
	}
#pragma endregion

#pragma region Operators
	TEST(Equivalence)
	{
		AttributedFoo a, b;
		a.AddAttribute("1", Datum{ 1 });
		b.AddAttribute("1", Datum{ 1 });

		REQUIRE(a == a);
		REQUIRE(a == b);
		REQUIRE(!(a != b));

		b.AddAttribute("2.0", Datum{ 2.0 });
		REQUIRE(a != b);
		REQUIRE(!(a == b));

		a.AddAttribute("str", Datum{ "hi" });
		REQUIRE(a != b);
		REQUIRE(!(a == b));
	}

	TEST(Subscript)
	{
		AttributedFoo s;
		Datum d = Random::Next<std::vector<int>>();
		
		s.AddAttribute("i", d);
		REQUIRE(d == s["i"]);

		const AttributedFoo cs = s;
		REQUIRE(d == cs["i"]);

		d = Random::Next<std::vector<float>>();
		s["f"] = d;
		REQUIRE(d == s["f"]);
	}
#pragma endregion

#pragma region RTTI
	TEST(StaticTypeID)
	{
		REQUIRE(Attributed::typeID != RTTI::typeID);
	}

	TEST(VirtualTypeID)
	{
		REQUIRE(AttributedFoo().TypeID() == AttributedFoo().TypeID());
		REQUIRE(AttributedFoo().TypeID() != Foo().TypeID());
	}

	TEST(Is)
	{
		REQUIRE(AttributedFoo().Is<Attributed>());
		REQUIRE(!AttributedFoo().Is<Foo>());
	}

	TEST(As)
	{
		REQUIRE(!AttributedFoo().As<Foo>());
		REQUIRE(AttributedFoo().As<Attributed>());
	}
#pragma endregion
}
