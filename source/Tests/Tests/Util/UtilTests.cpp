#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;

#define TEST(name) TEST_CASE_METHOD(MemLeak, name, "[Util]")

namespace UnitTests
{
	TEST("DefaultReserveStrategy")
	{
	    REQUIRE(size_t(7 * 1.5) == Util::DefaultReserveStrategy{}(7, 7));
	}

	TEST("IsOneOf")
	{
		REQUIRE(Util::IsOneOf<int, bool, float, std::string, int>());
		REQUIRE(!Util::IsOneOf<int, bool, float, std::string, double>());
	}

	TEST("FirstConvertible")
	{
		REQUIRE(std::is_same_v<Util::FirstConvertible<int, bool, float, std::string>, bool>);
		REQUIRE(std::is_same_v<Util::FirstConvertible<int, std::string, bool, float>, bool>);
	}

	TEST("BestMatch")
	{
		REQUIRE(std::is_same_v<Util::BestMatch<int, bool, float, std::string, int>, int>);
		REQUIRE(std::is_same_v<Util::BestMatch<int, bool, float, std::string>, bool>);
	}

#pragma region String
	TEST("std::to_string")
	{
		REQUIRE(std::to_string('c') == "c");
		REQUIRE(std::to_string(L"c") == "c");
		REQUIRE(std::to_wstring("c"s) == L"c"s);
	}
	
	TEST("PairToString")
	{
		std::pair<int, int> p = { 6, 7 };
		REQUIRE("{ 6, 7 }" == std::to_string(p));
	}

	TEST("IsEmptyOrWhitespace")
	{
		REQUIRE(Util::IsEmptyOrWhitespace(""));
		REQUIRE(Util::IsEmptyOrWhitespace(" "));
		REQUIRE(!Util::IsEmptyOrWhitespace("a"));
		REQUIRE(!Util::IsEmptyOrWhitespace(" a"));
	}

	TEST("IsWhitespace")
	{
		REQUIRE(!Util::IsWhitespace(""));
		REQUIRE(Util::IsWhitespace(" "));
		REQUIRE(!Util::IsEmptyOrWhitespace("a"));
		REQUIRE(!Util::IsEmptyOrWhitespace(" a"));
	}

	TEST("HasAlpha")
	{
		REQUIRE(!Util::HasAlpha(""));
		REQUIRE(!Util::HasAlpha(" "));
		REQUIRE(Util::HasAlpha(" a"));
		REQUIRE(Util::HasAlpha(" ;a"));
	}

	TEST("ToLower")
	{
		REQUIRE(Util::ToLower("").empty());
		REQUIRE(Util::ToLower(" ") == " ");
		REQUIRE(Util::ToLower("Hello, World!") == "hello, world!");
		REQUIRE(Util::ToLower("hello, world!") == "hello, world!");
	}

	TEST("ToUpper")
	{
		REQUIRE(Util::ToUpper("").empty());
		REQUIRE(Util::ToUpper(" ") == " ");
		REQUIRE(Util::ToUpper("Hello, World!") == "HELLO, WORLD!");
		REQUIRE(Util::ToUpper("HELLO, WORLD!") == "HELLO, WORLD!");
	}

	TEST("IsLower")
	{
		REQUIRE(!Util::IsLower(""));
		REQUIRE(!Util::IsLower(" "));
		REQUIRE(Util::IsLower("a"));
		REQUIRE(!Util::IsLower(" A"));
		REQUIRE(!Util::IsLower("Hello, World!"));
		REQUIRE(Util::IsLower("hello, world!"));
	}

	TEST("IsUpper")
	{
		REQUIRE(!Util::IsUpper(""));
		REQUIRE(!Util::IsUpper(" "));
		REQUIRE(!Util::IsUpper("a"));
		REQUIRE(Util::IsUpper(" A"));
		REQUIRE(!Util::IsUpper("Hello, World!"));
		REQUIRE(Util::IsUpper("HELLO, WORLD!"));
	}

	TEST("RemoveWhitespace")
	{
		REQUIRE(Util::RemoveWhitespace("").empty());
		REQUIRE(Util::RemoveWhitespace(" ").empty());
		REQUIRE(Util::RemoveWhitespace(" Hello, World!") == "Hello,World!");
		REQUIRE(Util::RemoveWhitespace(" HELLO, WORLD!") == "HELLO,WORLD!");
	}
#pragma endregion
}
