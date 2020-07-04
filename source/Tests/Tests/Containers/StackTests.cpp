#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

using Types = std::tuple<bool, char, int, float, uint64_t, std::string, Array<int>, Array<std::string>, SList<int>, SList<std::string>>;
#define TEST(name) TEMPLATE_LIST_TEST_CASE_METHOD(TemplateMemLeak, "Stack::" #name, "[Stack]", Types)
#define TEST_NO_TEMPLATE(name) TEST_CASE_METHOD(MemLeak, "Stack::" #name, "[Stack]")
#define TEST_NO_MEM_CHECK(name) TEMPLATE_LIST_TEST_CASE("Stack::" #name, "[Stack]", Types)
#define CONTAINER Stack<TestType>

namespace UnitTests
{
	TEST(IsEmpty)
	{
		CONTAINER s;
		REQUIRE(s.IsEmpty());
		s.Push(Random::Next<TestType>());
		REQUIRE(!s.IsEmpty());
		s.Pop();
		REQUIRE(s.IsEmpty());
	}

	TEST(Size)
	{
		CONTAINER s;
		REQUIRE(0_z == s.Size());
		s.Push(Random::Next<TestType>());
		REQUIRE(1_z == s.Size());
		s.Pop();
		REQUIRE(0_z == s.Size());
	}

	TEST(Top)
	{
		CONTAINER s;
		REQUIRE_THROWS(s.Top());
		auto t = Random::Next<TestType>();
		s.Push(t);
		REQUIRE(t == s.Top());
		const CONTAINER cs = s;
		REQUIRE(t == cs.Peek());
	}

	TEST(Clear)
	{
		CONTAINER s;
		s.Push(Random::Next<TestType>());
		s.Clear();
		REQUIRE(s.IsEmpty());
	}
}
