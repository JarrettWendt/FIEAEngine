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
	TEST_CLASS(MathTests)
	{
		template<typename T>
		using Container = SList<T>;

		INITIALIZE_CLEANUP

		TEST_METHOD(Mod)
		{
			Assert::AreEqual(1, Math::Mod(1, 10));
			Assert::AreEqual(9, Math::Mod(-1, 10));
			Assert::AreEqual(-9, Math::Mod(1, -10));
		}
	};
}
