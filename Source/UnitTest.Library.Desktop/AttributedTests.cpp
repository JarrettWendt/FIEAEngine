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
	TEST_CLASS(AttributedTests)
	{
		INITIALIZE_CLEANUP

		TEST_METHOD(Lorem)
		{
			std::weak_ptr<AttributedFoo> ptr;
			{
				const std::shared_ptr<AttributedFoo> foo = std::make_shared<AttributedFoo>();
				ptr = foo;
			}
			Assert::IsTrue(ptr.expired());
		}
	};
}
