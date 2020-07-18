#include "../../pch.h"

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
