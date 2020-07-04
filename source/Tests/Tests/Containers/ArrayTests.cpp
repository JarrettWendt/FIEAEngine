#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

using Types = std::tuple<bool, char, int, float, uint64_t, std::string, Array<int>, Array<std::string>, SList<int>, SList<std::string>>;
#define TEST(name) TEMPLATE_LIST_TEST_CASE_METHOD(TemplateMemLeak, "Array::" #name, "[Array]", Types)
#define TEST_NO_MEM_CHECK(name) TEMPLATE_LIST_TEST_CASE("Array::" #name, "[Array]", Types)
#define CONTAINER Array<TestType>

namespace UnitTests
{
	struct TestReserveStrategy final
	{
		size_t operator()(const size_t size, size_t) const noexcept
		{
			return size * 2 + 1;
		}
	};

#pragma region special members
	TEST(PrototypeConstructor)
	{
		const size_t count = Random::Range<size_t>(0, 10);
		const TestType t = Random::Next<TestType>();
		Array<TestType> c(count, t);
		
		for (const auto& a : c)
		{
			REQUIRE(t == a);
		}
	}

	TEST(IteratorConstructor)
	{
		const size_t count = Random::Range<size_t>(0, 10);
		const auto t = Random::Next<TestType>();
		Array<TestType> c(count, t);
		Array<TestType> b{ c.begin(), c.end() };

		for (const auto& a : b)
		{
			REQUIRE(t == a);
		}
	}

	TEST(CopyCtor)
	{
		const Array<TestType> a1 = Random::Next<Array<TestType>>();
		Array<TestType> a2(a1);
		REQUIRE(a1 == a2);
		a2.PushBack(Random::Next<TestType>());
		REQUIRE(a1 != a2);
	}

	TEST(CopyCtorDifferentReserve)
	{
		const auto a = Random::Next<Array<TestType, TestReserveStrategy>>();
		const Array<TestType> a1(a);
		Array<TestType> a2(a1);
		REQUIRE(a1 == a2);
		a2.PushBack(Random::Next<TestType>());
		REQUIRE(a1 != a2);
	}

	TEST(MoveCtorDifferentReserve)
	{
		const Array<TestType> a1 = Random::Next<Array<TestType, TestReserveStrategy>>();
		Array<TestType> a2(a1);
		REQUIRE(a1 == a2);
		a2.PushBack(Random::Next<TestType>());
		REQUIRE(a1 != a2);
	}

	TEST(CopyAssignDifferentReserve)
	{
		const Array<TestType, TestReserveStrategy> a1 = Random::Next<Array<TestType, TestReserveStrategy>>();
		Array<TestType> a2;
		a2 = a1;
		REQUIRE(a1 == a2);
		a2.PushBack(Random::Next<TestType>());
		REQUIRE(a1 != a2);
	}

	TEST(MoveAssignDifferentReserve)
	{
		Array<TestType> a1;
		a1 = Random::Next<Array<TestType, TestReserveStrategy>>();
		Array<TestType> a2 = a1;
		REQUIRE(a1 == a2);
		a2.PushBack(Random::Next<TestType>());
		REQUIRE(a1 != a2);
	}

	TEST(CopyAssignment)
	{
		Array<TestType> a = Random::Next<Array<TestType>>();
	}

	TEST(AssignmentOperator)
	{
		Array<TestType> a, b;

		auto t = Random::Next<TestType>();
		a.PushBack(t);
		// Make sure we hit the code of self-assignment.
		a = a;
		REQUIRE(t == a.Front());

		b.PushBack(Random::Next<TestType>());
		b = a;

		REQUIRE(a == b);
	}

	TEST(InitializerList)
	{
		auto a = Random::Next<TestType>();
		auto b = Random::Next<TestType>();
		Array<TestType> v{ a, b };

		REQUIRE(a == v.Front());
		REQUIRE(b == v.Back());

		v = { a, b };

		REQUIRE(a == v.Front());
		REQUIRE(b == v.Back());
	}

	TEST(RangeCtor)
	{
		const auto v = Random::Next<std::vector<TestType>>();
		const Array<TestType> a(v);
		REQUIRE(v.size() == a.Size());
		for (size_t i = 0; i < v.size(); i++)
		{
			REQUIRE(v[i] == a[i]);
		}
	}

	TEST(RangeAssignmentOperator)
	{
		const auto v = Random::Next<std::vector<TestType>>();
		Array<TestType> a;
		a = v;
		REQUIRE(v.size() == a.Size());
		for (size_t i = 0; i < v.size(); i++)
		{
			REQUIRE(v[i] == a[i]);
		}
	}
#pragma endregion
	
#pragma region iterator
	TEST(iterator)
	{
		if constexpr (std::is_same_v<bool, TestType>)
		{
			return;
		}
		
		using iterator = typename CONTAINER::iterator;

		REQUIRE(std::random_access_iterator<iterator>);
		
		Array<TestType> libC;
		std::vector<TestType> stdContainer;
		
		if constexpr (std::is_same_v<bool, TestType>)
		{
			stdContainer = Random::Unique<std::vector<TestType>>(2);
		}
		else if constexpr (std::is_same_v<char, TestType>)
		{
			stdContainer = Random::Unique<std::vector<TestType>>(26);
		}
		else
		{
			stdContainer = Random::Unique<std::vector<TestType>>(10);
		}
		libC = stdContainer;

		iterator it = libC.begin();
		auto stdIt = stdContainer.begin();

		// Make sure pre/post increment behave differently.
		TestType a = *++it;
		--it;
		TestType b = *it++;
		REQUIRE(a != b);
		++stdIt;
		// Make sure pre/post decrement behave differently.
		a = *--it;
		++it;
		b = *it--;
		REQUIRE(a != b);
		--stdIt;

		// Manual pass through the iterators to make sure they are the same.
		while (it != libC.end() && stdIt != stdContainer.end())
		{
			REQUIRE(*it++ == *stdIt++);
		}
		
		// make sure iterators are at end
		REQUIRE(it == libC.end());
		REQUIRE(stdIt == stdContainer.end());

		// Now make sure range-based for-loops work.
		it = libC.begin();
		for (const auto& t : libC)
		{
			REQUIRE(t == *it++);
		}

		// Make certain we compare some iterators in the middle of the container instead of just at the end.
		it = libC.begin();
		auto jt = libC.begin();
		REQUIRE(++it == ++jt);

		// Addition with difference_type
		it = libC.begin();
		it += 2;
		REQUIRE(*it == libC[2]);

		// Subtraction with size_t
		it = libC.end();
		it -= 2;
		REQUIRE(*it == libC[libC.Size() - 2]);

		REQUIRE(libC.end() < libC.end() + 1_z);
		REQUIRE(libC.begin() > libC.begin() - 1_z);

		// >=
		REQUIRE(libC.end() >= libC.begin());
		REQUIRE(libC.end() >= libC.end());
		REQUIRE(libC.end() - 1_z >= libC.begin() + 1_z);
		REQUIRE(libC.begin() + 2_z >= libC.begin() + 1_z);
		REQUIRE(libC.begin() + 2_z >= libC.begin() + 2_z);

		// <=
		REQUIRE(libC.begin() <= libC.end());
		REQUIRE(libC.end() <= libC.end());
		REQUIRE(libC.begin() + 1_z <= libC.end() - 1_z);
		REQUIRE(libC.begin() + 1_z <= libC.begin() + 2_z);
		REQUIRE(libC.begin() + 2_z <= libC.begin() + 2_z);

		// operator[]
		const iterator constInitialized = libC.begin();
		REQUIRE(constInitialized[2] == libC[2]);
	}

	TEST(const_iterator)
	{
		if constexpr (std::is_same_v<bool, TestType>)
		{
			return;
		}
		
		using const_iterator = typename CONTAINER::const_iterator;

		REQUIRE(std::random_access_iterator<const_iterator>);
		
		Array<TestType> libraryContainer;
		std::vector<TestType> stdContainer;
		const Array<TestType> constVector = Random::Next<Array<TestType>>();

		if constexpr (std::is_same_v<bool, TestType>)
		{
			stdContainer = Random::Unique<std::vector<TestType>>(2);
		}
		else if constexpr (std::is_same_v<char, TestType>)
		{
			stdContainer = Random::Unique<std::vector<TestType>>(26);
		}
		else
		{
			stdContainer = Random::Unique<std::vector<TestType>>(10);
		}
		libraryContainer = stdContainer;

		const_iterator it = libraryContainer.cbegin();
		auto stdIt = stdContainer.begin();

		TestType a, b;

		// Make sure pre/post increment behave differently.
		a = *++it;
		--it;
		b = *it++;
		REQUIRE(a != b);
		++stdIt;
		// Make sure pre/post decrement behave differently.
		a = *--it;
		++it;
		b = *it--;
		REQUIRE(a != b);
		--stdIt;

		while (it != libraryContainer.cend() && stdIt != stdContainer.cend())
		{
			REQUIRE(*it++ == *stdIt++);
		}
		
		// make sure iterators are at end
		REQUIRE(it == libraryContainer.cend());
		REQUIRE(stdIt == stdContainer.cend());

		// Make sure we hit the code of constructing a const_iterator from an iterator.
		auto temp = libraryContainer.begin();
		it = const_iterator(temp);
		// Now make sure range-based for-loops work.
		for (const auto& t : libraryContainer)
		{
			REQUIRE(t == *it++);
		}

		// Make certain we compare some iterators in the middle of the container instead of just at the end.
		it = libraryContainer.cbegin();
		auto jt = libraryContainer.cbegin();
		REQUIRE(++it == ++jt);

		// Addition with size_t
		it = libraryContainer.cbegin();
		it += 2;
		REQUIRE(*it == libraryContainer[2]);
		
		// Subtraction with size_t
		it = libraryContainer.cend();
		it -= 2;
		REQUIRE(*it == libraryContainer[libraryContainer.Size() - 2]);
		
		// >=
		REQUIRE(libraryContainer.cend() >= libraryContainer.cbegin());
		REQUIRE(libraryContainer.cend() >= libraryContainer.cend());
		REQUIRE(libraryContainer.cend() - 1_z >= libraryContainer.cbegin() + 1);
		REQUIRE(libraryContainer.cbegin() + 2_z >= libraryContainer.cbegin() + 1);
		REQUIRE(libraryContainer.cbegin() + 2_z >= libraryContainer.cbegin() + 2);

		// >
		REQUIRE(libraryContainer.cend() > libraryContainer.cbegin());

		// <=
		REQUIRE(libraryContainer.cbegin() <= libraryContainer.cend());
		REQUIRE(libraryContainer.cend() <= libraryContainer.cend());
		REQUIRE(libraryContainer.cbegin() + 1_z <= libraryContainer.cend() - 1);
		REQUIRE(libraryContainer.cbegin() + 1_z <= libraryContainer.cbegin() + 2);
		REQUIRE(libraryContainer.cbegin() + 2_z <= libraryContainer.cbegin() + 2);

		// <
		REQUIRE(libraryContainer.cbegin() < libraryContainer.cend());

		// operator->
		if constexpr (std::is_same_v<std::string, TestType>)
		{
			auto _ = libraryContainer.cbegin();
			REQUIRE(_->length() >= 0);
		}

		// operator[]
		const const_iterator constInitialized = libraryContainer.cbegin();
		REQUIRE(constInitialized[2] == libraryContainer[2]);
	}

	TEST(ReverseIterator)
	{
		using T = int;
		Array<T> c = Random::Next<Array<T>>(100);

		size_t i = c.Size();
		for (auto it = c.rbegin(); it != c.rend(); ++it)
		{
			--i;
			REQUIRE(c[i] == *it);
		}
	}
#pragma endregion
		
#pragma region Element Access
	TEST_NO_MEM_CHECK(Front)
	{
		Array<TestType> v(10);
		auto t = Random::Next<TestType>();
		v.PushFront(t);
		REQUIRE(t == v.Front());
		REQUIRE_THROWS_AS(Array<TestType>().Front(), std::out_of_range);
	}

	TEST_NO_MEM_CHECK(Back)
	{
		Array<TestType> v(10);
		auto t = Random::Next<TestType>();
		v.PushBack(t);
		REQUIRE(t == v.Back());
		REQUIRE_THROWS_AS(Array<TestType>().Back(), std::out_of_range);
	}

	TEST(At)
	{
		auto c = Random::Next<Array<TestType>>(5);
		const auto& r = c;
		const auto index = 4;
		REQUIRE(c.At(index) == r.At(index));
	}
#pragma endregion

#pragma region Query
	TEST(IndexOf)
	{
		if constexpr (std::is_same_v<bool, TestType>)
		{
			return;
		}
		const auto c = Random::Unique<Array<TestType>>(10);
		const size_t index = std::min(c.Size() - 1, Random::Range<size_t>(0, c.Size()));
		const auto& t = c[index];
		REQUIRE(index == c.IndexOf(t));
		REQUIRE(index == c.IndexOf([&t](const auto& a) { return t == a; }));
	}
	
	TEST(Find)
	{
		const auto c = Random::Next<Array<TestType>>();
		const auto& t = c[5];
		REQUIRE(Util::Find(c, t) != c.end());
		REQUIRE(Util::Find(c, [](const auto&) { return false; }) == c.end());
	}

	TEST(Exists)
	{
		const auto c = Random::Next<Array<TestType>>();
		const auto& t = Random::Element(c);
		REQUIRE(Util::Exists(c, [&t](const auto& a) { return t == a; }));
	}
#pragma endregion

#pragma region Insert
	TEST(InsertThreeIterators)
	{
		auto a = Random::Next<Array<TestType>>();
		Array<TestType> b;
		b.Insert(b.begin(), a.begin(), a.end());
		REQUIRE(a == b);
	}

	TEST(InsertIteratorLValue)
	{
		auto a = Random::Next<Array<TestType>>();
		auto t = Random::Next<TestType>();
		a.Insert(a.begin(), t);
		REQUIRE(t == a.Front());
	}

	TEST(InsertIteratorRValue)
	{
		auto a = Random::Next<Array<TestType>>();
		const auto t = a.Front();
		a.Insert(a.cbegin(), Random::NotEqualTo(t));
		REQUIRE(t != a.Front());
	}
	
	TEST(InsertIteratorCountPrototype)
	{
		auto a = Random::Next<Array<TestType>>();
		auto t = Random::Next<TestType>();
		size_t count = Random::Range<size_t>(0, 100);
		a.Insert(a.begin(), count, t);
		for (size_t i = 0; i < count; i++)
		{
			REQUIRE(t == a[i]);
		}
	}
	
	TEST(InsertInitializerList)
	{
		std::initializer_list<TestType> list = { Random::Next<TestType>(), Random::Next<TestType>() };
		Array<TestType> a = list;
		Array<TestType> b;
		b.Insert(b.begin(), list);
		REQUIRE(a == b);
	}
			
	TEST(Emplace)
	{
		auto a = Random::Next<Array<TestType>>();
		auto it = a.cbegin();
		Array<TestType> b = a;
		const auto& t = a.Emplace(it, Random::Next<TestType>());
		b.EmplaceFront(t);
		REQUIRE(a == b);

		REQUIRE_THROWS_AS(a.Emplace(a.Size() + 1, Random::Next<TestType>()), std::out_of_range);
	}
	
	TEST(PushBack)
	{
		Array<TestType> v;
		const size_t capacity = v.Capacity();

		auto t = Random::Next<TestType>();
		v.PushBack(t);

		// Make sure the case of inserting into an empty Vector works.
		REQUIRE(v.Back() == t);
		REQUIRE(capacity + 1 == v.Capacity());

		const size_t size = v.Size();
		t = Random::Next<TestType>();
		v.PushBack(t);

		// Make sure the case of inserting into a non-empty Vector works.
		REQUIRE(v.Back() == t);
		REQUIRE(size + 1 == v.Size());
	}

	TEST(PushFront)
	{
		Array<TestType> v;
		const size_t capacity = v.Capacity();

		auto t = Random::Next<TestType>();
		v.PushFront(t);

		// Make sure the case of inserting into an empty Vector works.
		REQUIRE(v.Front() == t);
		REQUIRE(capacity != v.Capacity());

		const size_t size = v.Size();
		t = Random::Next<TestType>();
		v.PushFront(t);

		// Make sure the case of inserting into a non-empty Vector works.
		REQUIRE(v.Front() == t);
		REQUIRE(size + 1 == v.Size());
	}

	TEST(PushFrontIterators)
	{
		const std::vector<TestType> v = Random::Next<std::vector<TestType>>();
		const Array<TestType> c1(v);
		Array<TestType> c2;
		c2.PushFront(v.begin(), v.end());
		REQUIRE(c1 == c2);
	}
#pragma endregion

#pragma region Remove
	TEST(Remove)
	{
		if constexpr (std::is_same_v<bool, TestType>)
		{
			return;
		}
		auto a = Random::Unique<Array<TestType>>(10);
		const auto t = Random::Element(a);
		REQUIRE(a.Remove(t));
		REQUIRE(!Util::Contains(a, t));
		REQUIRE(!a.Remove(t));

		auto begin = a.begin();
		auto end = a.end();
		REQUIRE_THROWS_AS(a.Remove(end, begin), std::invalid_argument);
	}

	TEST(RemoveAll)
	{
		auto a = Random::Next<CONTAINER>(10);
		const auto t = Random::Element(a);
		a.PushBack(t);
		REQUIRE(a.RemoveAll(t));
		REQUIRE(!Util::Contains(a, t));
	}

	TEST(RemoveIterator)
	{
		if constexpr (std::is_same_v<bool, TestType>)
		{
			return;
		}
		auto a = Random::Unique<CONTAINER>(10);
		CONTAINER b;
		b.PushBack(a.Front());
		b.PushBack(a.Back());

		auto begin = ++a.begin();
		auto end = --a.end();
		a.Remove(begin, end);

		REQUIRE(a == b);

		begin = a.begin();
		end = b.end();
	}

	TEST(RemoveAt)
	{
		if constexpr (std::is_same_v<bool, TestType>)
		{
			return;
		}
		auto c = Random::Unique<CONTAINER>(10);
		const auto t = Random::Element(c);
		const size_t index = c.IndexOf(t);
		c.RemoveAt(index);
		REQUIRE(!Util::Contains(c, t));
	}
	
	TEST(PopBack)
	{
		CONTAINER v;
		v.PushBack(Random::Next<TestType>());
		v.PopBack();
		REQUIRE(v.IsEmpty());
	}

	TEST(PopFront)
	{
		CONTAINER v;
		v.PushFront(Random::Next<TestType>());
		v.PopFront();
		REQUIRE(v.IsEmpty());

		auto stdVector = Random::Next<std::vector<TestType>>(26);
		v = stdVector;
		
		while (!stdVector.empty())
		{
			REQUIRE(v.Front() == stdVector.front());
			v.PopFront();
			stdVector.erase(stdVector.begin());
		}

		REQUIRE(v.IsEmpty());
	}
#pragma endregion

#pragma region Memory
	TEST(Reserve)
	{
		CONTAINER v;
		v.Reserve(10);
		REQUIRE(10_z == v.Capacity());
		v.Reserve(0);
		REQUIRE(10_z == v.Capacity());
	}

	TEST(Resize)
	{
		CONTAINER v;
		auto r = Random::Next<TestType>();
		v.Resize(100, r);

		REQUIRE(v.Size() == v.Capacity());
		REQUIRE(100_z == v.Size());

		for (const auto& t : v)
		{
			REQUIRE(t == r);
		}

		v.Resize(10);
		REQUIRE(v.Size() == 10_z);

		v.PushBack(Random::Next<TestType>());
		const size_t capacity = v.Capacity();
		v.ShrinkToFit();
		REQUIRE(capacity != v.Capacity());
	}

	TEST(ShrinkToFit)
	{
		CONTAINER v(100);
		v.PushBack(Random::Next<TestType>());
		v.ShrinkToFit();
		REQUIRE(1 == v.Capacity());
		v.Clear();
		v.ShrinkToFit();
		REQUIRE(0 == v.Capacity());
	}

	TEST(Swap)
	{
		auto a = Random::Next<CONTAINER>();
		auto b = Random::Next<CONTAINER>();
		const CONTAINER ca = a;
		const CONTAINER cb = b;
		a.Swap(b);
		REQUIRE(ca == b);
		REQUIRE(cb == a);
	}

	TEST(Sort)
	{
		// T::operator< must be defined.
		if constexpr (std::is_arithmetic_v<TestType>)
		{
			auto c = Random::Next<CONTAINER>();

			// std::iter_swap must work for std::sort to work
			const auto front = c.Front();
			const auto back = c.Back();
			std::iter_swap(c.begin(), c.end() - 1);
			REQUIRE(front == c.Back());
			REQUIRE(back == c.Front());

			std::sort(c.begin(), c.end());
			for (auto it = c.begin(), jt = c.begin() + 1; it != c.end() && jt != c.end(); ++it, ++jt)
			{
				REQUIRE(*it <= *jt);
			}
		}
	}

	TEST(Data)
	{
		CONTAINER a;
		REQUIRE(*reinterpret_cast<const TestType**>(const_cast<CONTAINER*>(&a)) == a.Data());

		const CONTAINER copy = a;
		CONTAINER b;
		b.SetData(a.TakeData());

		REQUIRE(copy == b);
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
			Random::Next<TestType>()
		};
		CONTAINER c = init;
		std::forward_list<TestType> list = init;

		list.reverse();
		c.Reverse();
		REQUIRE(c == CONTAINER(list.begin(), list.end()));
	}
#pragma endregion

#pragma region Operators
	TEST(Equivalence)
	{
		CONTAINER a = Random::Next<CONTAINER>(26);
		CONTAINER b = a;

		REQUIRE(a == a);
		REQUIRE(a == b);
		REQUIRE(!(a != b));

		// Now they'll have a different size.
		b.PopFront();
		REQUIRE(a != b);
		REQUIRE(!(a == b));

		// Now they'll have the same size but different elements.
		while (a.Size() != b.Size())
		{
			auto t = Random::Next<TestType>();
			if (t != a.Front())
			{
				b.PushFront(t);
			}
		}

		REQUIRE(a != b);
		REQUIRE(!(a == b));

		a.Clear();
		b.Clear();
		b = Random::Next<CONTAINER>();
		REQUIRE(!(a == b));
	}

	TEST(Subscript)
	{
		CONTAINER v;
		TestType t;
		REQUIRE_THROWS_AS(v[0], std::out_of_range);
		t = Random::Next<TestType>();
		v.PushBack(t);
		REQUIRE(t == v.Back());
		t = Random::Next<TestType>();
		v[0] = t;
		REQUIRE(v[0] == t);

		const CONTAINER cv = v;
		REQUIRE_THROWS_AS(cv[-1_z], std::out_of_range);
		REQUIRE(cv[0] == cv.Front());
	}
#pragma endregion
}
