// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventTests)
	{
		INITIALIZE_CLEANUP

		TEST_METHOD(AddListener)
		{
			Event e;
			bool invoked = false;
			e.AddListener([&invoked] { invoked = true; });
			e.Invoke();
			Assert::IsTrue(invoked);
		}

		TEST_METHOD(AddListenerCustomKey)
		{
			const auto listener = [] {};
			Event e;
			const auto key = "blarg";
			e.AddListener(key, listener);
			e.RemoveListener(key);
			e.Invoke();
			Assert::IsFalse(e.HasListeners());
		}

		TEST_METHOD(RemoveListener)
		{
			const auto listener = [] {};
			Event e;
			const auto key = e.AddListener(listener);
			e.RemoveListener(key);
			e.Invoke();
			Assert::IsFalse(e.HasListeners());
		}

		TEST_METHOD(RemoveAllListeners)
		{
			Event e;
			e.AddListener([] {});
			e.AddListener([] {});
			e.AddListener([] {});
			e.AddListener([] {});
			e.Invoke();
			Assert::AreEqual(4_z, e.ListenerCount());
			e.RemoveAllListeners();
			e.Invoke();
			Assert::IsFalse(e.HasListeners());
		}

		TEST_METHOD(OpsMidListener)
		{
			Event e;
			e.AddListener([&e]
				{
					const auto key = e.AddListener([] { Assert::Fail(); });
					e.RemoveListener(key);
					e.AddListener([] { Assert::Fail(); });
					e.AddListener([] { Assert::Fail(); });
					e.RemoveAllListeners();
				});
			// Invoke the outer event, pudding all the contained ops inside the pending queue.
			e.Invoke();
			Assert::IsTrue(e.HasListeners());
			// Carry out all the pending ops.
			e.Invoke();
			// One more invoke for good measure. If any of the adds made it, an Assert::Fail() should be hit.
			e.Invoke();
			Assert::IsFalse(e.HasListeners());
		}

		TEST_METHOD(Args)
		{
			Event<int, int> e;
			int i;
			e.AddListener([&i](const int a, const int b) { i = a + b; });
			e.Invoke(5, 6);
			Assert::AreEqual(11, i);
		}

		TEST_METHOD(Exception)
		{
			Event e;
			e.AddListener([] { throw std::exception(); });
			Assert::ExpectException<AggregateException>([&e] { e.Invoke(); });
		}
	};
}
