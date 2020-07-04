#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Matrix::" #name, "[Matrix]")

namespace UnitTests
{
	TEST(operator[])
	{
		const Matrix m = Matrix::Identity;
		REQUIRE(m[0][0] == 1);
		REQUIRE(m[1][1] == 1);
		REQUIRE(m[2][2] == 1);
		REQUIRE(m[3][3] == 1);
	}
	
	TEST(operator!=)
	{
		REQUIRE(Matrix{} != Matrix::Identity);
	}
	
	TEST(operator<<)
	{
		const Matrix m;
		std::stringstream stream;
		stream << m;
		REQUIRE(stream.str() == "[[0, 0, 0, 0] [0, 0, 0, 0] [0, 0, 0, 0] [0, 0, 0, 0]]");
	}
}
