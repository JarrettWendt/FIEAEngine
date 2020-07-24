#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Memory::" #name, "[Memory]")

namespace UnitTests
{
	TEST(Malloc)
	{
		REQUIRE(!Memory::Malloc(0));
	}
}
