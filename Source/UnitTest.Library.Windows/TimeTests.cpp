// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(InputTests)
	{
		INITIALIZE_CLEANUP

			using KeyCode = Input::KeyCode;
		using KeyState = Input::KeyState;

		TEST_METHOD(Lorem)
		{
			// I genuinely don't know how to test this because I can't get the same time on two different lines of code.
			// I could try threading, but I'm still at the mercy of the scheduler.
			Time::Reset();
			Time::DeltaTime();
			Time::TotalTime();
			Time::CurrentTime();
		}
	};
}
