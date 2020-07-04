#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

using Types = std::tuple<bool, char, int, float, uint64_t, std::string, Array<int>, Array<std::string>, SList<int>, SList<std::string>>;
#define TEST(name) TEMPLATE_LIST_TEST_CASE_METHOD(TemplateMemLeak, "SList::" #name, "[SList]", Types)
#define TEST_NO_MEM_CHECK(name) TEMPLATE_LIST_TEST_CASE("SList::" #name, "[SList]", Types)
#define CONTAINER SList<TestType>

namespace UnitTests
{
#pragma region special members
	TEST(initializer_list)
	{
		auto a = Random::Next<TestType>();
		auto b = Random::Next<TestType>();
		CONTAINER v{ a, b };
		
		REQUIRE(a == v.Front());
		REQUIRE(b == v.Back());

		v = { a, b };

		REQUIRE(a == v.Front());
		REQUIRE(b == v.Back());
	}

	TEST(RangeCtor)
	{
		const auto v = Random::Next<std::vector<TestType>>(100);
		const CONTAINER a(v);
		REQUIRE(v.size() == a.Size());
		auto it = v.begin();
		auto jt = a.begin();
		while (it != v.end())
		{
			REQUIRE(*it++ == *jt++);
		}
	}

	TEST(RangeAssignmentOperator)
	{
		const auto v = Random::Next<std::vector<TestType>>(100);
		CONTAINER a;
		a = v;
		REQUIRE(v.size() == a.Size());
		auto it = v.begin();
		auto jt = a.begin();
		while (it != v.end())
		{
			REQUIRE(*it++ == *jt++);
		}
	}
#pragma endregion

#pragma region iterator
	TEST(iterator)
	{
		SKIP_TYPE(bool);
		
		using iterator = typename CONTAINER::iterator;
		
		std::list<TestType> list = Random::Next<std::list<TestType>>(100);
		CONTAINER slist = list;

		iterator sit = slist.begin();
		typename std::list<TestType>::iterator it = list.begin();

		if constexpr (std::is_same_v<std::string, TestType>)
		{
			REQUIRE((sit->empty() || !sit->empty()));
		}

		REQUIRE(sit);

		// Make sure pre/post increment behave differently.
		auto a = *++sit;
		sit = slist.begin();
		auto b = *sit++;

		REQUIRE(a != b);
		++it;

		while (sit != slist.end() && it != list.end())
		{
			REQUIRE(*sit++ == *it++);
		}

		REQUIRE(sit == slist.end());
		REQUIRE(it == list.end());

		// Now make sure the range-based for loop works.
		it = list.begin();
		for (const auto& t : slist)
		{
			REQUIRE(t == *it++);
		}

		REQUIRE_THROWS_AS([&] { *(slist.end()) = Random::Next<TestType>(); }(), std::out_of_range);
	}

	TEST(const_iterator)
	{	
		SKIP_TYPE(bool);
		
		using const_iterator = typename CONTAINER::const_iterator;
	
		std::list<TestType> list = Random::Next<std::list<TestType>>(100);
		CONTAINER slist = list;

		// Make const copies.
		const CONTAINER cslist = slist;
		const std::list<TestType> clist = list;

		const_iterator sit = cslist.begin();
		typename std::list<TestType>::const_iterator it = clist.cbegin();

		if constexpr (std::is_same_v<std::string, TestType>)
		{
			REQUIRE((sit->empty() || !sit->empty()));
		}

		REQUIRE(!!sit);
		REQUIRE(sit);

		REQUIRE(!sit.IsAtEnd());

		// Make sure pre/post increment behave differently.
		TestType a = *++sit;
		sit = cslist.cbegin();
		TestType b = *sit++;

		REQUIRE(a != b);
		++it;

		while (sit != cslist.cend() && it != clist.cend())
		{
			REQUIRE(*sit++ == *it++);
		}

		REQUIRE(sit == cslist.cend());
		REQUIRE(it == clist.end());

		// Now make sure the range-based for loop works.
		it = clist.cbegin();
		for (const auto& t : cslist)
		{
			REQUIRE(t == *it++);
		}

		// Make sure * throws exceptions.
		REQUIRE_THROWS_AS([&] { *(cslist.cend()) = Random::Next<TestType>(); }(), std::out_of_range);
	}

	TEST(BeforeBegin)
	{
		auto c = Random::Next<CONTAINER>(100);
		const auto t = Random::Next<TestType>();
		c.InsertAfter(c.cbefore_begin(), t);
		REQUIRE(t == c.Front());
	}

	TEST(BeforeEnd)
	{
		auto c = Random::Next<CONTAINER>(100);
		const auto t = Random::Next<TestType>();
		c.InsertAfter(c.cbefore_end(), t);
		REQUIRE(t == c.Back());
	}
#pragma endregion

#pragma region Query
		TEST(Contains)
		{
			const auto a = Random::Next<TestType>();
			CONTAINER c = { a };
			REQUIRE(Util::Contains(c, a));
			auto b = Random::NotEqualTo(a);
			REQUIRE(!Util::Contains(c, b));
		}

		TEST(Exists)
		{
			const auto c = Random::Next<CONTAINER>(100);
			const auto t = Random::Element(c);
			REQUIRE(Util::Exists(c, [&t](const auto& a) { return t == a; }));
		}

		TEST(Find)
		{
			const CONTAINER c;
			REQUIRE(Util::Find(c, TestType{}) == c.begin());
			REQUIRE(Util::Find(c, TestType{}) == c.begin());			
		}

		TEST(UtilFind)
		{
			auto c = Random::Next<CONTAINER>(3);
			const auto t = Random::Element(c);
			REQUIRE(Util::Find(c, t) == Util::Find(c, [t](const auto& a) { return t == a; }));
		}

		TEST(FindPrev)
		{
			const CONTAINER c;
			REQUIRE(c.FindPrev(TestType{}) == c.before_begin());
			TestType t;
			REQUIRE(c.FindPrev(t) == c.before_begin());
			REQUIRE(c.FindPrev([&t](const auto& a) { return t == a; }) == c.before_begin());
		}
#pragma endregion

#pragma region Insert
	TEST(PushFront)
	{
		CONTAINER list;
		auto data = Random::Next<TestType>();
		const TestType constCopy(data);
		list.PushFront(data);
		// data should not have changed
		REQUIRE(data == constCopy);
		REQUIRE(!list.IsEmpty());
		// coverage for rvalue PushFront
		list.PushFront(Random::Next<TestType>());
	}

	TEST(PushBack)
	{
		CONTAINER list;
		auto data = Random::Next<TestType>();
		const TestType constCopy(data);
		list.PushBack(data);
		// data should not have changed
		REQUIRE(data == constCopy);
		REQUIRE(!list.IsEmpty());
	}

	TEST(InsertAfter)
	{
		SKIP_TYPE(bool);
		
		auto a = Random::Unique<CONTAINER>(26);
		auto b = Random::Unique<CONTAINER>(26);
		const auto elementToInsertAfter = Random::Element(a);
		auto elementToInsert = Random::Next<TestType>();

		auto it = Util::Find(a, elementToInsertAfter);

		a.InsertAfter(it, elementToInsert);

		REQUIRE(*++it == elementToInsert);

		elementToInsert = Random::Next<TestType>();
		const auto jt = a.InsertAfter(it, { elementToInsert });
		REQUIRE(*++it == elementToInsert);
		REQUIRE(jt == it);
		
		b.Clear();
		REQUIRE(b.IsEmpty());
		
		b = a;
		REQUIRE(a == b);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
		size_t aCount = 0;
		for (const auto& _ : a)
		{
			aCount++;
		}
		size_t bCount = 0;
		for (const auto& _ : b)
		{
			bCount++;
		}
		REQUIRE(aCount == bCount);
#pragma clang diagnostic pop

		a.Clear();
		b.Clear();
		b.PushBack(elementToInsert);
		a.InsertAfter(a.end(), elementToInsert);
		REQUIRE(a == b);

		b.PushBack(elementToInsert);
		a.InsertAfter(a.end(), elementToInsert);
		REQUIRE(a == b);

		b.PushBack(elementToInsert);
		a.InsertAfter(a.end(), elementToInsert);
		REQUIRE(a == b);

		b.PushBack(elementToInsert);
		a.InsertAfter(a.end(), elementToInsert);
		REQUIRE(a == b);

		b.PushBack(elementToInsert);
		a.InsertAfter(a.end(), elementToInsert);
		REQUIRE(a == b);

		REQUIRE(a == b);
	}

	TEST(InsertAfterForwardIterator)
	{
		CONTAINER c = { Random::Next<TestType>() };
		std::forward_list<TestType> l = { Random::Next<TestType>() };
		c.InsertAfter(c.begin(), l.begin(), l.end());
		REQUIRE(l.front() == *++c.begin());
	}
	
	TEST(InsertAfterRValue)
	{
		CONTAINER c;
		auto it = c.InsertAfter(c.begin(), Random::Next<TestType>());
		REQUIRE(c.begin() == it);
	}

	TEST(InsertAfterCount)
	{
		auto c = Random::Next<CONTAINER>(100);
		const auto t = Random::Next<TestType>();
		auto it = c.cbegin();
		for (size_t i = 1; i < c.Size(); i++, ++it);

		c.InsertAfter(it, 5_z, t);
		for (size_t i = 0; i < 5; i++)
		{
			REQUIRE(t == *++it);
		}
	}

	TEST(InsertAfterRange)
	{
		CONTAINER a = Random::Next<CONTAINER>(100);
		auto v = Random::Next<Array<TestType>>(100);
		CONTAINER b = a;
		
		a.Append(v.begin(), v.end());
		auto it = b.InsertAfter(b.end(), v.begin(), v.end());

		REQUIRE(a == b);
		REQUIRE(b.end() != it);
		REQUIRE(b.end() == ++it);
	}

	TEST(Append)
	{
		std::initializer_list<TestType> init =
		{
			Random::Next<TestType>(),
			Random::Next<TestType>(),
			Random::Next<TestType>(),
			Random::Next<TestType>(),
		};
		CONTAINER a = init, b;
		b.Append(init);
		REQUIRE(a == b);

		b.Clear();
		b.Append(a);
		REQUIRE(a == b);
	}

	TEST(AppendList)
	{
		CONTAINER a = Random::Next<CONTAINER>(100), b;
		b.Append(a.begin(), a.end());
		REQUIRE(a == b);
	}
#pragma endregion

#pragma region Remove
	TEST(PopFront)
	{
		std::list<TestType> list = Random::Next<std::list<TestType>>(100);
		CONTAINER slist;

		// Make sure nothing goes wrong when popping an empty list.
		slist.PopFront();

		slist = list;
		
		// Pop those values, make sure they're in the same order.
		while (!slist.IsEmpty())
		{
			REQUIRE(slist.Front() == list.front());
			slist.PopFront();
			list.pop_front();
		}

		// Should be empty now.
		REQUIRE(slist.IsEmpty());
	}

	TEST(PopBack)
	{
		std::list<TestType> list = Random::Next<std::list<TestType>>(100);
		CONTAINER slist;

		// Make sure nothing goes wrong when popping an empty list.
		slist.PopBack();

		slist = list;
		
		// Pop those values, make sure they're in the same order.
		while (!slist.IsEmpty())
		{
			REQUIRE(slist.Back() == list.back());
			slist.PopBack();
			list.pop_back();
		}

		// Should be empty now.
		REQUIRE(slist.IsEmpty());
	}

	TEST(Clear)
	{
		CONTAINER list;

		// Nothing should go wrong when clearing an empty list.
		list.Clear();

		// Make sure we can clear a list with one element.
		list.PushBack(TestType{});
		list.Clear();
		REQUIRE(list.IsEmpty());

		// Insert some values so that clearing will work.
		list = Random::Next<CONTAINER>(100);

		// Make sure that we can clear a list with many elements.
		list.Clear();
		REQUIRE(list.IsEmpty());
	}

	TEST(Remove)
	{
		SKIP_TYPE(bool);
		
		CONTAINER c;

		REQUIRE(!c.Remove(Random::Next<TestType>()));

		c = Random::Unique<CONTAINER>(5);

		auto randT = Library::Random::Element(c);
		// Can't Assert::IsTrue on this first Remove because it could be at end()
		c.Remove(randT);
		REQUIRE(!Util::Contains(c, randT));
		REQUIRE(!c.Remove(randT));

		REQUIRE(c.Remove(c.Front()));
		//Assert::IsFalse(c.Remove(c.Back()), L"Remove Back will always return false because it returns end()");

		while (c.Size() > 1)
		{
			REQUIRE(c.Remove(c.Front()));
		}
		REQUIRE(!c.Remove(c.Front()));
	}

	TEST(RemoveAt)
	{
		SKIP_TYPE(bool);
		
		CONTAINER a = Random::Unique<CONTAINER>(100);
		CONTAINER b(a);

		auto it = b.end();
		a.PopBack();
		it = b.RemoveAt(it);
		REQUIRE(a == b);
		REQUIRE(it == b.end());

		it = b.begin();
		a.PopFront();
		it = b.RemoveAt(it);
		REQUIRE(a == b);
		REQUIRE(it == b.begin());

		it = a.begin();

		auto t = Random::Element(a);
		a.Remove(t);

		it = Util::Find(b, t);
		b.RemoveAt(it);

		REQUIRE(a == b);

		while (!b.IsEmpty())
		{
			it = b.begin();
			t = *it;
			it = b.RemoveAt(it);
			REQUIRE(!Util::Contains(b, t));
			REQUIRE(it == b.begin());
		}
		REQUIRE(it == b.end());
	}

	TEST(RemoveAll)
	{
		auto c = Random::Next<CONTAINER>(100);
		auto t = c.Front();
		c.PushBack(t);
		REQUIRE(c.RemoveAll(t) >= 2);
		REQUIRE(!Util::Contains(c, t));

		t = c.Front();
		c.PushBack(t);
		REQUIRE(c.RemoveAll(std::move(t)) >= 2);
	}

	TEST(RemoveAllAfter)
	{
		auto c = Random::Next<CONTAINER>(100);
		c.RemoveAllAfter(c.begin());
		REQUIRE(c.Size() == 1);
	}

	TEST(RemoveRange)
	{
		REQUIRE_THROWS_AS(CONTAINER{}.Remove(100, 0), std::invalid_argument);
	}

	TEST(RemoveIterator)
	{
		CONTAINER c = { Random::Next<TestType>(), Random::Next<TestType>(), Random::Next<TestType>() };
		c.Remove(++c.cbegin(), c.cend());
		REQUIRE(c.Size() == 1);

		c = Random::Next<CONTAINER>(100);
		c.Remove(c.cbegin(), c.cbefore_end());
		REQUIRE(c.Size() == 1);
	}
#pragma endregion
		
#pragma region Accessors
	TEST(Front)
	{
		CONTAINER v;
		const auto t = Random::Next<TestType>();
		v.PushFront(t);
		REQUIRE(t == v.Front());
		REQUIRE_THROWS_AS(CONTAINER{}.Front(), std::out_of_range);
		REQUIRE(v.Front() == const_cast<const CONTAINER&>(v).Front());
	}

	TEST(Back)
	{
		CONTAINER v;
		auto t = Random::Next<TestType>();
		v.PushBack(t);
		REQUIRE(t == v.Back());
		REQUIRE_THROWS_AS(CONTAINER{}.Back(), std::out_of_range);
		REQUIRE(v.Back() == const_cast<const CONTAINER&>(v).Back());
	}

	TEST(At)
	{
		const auto c = Random::Next<CONTAINER>(100);
		const auto& r = c;
		const auto index = 6;
		REQUIRE(c.At(index) == r.At(index));
		REQUIRE_THROWS_AS(CONTAINER{}.At(std::numeric_limits<typename CONTAINER::size_type>::max()), std::out_of_range);
	}
#pragma endregion

#pragma region Memory
	TEST(ShrinkTo)
	{
		auto c = Random::Next<CONTAINER>(100);
		REQUIRE_THROWS_AS(c.ShrinkTo(c.Size() + 1), std::invalid_argument);
		const auto size = c.Size() / 2;
		c.ShrinkTo(size);
		REQUIRE(size == c.Size());
	}
	
	TEST(Resize)
	{
		auto c = Random::Next<CONTAINER>(100);
		const auto size = c.Size();
		const auto t = Random::Next<TestType>();
		
		c.Resize(size / 2);
		REQUIRE(size / 2 == c.Size());

		c.Resize(size, t);
		REQUIRE(size == c.Size());
		REQUIRE(t == c.Back());
		
	}
	
	TEST(Swap)
	{
		auto a = Random::Next<CONTAINER>(100);
		auto b = Random::Next<CONTAINER>(100);
		const auto oldA = a;
		const auto oldB = b;
		a.Swap(b);
		REQUIRE(oldB == a);
		REQUIRE(oldA == b);
	}

	TEST(Reverse)
	{
		std::initializer_list<TestType> init =
		{
			Random::Next<TestType>(),
			Random::Next<TestType>(),
			Random::Next<TestType>(),
			Random::Next<TestType>(),
			Random::Next<TestType>(),
			Random::Next<TestType>(),
			Random::Next<TestType>(),
		};
		CONTAINER c = init;
		std::forward_list<TestType> list = init;

		const auto& ref = *++c.begin();
		const auto t = ref;

		list.reverse();
		c.Reverse();
		REQUIRE(t == ref);
		REQUIRE(c == CONTAINER(list.begin(), list.end()));
	}

	TEST(Sort)
	{
		// T::operator<= must be defined.
		if constexpr (std::is_arithmetic_v<TestType>)
		{
			auto a = Random::Next<Array<TestType>>(100);
			CONTAINER l(a.begin(), a.end());
			std::sort(a.begin(), a.end());
			l.Sort();
			REQUIRE(CONTAINER(a.begin(), a.end()) == l);
		}
	}

	TEST(Merge)
	{
		// T::operator<= must be defined.
		if constexpr (std::is_arithmetic_v<TestType>)
		{
			auto a = Random::Next<Array<TestType>>(100);
			CONTAINER l;
			l.Merge(CONTAINER(a.begin(), a.end()));
			REQUIRE(CONTAINER(a.begin(), a.end()) == l);
		}
	}
#pragma endregion
}
