#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "WeakPtr::" #name, "[WeakPtr]")

namespace UnitTests
{
	TEST(ctor)
	{
		WeakPtr<int> weak;
		REQUIRE(!weak);
		
		SharedPtr<int> shared = SharedPtr<int>::Make();
		weak = shared;

		REQUIRE(weak.ReferenceCount() == 1);
		REQUIRE(weak.Raw() == shared.Raw());
	}

	TEST(operator SharedPtr)
	{
		SharedPtr<int> a = SharedPtr<int>::Make();
		WeakPtr<int> weak = a;
		SharedPtr<int> b = weak;

		REQUIRE(b.ReferenceCount() == 2);
		REQUIRE(b.Raw() == a.Raw());
	}

	TEST(Expired)
	{
		WeakPtr<int> weak;
		REQUIRE(weak.Expired());
		{
			const SharedPtr<int> shared = SharedPtr<int>::Make();
			weak = shared;
			REQUIRE(!weak.Expired());
		}
		REQUIRE(weak.Expired());
	}
}
