#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Event::" #name, "[Event]")

namespace UnitTests
{
	TEST(AddListener)
	{
		Event e;
		bool invoked = false;
		e.AddListener([&invoked] { invoked = true; });
		e.Invoke();
		REQUIRE(invoked);
	}

	TEST(AddListenerCustomKey)
	{
		const auto listener = [] {};
		Event e;
		const auto key = "blarg";
		e.AddListener(key, listener);
		e.RemoveListener(key);
		e.Invoke();
		REQUIRE(!e.HasListeners());
	}

	TEST(RemoveListener)
	{
		const auto listener = [] {};
		Event e;
		const auto key = e.AddListener(listener);
		e.RemoveListener(key);
		e.Invoke();
		REQUIRE(!e.HasListeners());
	}

	TEST(RemoveAllListeners)
	{
		Event e;
		e.AddListener([] {});
		e.AddListener([] {});
		e.AddListener([] {});
		e.AddListener([] {});
		e.Invoke();
		REQUIRE(4_z == e.ListenerCount());
		e.RemoveAllListeners();
		e.Invoke();
		REQUIRE(!e.HasListeners());
	}

	TEST(OpsMidListener)
	{
		Event e;
		e.AddListener([&e]
			{
				const auto key = e.AddListener([] { FAIL(); });
				e.RemoveListener(key);
				e.AddListener([] { FAIL(); });
				e.AddListener([] { FAIL(); });
				e.RemoveAllListeners();
			});
		// Invoke the outer event, pudding all the contained ops inside the pending queue.
		e.Invoke();
		REQUIRE(e.HasListeners());
		// Carry out all the pending ops.
		e.Invoke();
		// One more invoke for good measure. If any of the adds made it, an Assert::Fail() should be hit.
		e.Invoke();
		REQUIRE(!e.HasListeners());
	}

	TEST(Args)
	{
		Event<int, int> e;
		int i;
		e.AddListener([&i](const int a, const int b) { i = a + b; });
		e.Invoke(5, 6);
		REQUIRE(11 == i);
	}

	TEST(Exception)
	{
		Event e;
		e.AddListener([] { throw std::exception(); });
		REQUIRE_THROWS_AS(e.Invoke(), AggregateException);
	}
}
