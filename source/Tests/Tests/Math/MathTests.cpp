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

	TEST(ReMap)
	{
		REQUIRE(Math::ReMap(5, 0, 9, 0, 90) == 50);
	}

	TEST(ReMap01)
	{
		REQUIRE(Math::ReMap01(5.f, 0.f, 10.f) == 0.5f);
	}
}
