#include "../../pch.h"
#include "TestEntity.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "SharedPtr::" #name, "[SharedPtr]")

namespace UnitTests
{
	TEST(ctor)
	{
		SharedPtr<int> p;
	}

	TEST(copy assign)
	{
		auto a = SharedPtr<std::string>::Make("hello");
		auto b = SharedPtr<std::string>::Make("world");
		b = a;
		REQUIRE(a.ReferenceCount() == 2);
	}

	TEST(move assign)
	{
		auto a = SharedPtr<std::string>::Make("hello");
		auto b = a;
		b = SharedPtr<std::string>::Make("world");
		REQUIRE(*b != *a);
	}

	TEST(template copy assign)
	{		
		auto t = SharedPtr<TestEntity>::Make();
		auto e = SharedPtr<Entity>::Make();
		REQUIRE(t.Raw() != e.Raw());
		e = t;
		REQUIRE(e.ReferenceCount() == 2);
		REQUIRE(t.ReferenceCount() == 2);
		REQUIRE(t.Raw() == e.Raw());
	}

	TEST(template move assign)
	{
		auto t = SharedPtr<TestEntity>::Make();
		auto e = SharedPtr<Entity>::Make();
		e = SharedPtr<TestEntity>::Make();
		REQUIRE(e->Is<TestEntity>());
	}

	TEST(operator->)
	{
		const SharedPtr<std::string> p;
		REQUIRE_THROWS_AS(p->size(), NullReferenceException);
	}

	TEST(operator bool)
	{
		SharedPtr<int> p;
		REQUIRE(!p);
		p = SharedPtr<int>::Make();
		REQUIRE(p);
	}

	TEST(operator<<)
	{
		SharedPtr<int> p;
		std::string string = (std::stringstream() << p).str();
		REQUIRE(string == "nullptr");

		p = SharedPtr<int>::Make();
		string = (std::stringstream() << p).str();
		REQUIRE(string == "0");

		struct NonStreamable {};
		
		auto a = SharedPtr<NonStreamable>::Make();
		string = (std::stringstream() << a).str();
		std::string otherString = (std::stringstream() << a.Raw()).str();
		REQUIRE(string == otherString);
	}

	TEST(ReferenceCount)
	{
		SharedPtr<int> a = SharedPtr<int>::Make();
		REQUIRE(a.ReferenceCount() == 1);
		SharedPtr<int> b = a;
		REQUIRE(a.ReferenceCount() == 2);
		REQUIRE(b.ReferenceCount() == 2);
	}

	TEST(Raw)
	{
		SharedPtr<int> p;
		REQUIRE(p.Raw() == nullptr);
		p = SharedPtr<int>::Make();
		REQUIRE(p.Raw());
	}
}
