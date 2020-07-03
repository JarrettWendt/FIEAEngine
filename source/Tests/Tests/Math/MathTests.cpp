#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;

#define TEST(name) TEST_CASE_METHOD(MemLeak, #name, "[Math]")

namespace UnitTests
{
	TEST(Mod)
	{
		REQUIRE(1 == Math::Mod(1, 10));
		REQUIRE(9 == Math::Mod(-1, 10));
		REQUIRE(-9 == Math::Mod(1, -10));
	}
}
