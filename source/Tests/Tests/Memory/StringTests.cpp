#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "String::" #name, "[String]")

namespace UnitTests
{
	TEST(NumInterned)
	{
		const size_t oldNumInterned = String::NumInterned();
		const String a = "hello, world!"_s;
		const String b = "hello, world!";
		REQUIRE(String::NumInterned() == oldNumInterned + 1);
	}

	TEST(wchar_t)
	{
		REQUIRE(L"hello, world"_s == "hello, world");
		REQUIRE(String(L"hello") == "hello"_s);
	}

#pragma region properties
	TEST(IsEmpty)
	{
		REQUIRE(""_s.IsEmpty());
		REQUIRE(!" "_s.IsEmpty());
	}

	TEST(IsWhitespace)
	{
		REQUIRE(!""_s.IsWhitespace());
		REQUIRE(" "_s.IsWhitespace());
		REQUIRE(!"_"_s.IsWhitespace());
		REQUIRE(!" _"_s.IsWhitespace());
	}

	TEST(IsEmptyOrWhitespace)
	{
		REQUIRE(" "_s.IsEmptyOrWhitespace());
		REQUIRE(""_s.IsEmpty());
	}

	TEST(HasAlpha)
	{
		REQUIRE(!""_s.HasAlpha());
		REQUIRE("a"_s.HasAlpha());
		REQUIRE(!"1"_s.HasAlpha());
		REQUIRE("1a"_s.HasAlpha());
	}

	TEST(IsLower)
	{
		REQUIRE(!""_s.IsLower());
		REQUIRE("a"_s.IsLower());
		REQUIRE(!"A"_s.IsLower());
	}

	TEST(IsUpper)
	{
		REQUIRE(!""_s.IsUpper());
		REQUIRE(!"a"_s.IsUpper());
		REQUIRE("A"_s.IsUpper());
	}
#pragma endregion

#pragma region util
	TEST(ToLower)
	{
		REQUIRE("HELLO"_s.ToLower() == "hello");
	}

	TEST(ToUpper)
	{
		REQUIRE("hello"_s.ToUpper() == "HELLO");
	}

	TEST(RemoveWhitespace)
	{
		REQUIRE(" "_s.RemoveWhitespace().IsEmpty());
		REQUIRE("hello, world!"_s.RemoveWhitespace() == "hello,world!");
	}

	TEST(ReplaceAll)
	{
		REQUIRE("hello, world!"_s.ReplaceAll("l", "1") == "he11o, wor1d!");
	}
#pragma endregion

#pragma region element access
	TEST(operator[])
	{
		REQUIRE("hello"_s[0] == 'h');
	}

	TEST(At)
	{
		REQUIRE("hello"_s.At(0) == 'h');
		REQUIRE_THROWS_AS("hello"_s.At(100), std::out_of_range);
	}

	TEST(Front)
	{
		REQUIRE("hello"_s.Front() == 'h');
	}

	TEST(Back)
	{
		REQUIRE("hello"_s.Back() == 'o');
	}
#pragma endregion

#pragma region iterator
	TEST(cbegin)
	{
		REQUIRE(*"hello"_s.cbegin() == 'h');
	}

	TEST(cend)
	{
		REQUIRE(*("hello"_s.cend() - 1) == 'o');
	}
#pragma endregion
	
	TEST(str)
	{
		REQUIRE(""s == ""_s.str());
		REQUIRE(std::is_same_v<const std::string&, decltype(""_s.str())>);
	}

	TEST(c_str)
	{
		REQUIRE(!std::strcmp("", ""_s.c_str()));
		REQUIRE(std::is_same_v<const char*, decltype(""_s.c_str())>);
	}

#pragma region operators
	TEST(operator+=(const char*))
	{
		String s = "hello";
		s += ", world!";
		REQUIRE(s == "hello, world!");
	}

	TEST(operator+=(String))
	{
		String s = "hello"_s;
		s += ", world!"_s;
		REQUIRE(s == "hello, world!");
	}

	TEST(operator<)
	{
		REQUIRE("a"_s < "z"_s);
	}

	TEST(operator<<)
	{
		std::stringstream stream;
		stream << "hello"_s;
		REQUIRE("hello"_s == stream.str());
	}
#pragma endregion
}
