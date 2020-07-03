#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Event::" #name, "[Event]")

namespace UnitTests
{
	TEST(StartListening)
	{
		int count = 0;
		const auto listener = [&count] { count++; };
		EventManager<>::Event("Event").AddListener("Listener", listener);
		EventManager<>::Event("Event").AddListener("Other Listener", listener);
		EventManager<>::Event("Event").Invoke();
		REQUIRE(2 == count);
		EventManager<>::RemoveAllEvents();
	}

	TEST(StopListening)
	{
		int count = 0;
		EventManager<std::string, int>::Event("Event").AddListener("Listener", [&count](const int i) { count += i; });
		EventManager<std::string, int>::RemoveAllListeners();
		EventManager<std::string, int>::Event("Event").Invoke(1);
		REQUIRE(0 == count);
		EventManager<std::string, int>::RemoveAllEvents();
	}
}
