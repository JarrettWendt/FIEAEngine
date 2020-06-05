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
	TEST_CLASS(FooTests)
	{
		INITIALIZE_CLEANUP

		TEST_METHOD(HasID)
		{
			const Foo f;
			const RTTI* r = f.As<RTTI>();
			Assert::IsFalse(r->HasID(std::numeric_limits<RTTI::IDType>::max()));
		}
		
		TEST_METHOD(Is)
		{
			const Foo f;
			
			Assert::IsTrue(f.Is<RTTI>());
			
			const RTTI* r = f.As<RTTI>();
			
			Assert::IsTrue(r->Is<Foo>());
		}

		TEST_METHOD(As)
		{
			const Foo f;
			Assert::IsNotNull(f.As<Foo>());
			Assert::IsNotNull(f.As<RTTI>());
		}

		TEST_METHOD(AssertAs)
		{
			const Foo f;
			Assert::IsNotNull(&f.AssertAs<Foo>());
			Assert::IsNotNull(&f.AssertAs<RTTI>());
		}
	};
}
