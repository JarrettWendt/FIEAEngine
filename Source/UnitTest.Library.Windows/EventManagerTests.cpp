// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventManagerTests)
	{
		INITIALIZE_CLEANUP

		TEST_METHOD(StartListening)
		{
			int count = 0;
			const auto listener = [&count] { count++; };
			EventManager<>::Event("Event").AddListener("Listener", listener);
			EventManager<>::Event("Event").AddListener("Other Listener", listener);
			EventManager<>::Event("Event").Invoke();
			Assert::AreEqual(2, count);
			EventManager<>::RemoveAllEvents();
		}

		TEST_METHOD(StopListening)
		{
			int count = 0;
			EventManager<std::string, int>::Event("Event").AddListener("Listener", [&count](const int i) { count += i; });
			EventManager<std::string, int>::RemoveAllListeners();
			EventManager<std::string, int>::Event("Event").Invoke(1);
			Assert::AreEqual(0, count);
			EventManager<std::string, int>::RemoveAllEvents();
		}
	};
}
