// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"

using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;

template<typename T>
using Rand = UnitTests::Random<T>;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(CoroutineTests)
	{
		//INITIALIZE_CLEANUP

		static inline int myCoroutineRunCount = 0;
		
		static Coroutine MyCoroutine()
		{
			co_yield Time::Seconds(0.5);
			myCoroutineRunCount++;
			co_return;
		}
		
		TEST_METHOD(FreeFunction)
		{
			const auto oldCount = myCoroutineRunCount;
			Coroutines::Start(MyCoroutine);
			Engine::Update();
			std::this_thread::sleep_for(0.5s);
			Engine::Update();
			Assert::AreEqual(oldCount + 1, myCoroutineRunCount);
			Engine::Update();
		}

		TEST_METHOD(Start)
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
			Assert::AreEqual(1, count);

			Engine::Update();
			Assert::AreEqual(1, count, L"Still one because it hasn't been enough time");

			std::this_thread::sleep_for(0.5s);
			Engine::Update();

			Assert::AreEqual(2, count);

			std::this_thread::sleep_for(500ms);
			Engine::Update();
			Assert::AreEqual(3, count);

			// one more Update() to remove everything
			Engine::Update();
			Assert::AreEqual(0_z, Coroutines::Count());
		}

		TEST_METHOD(LossOfScope)
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
			Assert::AreEqual(1, count);

			Engine::Update();
			Assert::AreEqual(1, count, L"Still one because it hasn't been enough time");

			std::this_thread::sleep_for(Time::Seconds(0.5f));
			Engine::Update();
			Assert::AreEqual(2, count);

			// one more Update() to remove everything
			Engine::Update();
			Assert::AreEqual(0_z, Coroutines::Count());
		}

		TEST_METHOD(Exception)
		{
			Coroutines::Start([]()->Coroutine
			{
				co_yield 1ns;
				throw std::exception();
			});
			Engine::Update();
			Assert::ExpectException<AggregateException>([] { Engine::Update(); });

			Coroutines::Start([]()->Coroutine
			{
				co_yield 1ns;
				throw std::exception();
			}, true);
			Assert::ExpectException<AggregateException>([] { Engine::Update(); Engine::Update(); });

			// one more Update() to remove everything
			Engine::Update();
			Assert::AreEqual(0_z, Coroutines::Count());
		}

		TEST_METHOD(StopWithinCoroutine)
		{
			Coroutines::Start("co", []()->Coroutine
			{
				Coroutines::Stop("co");
				co_return;
			});
			Engine::Update();
			Engine::Update();
		}

#pragma region async
		TEST_METHOD(AsyncOneThread)
		{
			int count = 0;
			Coroutines::Start([&count]()->Coroutine
			{
				count++;
				co_return;
			}, true);

			Engine::Update();
			std::this_thread::sleep_for(5ms);
			Engine::Update();

			Engine::Update();
			Assert::AreEqual(1, count);

			std::this_thread::sleep_for(5ms);
			Engine::Update();

			// one more Update() to remove everything
			Engine::Update();
			Assert::AreEqual(0_z, Coroutines::Count());
		}

		TEST_METHOD(AsyncAtomicIncrement)
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

			std::this_thread::sleep_for(5ms);
			Engine::Update();
			Assert::AreEqual(1000, count.load());

			std::this_thread::sleep_for(5ms);
			Engine::Update();

			// one more Update() to remove everything
			Coroutines::StopAll();
			Engine::Update();
			Assert::AreEqual(0_z, Coroutines::Count());
		}

		TEST_METHOD(AsyncSort)
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

			Engine::Update();

			std::this_thread::sleep_for(5ms);
			Engine::Update();

			// using ints to prevent underflow
			for (int i = int(arrays.Size()) - 1; i >= 0; i--)
			{
				auto& v = arrays[i];
				Assert::IsTrue(std::is_sorted(v.begin(), v.end()));
			}

			// one more Update() to remove everything
			Engine::Update();
			Assert::AreEqual(0_z, Coroutines::Count());
		}
#pragma endregion
	};
}
