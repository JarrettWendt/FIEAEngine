#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Literals::" #name, "[Literals]")

namespace UnitTests
{
	TEST(size_t)
	{
		REQUIRE(std::is_same_v<decltype(0_z), size_t>);
	}

	TEST(std::integral_constant<size_t>)
	{
		auto expected = std::integral_constant<size_t, 0>();
		auto actual = 0_zc;
		REQUIRE(std::is_same_v<decltype(expected), decltype(actual)>);
	}
}