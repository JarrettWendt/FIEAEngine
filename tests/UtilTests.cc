/**
 * Copyright (c) 2019-2022 Jarrett Wendt
 */

#include <type_traits>

#include "catch_amalgamated.hpp"

#include "util.hh"

using namespace fiea;

TEST_CASE("operator""_z", "[Util]")
{
	REQUIRE(std::is_same_v<decltype(99_z), size_t>);
	REQUIRE(99_z == size_t(99));
}

TEST_CASE("operator""_ptrdiff", "[Util]")
{
	REQUIRE(std::is_same_v<decltype(99_ptrdiff), ptrdiff_t>);
	REQUIRE(99_ptrdiff == ptrdiff_t(99));
}

TEST_CASE("IsWhitespace", "[Util]")
{
	REQUIRE_FALSE(IsWhitespace("a"));
	REQUIRE_FALSE(IsWhitespace(""));
	REQUIRE(IsWhitespace(" "));
	REQUIRE(IsWhitespace("\t"));
	REQUIRE(IsWhitespace("\n"));
	REQUIRE(IsWhitespace("\r"));
}

TEST_CASE("IsEmptyOrWhitespace", "[Util]")
{
	REQUIRE_FALSE(IsEmptyOrWhitespace("a"));
	REQUIRE(IsEmptyOrWhitespace(""));
	REQUIRE(IsEmptyOrWhitespace(" "));
	REQUIRE(IsEmptyOrWhitespace("\t"));
	REQUIRE(IsEmptyOrWhitespace("\n"));
	REQUIRE(IsEmptyOrWhitespace("\r"));
}
