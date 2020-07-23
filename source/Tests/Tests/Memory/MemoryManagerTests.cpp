#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;

#define TEST(name) TEST_CASE("Memory::Manager::" #name, "[Memory::Manager]")

namespace UnitTests
{
	using SharedString = SharedPtr<std::string>;
	using SharedEntity = SharedPtr<Entity>;
	
	TEST(std::string)
	{
		std::string* addr;
		{
			auto a = SharedString::Make("hello");
			REQUIRE(*a == "hello");
			addr = &*a;
		}
		Memory::Manager::Defrag();

#ifdef _DEBUG
		REQUIRE(*reinterpret_cast<std::uint16_t*>(addr) == std::uint16_t(0xF1EA));
#endif
		
		{
			auto a = SharedString::Make("hello");
			REQUIRE(*a == "hello");
			REQUIRE(&*a == addr);
		}
		Memory::Manager::Defrag();
	}

	TEST(ShuffleBackwards)
	{
		auto shared = SharedString::Make("hello");
		std::string* addr = &*shared;
		{
			SharedString other = shared;
			shared = SharedString::Make("some new string");
		}
		Memory::Manager::Defrag();

		REQUIRE(*shared == "some new string");
		REQUIRE(addr == &*shared);
	}
}
