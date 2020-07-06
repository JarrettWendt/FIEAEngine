#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Vector4::" #name, "[Vector4]")

namespace UnitTests
{
	TEST(Vector3 + w ctor)
	{
		Vector4 v(4, { 1, 2, 3 });
		REQUIRE(v == Vector4{ 1, 2, 3, 4 });
	}
	
	TEST(operator[](std::integral_constant))
	{
		Vector4 v = { 1, 2, 3, 4 };
		REQUIRE(v[0_zc] == 1);
	}

	TEST(operator[])
	{
		const Vector4 v{ 1, 2, 3, 4 };
		REQUIRE(v[0] == v.x);
		REQUIRE(v[1] == v.y);
	}

	TEST(operator+(float))
	{
		REQUIRE((Vector4::One + 1) == (1 + Vector4::One));
	}

	TEST(operator-(float))
	{
		REQUIRE((Vector4::One - 1) == (1 - Vector4::One));
	}

	TEST(operator*(float))
	{
		REQUIRE((Vector4::One * 1) == (1 * Vector4::One));
	}

	TEST(operator/(float))
	{
		REQUIRE((Vector4::One / 1) == (1 / Vector4::One));
	}

	TEST(operator+=(float))
	{
		Vector4 v;
		v += 1;
		REQUIRE(v == Vector4::One);
	}

	TEST(operator-=(float))
	{
		Vector4 v;
		v -= 1;
		REQUIRE(v == -Vector4::One);
	}

	TEST(operator*=(float))
	{
		Vector4 v = Vector4::One;
		v *= 2;
		REQUIRE(v == Vector4{ 2, 2, 2, 2 });
	}

	TEST(operator/=(float))
	{
		Vector4 v = Vector4::One;
		v /= 2;
		REQUIRE(v == Vector4{ 0.5, 0.5, 0.5, 0.5 });
	}

	TEST(operator+=)
	{
		Vector4 v;
		v += Vector4::One;
		REQUIRE(v == Vector4::One);
	}

	TEST(operator-=)
	{
		Vector4 v;
		v -= Vector4::One;
		REQUIRE(v == -Vector4::One);
	}

	TEST(operator*=)
	{
		Vector4 v;
		v *= 2 * Vector4::One;
		REQUIRE(v == Vector4::Zero);
	}

	TEST(operator/=)
	{
		Vector4 v = Vector4::One;
		v /= { 2, 2, 2, 2 };
		REQUIRE(v == Vector4{ 0.5, 0.5, 0.5, 0.5 });
	}

	TEST(operator!=)
	{
		const Vector4 a{ 1, 2, 3, 4 };
		REQUIRE(a != Vector4{});
	}

	TEST(operator<<)
	{
		const Vector4 v{ 2, 3, 4, 5 };
		std::stringstream stream;
		stream << v;
		REQUIRE(stream.str() == "<2, 3, 4, 5>");
	}
}
