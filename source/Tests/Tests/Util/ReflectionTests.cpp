#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;

#define TEST(name) TEST_CASE_METHOD(MemLeak, #name, "[Reflection]")

namespace UnitTests
{
	TEST(ConstructScope)
	{
		const auto s = Reflection::Construct<Entity>("Entity");
		REQUIRE(s != nullptr);
		REQUIRE(s->Is<Entity>());

		// just doing something to make sure we don't have a memory leak
		s->CreateChild("child"s);
	}
}
