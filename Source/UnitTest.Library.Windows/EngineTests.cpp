// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"

using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EngineTests)
	{
	INITIALIZE_CLEANUP

		TEST_METHOD(Main)
		{
			// Start a coroutine to kill the simulation so we don't get into an infinite loop.
			Coroutines::Start([]()-> Coroutine
			{
				co_yield 5ms;
				Engine::IsActive() = false;
			});
			Engine::Main();
			Engine::IsActive() = true;
			// One more to clear the Coroutines.
			Engine::Update();
		}
	};
}
