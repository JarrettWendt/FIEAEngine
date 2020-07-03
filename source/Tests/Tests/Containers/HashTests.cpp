#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

namespace UnitTests
{
	TEMPLATE_TEST_CASE("hashing", "[Hash]", char, uint16_t, size_t, float, double, long, int64_t)
	{
		auto hash = Hash<TestType>();

		const auto a = Random::Next<TestType>();

		REQUIRE(hash(a) == hash(a));

		auto b = Random::NotEqualTo<TestType>(a);
		REQUIRE(hash(a) != hash(b));

		const TestType a1 = a;

		REQUIRE(hash(a) == hash(a1));
	}

	TEMPLATE_TEST_CASE("hashing c-strings", "[Hash]", char*, const char*, char* const, const char* const)
	{
		auto hash = Hash<TestType>();

		const auto strA = Random::Next<std::string>();
		const auto strB = Random::NotEqualTo(strA);
		TestType a = Util::UnionCast<TestType>(strA.c_str());
		TestType b = Util::UnionCast<TestType>(strB.c_str());
		
		REQUIRE(hash(a) == hash(a));		
		REQUIRE(hash(a) != hash(b));

		const TestType a1 = a;

		REQUIRE(hash(a) == hash(a1));
	}
}
