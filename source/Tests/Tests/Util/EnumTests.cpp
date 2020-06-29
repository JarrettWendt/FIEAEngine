#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;

#define TEST(name) TEST_CASE_METHOD(MemLeak, #name, "[Util]")

namespace UnitTests
{
	TEST(RangeBasedForLoop)
	{
		int i = 0;
		for (Digit d : Enum<Digit>())
		{
			REQUIRE(Digit(i++) == d);
			REQUIRE(Digit(10) != d);
		}
	}

	TEST(ForLoopInMiddle)
	{
		Enum<Digit>::iterator it = Digit::Five;
		for (int i = 5; it; ++it)
		{
			REQUIRE(Digit(i++) == *it);
			REQUIRE(Digit(10) != *it);
		}
		REQUIRE_THROWS_AS(*it, std::runtime_error);
	}

	TEST(BoolConversion)
	{
		Enum<Digit>::iterator it = Digit::End;
		REQUIRE(it);
		REQUIRE(!!it++);
		REQUIRE(!it);
		REQUIRE(!it);
	}

	TEST(EnumToString)
	{
		REQUIRE("Zero"s == Enum<Digit>::ToString(Digit::Zero));
		REQUIRE("One"s == Enum<Digit>::ToString(Digit::One));
		REQUIRE("Two"s == Enum<Digit>::ToString(Digit::Two));
		REQUIRE("Three"s == Enum<Digit>::ToString(Digit::Three));
		REQUIRE("Four"s == Enum<Digit>::ToString(Digit::Four));
		REQUIRE("Five"s == Enum<Digit>::ToString(Digit::Five));
		REQUIRE("Six"s == Enum<Digit>::ToString(Digit::Six));
		REQUIRE("Seven"s == Enum<Digit>::ToString(Digit::Seven));
		REQUIRE("Eight"s == Enum<Digit>::ToString(Digit::Eight));
		REQUIRE("Nine"s == Enum<Digit>::ToString(Digit::Nine));
	}

	TEST(EnumFromString)
	{
		REQUIRE_THROWS_AS(Enum<Digit>::FromString(""), std::invalid_argument);
		REQUIRE(Digit::Zero == Enum<Digit>::FromString("Zero"));
		REQUIRE(Digit::Zero == Enum<Digit>::FromString("zero"));
		REQUIRE(Digit::Zero == Enum<Digit>::FromString("Zero "));
	}

	TEST(StdToString)
	{
		REQUIRE("Zero" == std::to_string(Digit::Zero));
		REQUIRE("One" == std::to_string(Digit::One));
		REQUIRE("Two" == std::to_string(Digit::Two));
		REQUIRE("Three" == std::to_string(Digit::Three));
		REQUIRE("Four" == std::to_string(Digit::Four));
		REQUIRE("Five" == std::to_string(Digit::Five));
		REQUIRE("Six" == std::to_string(Digit::Six));
		REQUIRE("Seven" == std::to_string(Digit::Seven));
		REQUIRE("Eight" == std::to_string(Digit::Eight));
		REQUIRE("Nine" == std::to_string(Digit::Nine));
	}

	TEST(Count)
	{
		REQUIRE(10 == Enum<Digit>::Count);
	}

	TEST(UnsignedOverflow)
	{
		using D = UnsignedDigit;
		
		D d = D::Nine;
		++d;
		REQUIRE(D::Zero == d);

		d = D::Eight;
		d += 2;
		REQUIRE(D::Zero == d);

		d = D::One;
		d += 10;
		REQUIRE(D::One == d);
	}

	TEST(UnsignedUnderflow)
	{
		using D = UnsignedDigit;
		
		D d = D::Zero;
		--d;
		REQUIRE(D::Nine == d);

		d = D::One;
		d -= 2;
		REQUIRE(D::Nine == d);

		d = D::One;
		d -= 10;
		REQUIRE(D::One == d);
	}

	TEST(SignedOverflow)
	{
		using D = SignedDigit;
		
		D d = D::Nine;
		++d;
		REQUIRE(D::Zero == d);

		d = D::Eight;
		d += 2;
		REQUIRE(D::Zero == d);

		d = D::One;
		d += 10;
		REQUIRE(D::One == d);
	}

	TEST(SignedUnderflow)
	{
		using D = SignedDigit;
		
		D d = D::Zero;
		--d;
		REQUIRE(D::Nine == d);

		d = D::One;
		d -= 2;
		REQUIRE(D::Nine == d);

		d = D::One;
		d -= 10;
		REQUIRE(D::One == d);
	}
}
