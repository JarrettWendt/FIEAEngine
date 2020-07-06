#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define NAMESPACE "Queue::"
#define CATEGORY "[Queue]"
#define TYPES bool, char, int, float, uint64_t, std::string, Array<int>, Array<std::string>, SList<int>, SList<std::string>
#define TEST_NO_TEMPLATE(name) TEST_CASE_METHOD(MemLeak, NAMESPACE #name, CATEGORY)
#define TEST(name) TEMPLATE_TEST_CASE_METHOD(TemplateMemLeak, NAMESPACE "::" #name, CATEGORY, TYPES)
#define TEST_NO_MEM_CHECK(name) TEMPLATE_TEST_CASE(NAMESPACE "::" #name, CATEGORY, TYPES)
#define CONTAINER Queue<TestType>

namespace UnitTests
{
	TEST(IsEmpty)
	{
		CONTAINER q;
		REQUIRE(q.IsEmpty());
		q.Enqueue(Random::Next<TestType>());
		REQUIRE(!q.IsEmpty());
		q.Dequeue();
		REQUIRE(q.IsEmpty());
	}

	TEST(Size)
	{
		CONTAINER q;
		REQUIRE(0_z == q.Size());
		q.Enqueue(Random::Next<TestType>());
		REQUIRE(1_z == q.Size());
		q.Dequeue();
		REQUIRE(0_z == q.Size());
	}

	TEST(Front)
	{
		CONTAINER q;
		REQUIRE_THROWS(q.Front());
		auto t = Random::Next<TestType>();
		q.Enqueue(t);
		REQUIRE(t == q.Front());
		const CONTAINER cq = q;
		REQUIRE(t == cq.Front());
	}

	TEST(Back)
	{
		CONTAINER q;
		REQUIRE_THROWS(q.Back());
		auto t = Random::Next<TestType>();
		q.Enqueue(t);
		REQUIRE(t == q.Back());
		const CONTAINER cq = q;
		REQUIRE(t == cq.Back());
	}

	TEST(Clear)
	{
		CONTAINER q;
		q.Enqueue(Random::Next<TestType>());
		q.Clear();
		REQUIRE(q.IsEmpty());
	}
}
