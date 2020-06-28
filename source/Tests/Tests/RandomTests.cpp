#include "../pch.h"

using namespace std::string_literals;
using namespace Library;

#define TEST(name) TEST_CASE_METHOD(MemLeak, name, "[Random]")

namespace UnitTests
{
	TEST("Range")
	{
		auto r = Random::Range<size_t>(0, 100);
		REQUIRE(std::is_same_v<decltype(r), size_t>);
		REQUIRE(r <= 100);
		REQUIRE(r >= 0);
	}
}
