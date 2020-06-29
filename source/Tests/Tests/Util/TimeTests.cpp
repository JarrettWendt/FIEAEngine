#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;

#define TEST(name) TEST_CASE_METHOD(MemLeak, #name, "[Reflection]")

namespace UnitTests
{
	TEST(Lorem)
	{
		// I genuinely don't know how to test this because I can't get the same time on two different lines of code.
		// I could try threading, but I'm still at the mercy of the scheduler.
		Time::Reset();
		Time::DeltaTime();
		Time::TotalTime();
		Time::CurrentTime();
	}
}
