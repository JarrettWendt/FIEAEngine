// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;

template<typename T>
using Rand = UnitTests::Random<T>;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(UtilTests)
	{
		template<typename T>
		using Container = SList<T>;
		
		INITIALIZE_CLEANUP

		TEST_METHOD(DefaultReserveStrategy)
		{
			Assert::AreEqual(size_t(7 * 1.5), Util::DefaultReserveStrategy{}(7, 7));
		}

		TEST_METHOD(IsOneOf)
		{
			Assert::IsTrue(Util::IsOneOf<int, bool, float, std::string, int>());
			Assert::IsFalse(Util::IsOneOf<int, bool, float, std::string, double>());
		}

		TEST_METHOD(FirstConvertible)
		{
			Assert::IsTrue(std::is_same_v<Util::FirstConvertible<int, bool, float, std::string>, bool>);
			Assert::IsTrue(std::is_same_v<Util::FirstConvertible<int, std::string, bool, float>, bool>);
		}

		TEST_METHOD(BestMatch)
		{
			Assert::IsTrue(std::is_same_v<Util::BestMatch<int, bool, float, std::string, int>, int>);
			Assert::IsTrue(std::is_same_v<Util::BestMatch<int, bool, float, std::string>, bool>);
		}

#pragma region String
		TEST_METHOD(StdToString)
		{
			using namespace std;
			using namespace Library;
			using namespace Library::Util;
			const std::vector<int> v = { 1, 2 };
			const std::string s = to_string(v);
			Assert::AreEqual("{ 1, 2 }"s, s);
		}

		TEST_METHOD(PairToString)
		{
			std::pair<int, int> p = { 6, 7 };
			Assert::AreEqual("{ 6, 7 }"s, std::to_string(p));
		}
		
		TEST_METHOD(IsEmptyOrWhitespace)
		{
			Assert::IsTrue(Util::IsEmptyOrWhitespace(""));
			Assert::IsTrue(Util::IsEmptyOrWhitespace(" "));
			Assert::IsFalse(Util::IsEmptyOrWhitespace("a"));
			Assert::IsFalse(Util::IsEmptyOrWhitespace(" a"));
		}
		
		TEST_METHOD(IsWhitespace)
		{
			Assert::IsFalse(Util::IsWhitespace(""));
			Assert::IsTrue(Util::IsWhitespace(" "));
			Assert::IsFalse(Util::IsEmptyOrWhitespace("a"));
			Assert::IsFalse(Util::IsEmptyOrWhitespace(" a"));
		}

		TEST_METHOD(HasAlpha)
		{
			Assert::IsFalse(Util::HasAlpha(""));
			Assert::IsFalse(Util::HasAlpha(" "));
			Assert::IsTrue(Util::HasAlpha(" a"));
			Assert::IsTrue(Util::HasAlpha(" ;a"));
		}

		TEST_METHOD(ToLower)
		{
			Assert::AreEqual(Util::ToLower(""), "");
			Assert::AreEqual(Util::ToLower(" "), " ");
			Assert::AreEqual(Util::ToLower("Hello, World!"), "hello, world!");
			Assert::AreEqual(Util::ToLower("hello, world!"), "hello, world!");
		}

		TEST_METHOD(ToUpper)
		{
			Assert::AreEqual(Util::ToUpper(""), "");
			Assert::AreEqual(Util::ToUpper(" "), " ");
			Assert::AreEqual(Util::ToUpper("Hello, World!"), "HELLO, WORLD!");
			Assert::AreEqual(Util::ToUpper("HELLO, WORLD!"), "HELLO, WORLD!");
		}

		TEST_METHOD(IsLower)
		{
			Assert::IsFalse(Util::IsLower(""));
			Assert::IsFalse(Util::IsLower(" "));
			Assert::IsTrue(Util::IsLower("a"));
			Assert::IsFalse(Util::IsLower(" A"));
			Assert::IsFalse(Util::IsLower("Hello, World!"));
			Assert::IsTrue(Util::IsLower("hello, world!"));
		}

		TEST_METHOD(IsUpper)
		{
			Assert::IsFalse(Util::IsUpper(""));
			Assert::IsFalse(Util::IsUpper(" "));
			Assert::IsFalse(Util::IsUpper("a"));
			Assert::IsTrue(Util::IsUpper(" A"));
			Assert::IsFalse(Util::IsUpper("Hello, World!"));
			Assert::IsTrue(Util::IsUpper("HELLO, WORLD!"));
		}

		TEST_METHOD(RemoveWhitespace)
		{
			Assert::AreEqual(Util::RemoveWhitespace(""), "");
			Assert::AreEqual(Util::RemoveWhitespace(" "), "");
			Assert::AreEqual(Util::RemoveWhitespace(" Hello, World!"), "Hello,World!");
			Assert::AreEqual(Util::RemoveWhitespace(" HELLO, WORLD!"), "HELLO,WORLD!");
		}
#pragma endregion
	};
}
