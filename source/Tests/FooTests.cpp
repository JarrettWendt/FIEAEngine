#include "pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Foo::" #name, "[Foo]")

namespace UnitTests
{
	TEST(HasID)
	{
		const Foo f;
		const RTTI* r = f.As<RTTI>();
		REQUIRE(!r->HasID(std::numeric_limits<RTTI::IDType>::max()));
	}
	
	TEST(Is)
	{
		const Foo f;		
		REQUIRE(f.Is<RTTI>());		
		const RTTI* r = f.As<RTTI>();		
		REQUIRE(r->Is<Foo>());
	}

	TEST(As)
	{
		const Foo f;
		REQUIRE(f.As<Foo>());
		REQUIRE(f.As<RTTI>());
	}

	TEST(AssertAs)
	{
		const Foo f;
		REQUIRE(&f.AssertAs<Foo>());
		REQUIRE(&f.AssertAs<RTTI>());
	}
}
