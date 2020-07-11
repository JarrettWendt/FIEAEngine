#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;

#define TEST(name) TEST_CASE_METHOD(MemLeak, #name, "[Engine]")

namespace UnitTests
{
	TEST(Main)
	{
		//Engine::IsActive() = false;
		//Engine::Main({});
		//Engine::IsActive() = true;
		//Engine::Init();
	}
}
