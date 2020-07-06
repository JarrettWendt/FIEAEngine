#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Vector3::" #name, "[Vector3]")

namespace UnitTests
{
	TEST(operator[](std::integral_constant))
	{
		Vector3 v = { 1, 2 };
		REQUIRE(v[0_zc] == 1);
	}

	TEST(operator[])
	{
		const Vector3 v{ 1, 2 };
		REQUIRE(v[0] == v.x);
		REQUIRE(v[1] == v.y);
	}

	TEST(operator+(float))
	{
		REQUIRE((Vector3::One + 1) == (1 + Vector3::One));
	}

	TEST(operator-(float))
	{
		REQUIRE((Vector3::One - 1) == (1 - Vector3::One));
	}

	TEST(operator*(float))
	{
		REQUIRE((Vector3::One * 1) == (1 * Vector3::One));
	}

	TEST(operator/(float))
	{
		REQUIRE((Vector3::One / 1) == (1 / Vector3::One));
	}

	TEST(operator+=(float))
	{
		Vector3 v;
		v += 1;
		REQUIRE(v == Vector3::One);
	}

	TEST(operator-=(float))
	{
		Vector3 v;
		v -= 1;
		REQUIRE(v == -Vector3::One);
	}

	TEST(operator*=(float))
	{
		Vector3 v = Vector3::One;
		v *= 2;
		REQUIRE(v == Vector3{ 2, 2, 2 });
	}

	TEST(operator/=(float))
	{
		Vector3 v = Vector3::One;
		v /= 2;
		REQUIRE(v == Vector3{ 0.5, 0.5, 0.5 });
	}

	TEST(operator+=)
	{
		Vector3 v;
		v += Vector3::One;
		REQUIRE(v == Vector3::One);
	}

	TEST(operator-=)
	{
		Vector3 v;
		v -= Vector3::One;
		REQUIRE(v == -Vector3::One);
	}

	TEST(operator*=)
	{
		Vector3 v;
		v *= 2 * Vector3::One;
		REQUIRE(v == Vector3::Zero);
	}

	TEST(operator/=)
	{
		Vector3 v = Vector3::One;
		v /= { 2, 2, 2 };
		REQUIRE(v == Vector3{ 0.5, 0.5, 0.5 });
	}

	TEST(operator!=)
	{
		const Vector3 a{ 1, 2 };
		REQUIRE(a != Vector3{});
	}

	TEST(operator<<)
	{
		const Vector3 v{ 2, 3, 4 };
		std::stringstream stream;
		stream << v;
		REQUIRE(stream.str() == "<2, 3, 4>");
	}
}
