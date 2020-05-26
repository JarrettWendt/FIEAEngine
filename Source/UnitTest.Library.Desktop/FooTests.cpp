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
		struct MyRTTI : public RTTI
		{
			RTTI_DECLARATIONS(RTTI)
		};

		INITIALIZE_CLEANUP

		TEST_METHOD(Is)
		{
			const Foo f;
			const RTTI* rtti = f.As<RTTI>();
			Assert::IsTrue(rtti->Is<Foo>());
			Assert::IsFalse(rtti->Is(std::numeric_limits<size_t>::max()));
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

		TEST_METHOD(Equals)
		{
			Assert::IsFalse(MyRTTI().Equals(MyRTTI()));
		}
	};
}
