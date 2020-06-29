#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TYPES bool, char, int, float, uint64_t//, std::string, Array<int>, Array<std::string>, SList<int>, SList<std::string>)
#define TEST(name) TEMPLATE_TEST_CASE_METHOD(TemplateMemLeak, "HashMap::" #name, "[HashMap]", TYPES)
#define TEST_NO_MEM_CHECK(name) TEMPLATE_TEST_CASE("HashMap::" #name, "[HashMap]", TYPES)
#define CONTAINER Array<TestType>

namespace UnitTests
{
	
}
