#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Quaternion::" #name, "[Quaternion]")

namespace UnitTests
{
	TEST(Vector3 + w ctor)
	{
		Quaternion v(4, { 1, 2, 3 });
		REQUIRE(v == Quaternion{ 1, 2, 3, 4 });
	}
	
	TEST(operator[](std::integral_constant))
	{
		Quaternion v = { 1, 2, 3, 4 };
		REQUIRE(v[0_zc] == 1);
	}

	TEST(operator[])
	{
		const Quaternion v{ 1, 2, 3, 4 };
		REQUIRE(v[0] == v.x);
		REQUIRE(v[1] == v.y);
	}

	TEST(operator+(float))
	{
		REQUIRE((Quaternion::One + 1) == (1 + Quaternion::One));
	}

	TEST(operator-(float))
	{
		REQUIRE((Quaternion::One - 1) == (1 - Quaternion::One));
	}

	TEST(operator*(float))
	{
		REQUIRE((Quaternion::One * 1) == (1 * Quaternion::One));
	}

	TEST(operator/(float))
	{
		REQUIRE((Quaternion::One / 1) == (1 / Quaternion::One));
	}

	TEST(operator+=(float))
	{
		Quaternion v = Quaternion::Zero;
		v += 1;
		REQUIRE(v == Quaternion::One);
	}

	TEST(operator-=(float))
	{
		Quaternion v = Quaternion::Zero;
		v -= 1;
		REQUIRE(v == -Quaternion::One);
	}

	TEST(operator*=(float))
	{
		Quaternion v = Quaternion::One;
		v *= 2;
		REQUIRE(v == Quaternion{ 2, 2, 2, 2 });
	}

	TEST(operator/=(float))
	{
		Quaternion v = Quaternion::One;
		v /= 2;
		REQUIRE(v == Quaternion{ 0.5, 0.5, 0.5, 0.5 });
	}

	TEST(operator+=)
	{
		Quaternion v = Quaternion::Zero;
		v += Quaternion::One;
		REQUIRE(v == Quaternion::One);
	}

	TEST(operator-=)
	{
		Quaternion v = Quaternion::Zero;
		v -= Quaternion::One;
		REQUIRE(v == -Quaternion::One);
	}

	TEST(operator*=)
	{
		Quaternion v = Quaternion::Zero;
		v *= 2 * Quaternion::One;
		REQUIRE(v == Quaternion::Zero);
	}

	TEST(operator/=)
	{
		Quaternion v = Quaternion::One;
		v /= { 2, 2, 2, 2 };
		REQUIRE(v != Quaternion{ 0.5, 0.5, 0.5, 0.5 });
	}

	TEST(operator!=)
	{
		const Quaternion a{ 1, 2, 3, 4 };
		REQUIRE(a != Quaternion{});
	}

	TEST(operator<<)
	{
		const Quaternion v{ 2, 3, 4, 5 };
		std::stringstream stream;
		stream << v;
		REQUIRE(stream.str() == "{2, 3, 4, 5}");
	}
}
