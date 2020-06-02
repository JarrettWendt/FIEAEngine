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
	TEST_CLASS(ReflectionTests)
	{
		template<typename T>
		using Container = SList<T>;

		INITIALIZE_CLEANUP

		TEST_METHOD(ConstructScope)
		{
			const auto s = Reflection::Construct<Scope>("Scope");
			Assert::IsFalse(s == nullptr);
			Assert::IsTrue(s->Is<Scope>());

			// just doing something to make sure we don't have a memory leak
			s->Insert("child");
		}
	};
}
