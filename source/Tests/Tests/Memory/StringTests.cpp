#include "../../pch.h"
#include "InternedString.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "String" #name, "[String]")

namespace UnitTests
{
	TEST(NumInterned)
	{
		String a = "hello, world!"_s;
		String b = "hello, world!";
		REQUIRE(String::NumInterned() == 1);
	}
}
