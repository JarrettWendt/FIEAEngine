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

	TEST(DifferentAlignment)
	{
		{
			auto _8 = SharedPtr<uint8_t>::Make();
			// 1 pad byte
			auto _16 = SharedPtr<uint16_t>::Make();
			// 2 pad bytes
			auto _32 = SharedPtr<uint32_t>::Make();
			// 4 pad bytes
			auto _64 = SharedPtr<uint64_t>::Make();
			// 8 pad bytes
			auto _128 = SharedPtr<uint128_t>::Make();
		}
		Memory::Manager::Defrag();
		REQUIRE(Memory::Manager::IsEmpty());

		{
			auto _8 = SharedPtr<uint8_t>::Make();
			// 1 pad byte
			auto _16 = SharedPtr<uint16_t>::Make();
			// 4 pad bytes
			auto _128 = SharedPtr<uint128_t>::Make();

			_16 = nullptr;
			Memory::Manager::Defrag();
		}
	}

	TEST(TotalBytes)
	{
		REQUIRE(Memory::Manager::TotalBytes() == 1024 + 2048);
	}

	TEST(ShrinkToFit)
	{
		std::deque<SharedPtr<uint128_t>> bloat{};
		while (Memory::Manager::TotalBytes() <= 1024 + 2048)
		{
			bloat.push_back(SharedPtr<uint128_t>::Make());
		}
		REQUIRE(Memory::Manager::TotalBytes() == 1024 + 2048 + 4096);
		
		bloat.clear();
		Memory::Manager::ShrinkToFit();
		REQUIRE(Memory::Manager::TotalBytes() == 1024 + 2048);
		REQUIRE(Memory::Manager::IsEmpty());
	}
		
	struct Big
	{
		uint64_t _512[8];
		
		friend bool operator==(const Big& a, const Big& b) noexcept
		{
			return std::memcmp(&a, &b, sizeof(Big));
		}
		friend bool operator!=(const Big& a, const Big& b) noexcept
		{
			return !operator==(a, b);
		}
	};
	
	TEST(Graduate)
	{
		using T = uint64_t;
		
		std::deque<SharedPtr<T>> bloat{};
		std::deque<T> copies{};

		auto areEqual = [&bloat, &copies]
		{
			auto it = bloat.begin();
			auto jt = copies.begin();
			while (jt != copies.end())
			{
				REQUIRE(**it++ == *jt++);
			}
		};
		
		// allocation
		{
			for (T t{}; Memory::Manager::TotalBytes() <= 1024 + 2048; ++t)
			{
				copies.push_back(t);
				bloat.push_back(SharedPtr<T>::Make(t));
			}
			REQUIRE(Memory::Manager::TotalBytes() == 1024 + 2048 + 4096);
			areEqual();
		}

		// remove last
		{
			bloat.pop_back();
			copies.pop_back();
			Memory::Manager::ShrinkToFit();
			areEqual();
			REQUIRE(Memory::Manager::TotalBytes() == 1024 + 2048);
		}

		// graduate
		{
			Memory::Manager::Graduate();
			areEqual();
			REQUIRE(Memory::Manager::TotalBytes() == 1024 + 2048 + 4096);
		}
		
		// clear
		{
			bloat.clear();
			copies.clear();
			Memory::Manager::ShrinkToFit();
			areEqual();
			REQUIRE(Memory::Manager::TotalBytes() == 1024 + 2048);
			REQUIRE(Memory::Manager::IsEmpty());
		}
	}

	TEST(GraduateEmpty)
	{
		REQUIRE(Memory::Manager::IsEmpty());
		Memory::Manager::Graduate();
		REQUIRE(Memory::Manager::IsEmpty());
	}
}
