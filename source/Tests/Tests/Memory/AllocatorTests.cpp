#include "../../pch.h"
#include "LibAllocator.h"

using namespace std::string_literals;
using namespace Library;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Allocator" #name, "[Allocator]")

namespace UnitTests
{
	TEST(std::vector)
	{
		std::vector<int, Allocator<int>> v;
		v.push_back(5);
		REQUIRE(Allocator<int>::NumAllocations() == 1);
	}
}
