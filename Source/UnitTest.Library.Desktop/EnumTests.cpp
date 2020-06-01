// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;

template<typename T>
using Rand = UnitTests::Random<T>;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EnumTests)
	{
		template<typename T>
		using Container = SList<T>;

		INITIALIZE_CLEANUP

		TEST_METHOD(RangeBasedForLoop)
		{
			int i = 0;
			for (Digit d : Enum<Digit>())
			{
				Assert::AreEqual(Digit(i++), d);
				Assert::AreNotEqual(Digit(10), d);
			}
		}

		TEST_METHOD(ForLoopInMiddle)
		{
			Enum<Digit>::iterator it = Digit::Five;
			for (int i = 5; it; ++it)
			{
				Assert::AreEqual(Digit(i++), *it);
				Assert::AreNotEqual(Digit(10), *it);
			}
			Assert::ExpectException<std::runtime_error>([&it] { *it; });
		}

		TEST_METHOD(BoolConversion)
		{
			Enum<Digit>::iterator it = Digit::End;
			Assert::IsTrue(it);
			Assert::IsFalse(!it++);
			Assert::IsFalse(it);
			Assert::IsTrue(!it);
		}

		TEST_METHOD(EnumToString)
		{
			Assert::AreEqual("Zero"s, Enum<Digit>::ToString(Digit::Zero));
			Assert::AreEqual("One"s, Enum<Digit>::ToString(Digit::One));
			Assert::AreEqual("Two"s, Enum<Digit>::ToString(Digit::Two));
			Assert::AreEqual("Three"s, Enum<Digit>::ToString(Digit::Three));
			Assert::AreEqual("Four"s, Enum<Digit>::ToString(Digit::Four));
			Assert::AreEqual("Five"s, Enum<Digit>::ToString(Digit::Five));
			Assert::AreEqual("Six"s, Enum<Digit>::ToString(Digit::Six));
			Assert::AreEqual("Seven"s, Enum<Digit>::ToString(Digit::Seven));
			Assert::AreEqual("Eight"s, Enum<Digit>::ToString(Digit::Eight));
			Assert::AreEqual("Nine"s, Enum<Digit>::ToString(Digit::Nine));
		}

		TEST_METHOD(EnumFromString)
		{
			Assert::ExpectException<std::invalid_argument>([] { Enum<Digit>::FromString(""); });
			Assert::AreEqual(Digit::Zero, Enum<Digit>::FromString("Zero"));
			Assert::AreEqual(Digit::Zero, Enum<Digit>::FromString("zero"));
			Assert::AreEqual(Digit::Zero, Enum<Digit>::FromString("Zero "));
		}

		TEST_METHOD(StdToString)
		{
			Assert::AreEqual("Zero", std::to_string(Digit::Zero));
			Assert::AreEqual("One", std::to_string(Digit::One));
			Assert::AreEqual("Two", std::to_string(Digit::Two));
			Assert::AreEqual("Three", std::to_string(Digit::Three));
			Assert::AreEqual("Four", std::to_string(Digit::Four));
			Assert::AreEqual("Five", std::to_string(Digit::Five));
			Assert::AreEqual("Six", std::to_string(Digit::Six));
			Assert::AreEqual("Seven", std::to_string(Digit::Seven));
			Assert::AreEqual("Eight", std::to_string(Digit::Eight));
			Assert::AreEqual("Nine", std::to_string(Digit::Nine));
		}

		TEST_METHOD(Overflow)
		{
			Digit d = Digit::Nine;
			++d;
			Assert::AreEqual(Digit::Zero, d);
		}

		TEST_METHOD(UnsignedUnderflow)
		{
			UnsignedDigit d = UnsignedDigit::Zero;
			--d;
			Assert::AreEqual(UnsignedDigit::Nine, d);
		}

		TEST_METHOD(SignedUnderflow)
		{
			SignedDigit d = SignedDigit::Zero;
			--d;
			Assert::AreEqual(SignedDigit::Nine, d);
		}
	};
}
