#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Transform::" #name, "[Transform]")

namespace UnitTests
{
	TEST(operator+=)
	{
		Transform a, b;
		a += b;
		REQUIRE(a == b);
	}
	
	TEST(operator-=)
	{
		Transform a, b;
		a -= b;
		REQUIRE(a == b);
	}
	
	TEST(operator!=)
	{
		Transform a, b;
		b.scale += 1;
		REQUIRE(a != b);
	}
	
	TEST(operator<<)
	{
		const Transform t;
		std::stringstream stream;
		stream << t;
		REQUIRE(stream.str() == "t: <0, 0, 0>, r: {0, 0, 0, 1}, s: <1, 1, 1>");
	}
}
