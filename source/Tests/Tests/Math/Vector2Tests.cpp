#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Vector2::" #name, "[Vector2]")

namespace UnitTests
{
	TEST(operator[](std::integral_constant))
	{
		Vector2 v{ 1, 2 };
		REQUIRE(v[0_zc] == 1);
	}

	TEST(operator[])
	{
		const Vector2 v{ 1, 2 };
		REQUIRE(v[0] == v.x);
		REQUIRE(v[1] == v.y);
	}

	TEST(operator!=)
	{
		const Vector2 a{ 1, 2 };
		REQUIRE(a != Vector2{});
	}

	TEST(operator<<)
	{
		const Vector2 v{ 2, 3 };
		std::stringstream stream;
		stream << v;
		REQUIRE(stream.str() == "<2, 3>");
	}
}
