#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Vector2::" #name, "[Vector2]")

namespace UnitTests
{
	TEST(operator[](std::integral_constant))
	{
		Vector2 v = { 1, 2 };
		REQUIRE(v[0_zc] == 1);
	}
}
