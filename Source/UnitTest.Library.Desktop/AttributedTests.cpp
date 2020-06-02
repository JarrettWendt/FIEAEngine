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

		TEST_METHOD(Integers)
		{
			auto foo = std::make_shared<AttributedFoo>();
			auto& integer = foo->At("integer").Front<int>();
			auto& integers = foo->At("integers").Front<int>();
			
			Assert::AreSame(foo->integer, integer);
			Assert::AreSame(foo->integers[0], integers);

			for (int i = 0; i < 10; i++)
			{
				Assert::AreEqual(foo->integers[i], foo->At("integers").Get<int>(i));
			}

			integer = 9;
			Assert::AreEqual(9, foo->integer);
		}

		TEST_METHOD(Floats)
		{
			auto foo = std::make_shared<AttributedFoo>();
			auto& Float = foo->At("Float").Front<float>();
			auto& floats = foo->At("floats").Front<float>();

			Assert::AreSame(foo->Float, Float);
			Assert::AreSame(foo->floats[0], floats);
			
			for (int i = 0; i < 10; i++)
			{
				Assert::AreEqual(foo->floats[i], foo->At("floats").Get<float>(i));
			}

			Float = 9.f;
			Assert::AreEqual(9, foo->Float);
		}

		TEST_METHOD(Strings)
		{
			auto foo = std::make_shared<AttributedFoo>();
			auto& string = foo->At("string").Front<std::string>();
			auto& strings = foo->At("strings").Front<std::string>();

			Assert::AreSame(foo->string, string);
			Assert::AreSame(foo->strings[0], strings);

			for (int i = 0; i < 10; i++)
			{
				Assert::AreEqual(foo->strings[i], foo->At("strings").Get<std::string>(i));
			}

			string = "hi";
			Assert::AreEqual("hi", foo->string);
		}

		TEST_METHOD(Foos)
		{
			const auto afoo = std::make_shared<AttributedFoo>();
			auto& foo = afoo->At("foo").Front<std::shared_ptr<Foo>>();
			const auto foos = afoo->At("foos").Front<std::shared_ptr<Foo>>();

			Assert::AreSame(*afoo->foo, *foo);
			Assert::AreSame(*afoo->foos[0], *foos);

			for (int i = 0; i < 10; i++)
			{
				Assert::AreSame(afoo->foos[i], afoo->At("foos").Get<std::shared_ptr<Foo>>(i));
			}

			foo = std::make_shared<Foo>(9);
			Assert::AreEqual(Foo(9), *afoo->foo);
		}

		TEST_METHOD(scope)
		{
			const auto foo = std::make_shared<AttributedFoo>();
			const auto s = foo->At("scope").Front<std::shared_ptr<Scope>>();

			Assert::IsFalse(s->Parent(), "a scope attribute isn't necessarily a child scope");
		}

		TEST_METHOD(NoDataMembers)
		{
			const auto foo = std::make_shared<AttributedFoo>();
			Assert::IsTrue(foo->Contains("intWithoutMember"s));
			Assert::IsTrue(foo->Contains("floatWithoutmember"s));
			Assert::IsTrue(foo->Contains("stringWithoutMember"s));
			Assert::IsTrue(foo->Contains("fooWithoutMember"s));
			Assert::IsTrue(foo->Contains("scopeWithoutMember"s));
			Assert::IsTrue(foo->Contains("recursive"s));

			Assert::IsTrue(foo->At("fooWithoutMember").IsEmpty(), "it indeed has a Datum of that type with that name, but it's empty");
			// It's a good thing these aren't default constructed or else "recursive" would indeed be its namesake.
		}
	};
}
