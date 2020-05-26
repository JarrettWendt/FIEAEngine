#include "pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;
using namespace Library::Concept;

template<typename T>
using Rand = UnitTests::Random<T>;

namespace UnitTestLibraryDesktop
{	
	TEST_CLASS(UtilTests)
	{
		template<typename T>
		using Container = SList<T>;

		INITIALIZE_CLEANUP

		TEST_METHOD(Predicate)
		{
			Assert::IsTrue(std::predicate<std::equal_to<int>, int, int>);
		}

		TEST_METHOD(HasValueType)
		{			
			Assert::IsTrue(has_value_type<Array<int>>);
			Assert::IsFalse(has_value_type<int>);
		}

		TEST_METHOD(IsIterator)
		{
			Assert::IsTrue(::Iterator<std::vector<int>::iterator>);
			Assert::IsFalse(::Iterator<int>);
			Assert::IsTrue(::Iterator<int*>);
		}

		TEST_METHOD(HasOperatorEqual)
		{
			Assert::IsTrue(std::equality_comparable<int>);
			Assert::IsFalse(std::equality_comparable<std::function<void()>>);
		}

		TEST_METHOD(RangeOf)
		{
			Assert::IsTrue(Concept::RangeOf<std::vector<int>, int>);
			Assert::IsFalse(Concept::RangeOf<int, int>);
		}

		TEST_METHOD(ReserveStrategy)
		{
			auto lambda = []() {};
			Assert::IsTrue(Concept::ReserveStrategy<Util::DefaultReserveStrategy>);
			Assert::IsTrue(Concept::ReserveStrategy<Util::PrimeReserveStrategy>);
			Assert::IsFalse(Concept::ReserveStrategy<decltype(lambda)>);
		}

		TEST_METHOD(Hasher)
		{
			Assert::IsTrue(Concept::Hasher<Hash<int>, int>);
			Assert::IsFalse(Concept::Hasher<Hash<std::string>, int>);
			Assert::IsTrue(Concept::Hasher<std::hash<int>, int>);
			Assert::IsFalse(Concept::Hasher<std::hash<std::string>, int>);
		}

		TEST_METHOD(HasSize)
		{
			Assert::IsTrue(Concept::has_size<std::vector<int>>);
			Assert::IsFalse(Concept::has_Size<std::vector<int>>);
			Assert::IsTrue(Concept::HasSize<std::vector<int>>);
			
			Assert::IsTrue(Concept::has_Size<Array<int>>);
			Assert::IsFalse(Concept::has_size<Array<int>>);
			Assert::IsTrue(Concept::HasSize<Array<int>>);
		}
	};
}
