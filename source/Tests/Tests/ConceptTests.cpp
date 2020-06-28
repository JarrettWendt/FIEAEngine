#include "../pch.h"

using namespace std::string_literals;
using namespace Library;

#define TEST(name) TEST_CASE_METHOD(MemLeak, name, "[Concept]")

namespace UnitTests
{
#pragma region std
	TEST("std::integral")
	{
		REQUIRE(std::integral<int>);
		REQUIRE(!std::integral<float>);
	}

	TEST("std::predicate")
	{
		REQUIRE(std::predicate<std::equal_to<int>, int, int>);
	}

	TEST("std::equality_comparable")
	{
		REQUIRE(std::equality_comparable<int>);
		REQUIRE(!std::equality_comparable<std::function<void()>>);
	}
#pragma endregion

	TEST("has_value_type")
	{
		REQUIRE(Concept::has_value_type<Array<int>>);
		REQUIRE(!Concept::has_value_type<int>);
	}

	TEST("Iterator")
	{
		REQUIRE(Concept::Iterator<std::vector<int>::iterator>);
		REQUIRE(!Concept::Iterator<int>);
		REQUIRE(Concept::Iterator<int*>);
	}
	
	TEST("RangeOf")
	{
		REQUIRE(Concept::RangeOf<std::vector<int>, int>);
		REQUIRE(!Concept::RangeOf<int, int>);
	}

	TEST("ReserveStrategy")
	{
		auto lambda = []() {};
		REQUIRE(Concept::ReserveStrategy<Util::DefaultReserveStrategy>);
		REQUIRE(Concept::ReserveStrategy<Util::PrimeReserveStrategy>);
		REQUIRE(!Concept::ReserveStrategy<decltype(lambda)>);
	}

	TEST("Hasher")
	{
		REQUIRE(Concept::Hasher<Hash<int>, int>);
		REQUIRE(!Concept::Hasher<Hash<std::string>, int>);
		REQUIRE(Concept::Hasher<std::hash<int>, int>);
		REQUIRE(!Concept::Hasher<std::hash<std::string>, int>);
	}

	TEST("HasSize")
	{
		REQUIRE(Concept::has_size<std::vector<int>>);
		REQUIRE(!Concept::has_Size<std::vector<int>>);
		REQUIRE(Concept::HasSize<std::vector<int>>);
		
		REQUIRE(Concept::has_Size<Array<int>>);
		REQUIRE(!Concept::has_size<Array<int>>);
		REQUIRE(Concept::HasSize<Array<int>>);
	}
}
