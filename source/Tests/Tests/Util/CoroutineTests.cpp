#ifndef _WIN32
#include "../../pch.h"

using namespace std::chrono_literals;
using namespace std::string_literals;
using namespace Library;

#define TEST(name) TEST_CASE_METHOD(MemLeak, #name, "[Concept]")

namespace UnitTests
{
	static inline int myCoroutineRunCount = 0;
	
	static Coroutine MyCoroutine()
	{
		co_yield Time::Seconds(0.5);
		myCoroutineRunCount++;
		co_return;
	}
	
	TEST(FreeFunction)
	{
		const auto oldCount = myCoroutineRunCount;
		Coroutines::Start(MyCoroutine);
		while (Coroutines::Count())
		{
			Engine::Update();
		}
		REQUIRE(oldCount + 1 == myCoroutineRunCount);
	}

	TEST(Start)
	{
		int count = 0;
		Coroutines::Start([&count]()->Coroutine
		{
			count++;
			// you can yield a Time::Seconds literal
			co_yield 0.5s;
			count++;
			// you can yield a Time::Millis literal
			co_yield 500ms;
			count++;
			co_return;
		});
		
		Engine::Update();
		REQUIRE(1 == count);

		Engine::Update();
		REQUIRE(1 == count);

		std::this_thread::sleep_for(900ms);
		Engine::Update();

		REQUIRE(2 == count);

		while (Coroutines::Count())
		{
			Engine::Update();
		}
		REQUIRE(3 == count);
	}

	TEST(LossOfScope)
	{
		int count = 0;
		// Braces to see what happens when the lambda goes out of scope.
		{
			float wait = 0.5f;
			Coroutines::Start([&count, wait]() mutable ->Coroutine
			{
				wait = 0.5f;
				count++;
				co_yield Time::Seconds(wait);
				count++;
				co_return;
			});
		}
		// Make sure that stack frame is nice and zeroed.
		int dummy[1000]{};
		static int dummySum{};
		// Doing a sum over the dummy memory into static memory to make sure the compiler can't optimize it out.
		for (const auto d : dummy)
		{
			dummySum += d;
		}

		Engine::Update();
		REQUIRE(1 == count);

		Engine::Update();
		REQUIRE(1 == count);

		while (Coroutines::Count())
		{
			Engine::Update();
		}
		REQUIRE(2 == count);
	}

	TEST(Exception)
	{
		Coroutines::Start([]()->Coroutine
		{
			co_yield 1ns;
			throw std::exception();
		});
		Engine::Update();
		REQUIRE_THROWS_AS([]
		{
			while (Coroutines::Count())
			{
				Engine::Update();
			}
		}(), AggregateException);

		Coroutines::Start([]()->Coroutine
		{
			co_yield 1ns;
			throw std::exception();
		}, true);
		REQUIRE_THROWS_AS([]
		{
			while (Coroutines::Count())
			{
				Engine::Update();
			}
		}(), AggregateException);
	}

	TEST(StopWithinCoroutine)
	{
		Coroutines::Start("co", []()->Coroutine
		{
			Coroutines::Stop("co");
			co_return;
		});
		Engine::Update();
		Engine::Update();
		REQUIRE(0 == Coroutines::Count());
	}

#pragma region async
	TEST(AsyncOneThread)
	{
		int count = 0;
		Coroutines::Start([&count]()->Coroutine
		{
			count++;
			co_return;
		}, true);

		while (Coroutines::Count())
		{
			Engine::Update();
		}
		REQUIRE(1 == count);
	}

	TEST(AsyncAtomicIncrement)
	{
		std::atomic<int> count{};
		for (size_t i = 0; i < 1000; i++)
		{
			Coroutines::Start([&count]()->Coroutine
			{
				++count;
				co_return;
			}, true);
		}

		while (Coroutines::Count())
		{
			Engine::Update();
		}
		REQUIRE(1000 == count.load());
	}

	TEST(AsyncSort)
	{
		Array<Array<int>> arrays{ 100 };
		for (auto& array : arrays)
		{
			array = Library::Random::Next<Array<int>>(1000);
			Coroutines::Start([&array]()->Coroutine
			{
				std::sort(array.begin(), array.end());
				co_return;
			}, true);
		}

		while (Coroutines::Count())
		{
			Engine::Update();
		}

		// using ints to prevent underflow
		for (int i = int(arrays.Size()) - 1; i >= 0; i--)
		{
			auto& v = arrays[i];
			REQUIRE(std::is_sorted(v.begin(), v.end()));
		}
	}
#pragma endregion
}
#endif
