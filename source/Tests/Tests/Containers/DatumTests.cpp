#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define NAMESPACE "Datum::"
#define CATEGORY "[Datum]"
#define TYPES bool, int, float, Vector2, Vector3, Vector4, Quaternion, Matrix, Transform /*, SharedPtr<Foo>*/
#define TEST_NO_TEMPLATE(name) TEST_CASE_METHOD(MemLeak, NAMESPACE #name, CATEGORY)
#define TEST(name) TEMPLATE_TEST_CASE_METHOD(TemplateMemLeak, NAMESPACE "::" #name, CATEGORY, TYPES)
#define TEST_NO_MEM_CHECK(name) TEMPLATE_TEST_CASE(NAMESPACE "::" #name, CATEGORY, TYPES)

namespace UnitTests
{	
	TEST_NO_TEMPLATE(TypeOf)
	{
		using Type = Datum::Type;
		REQUIRE(Type::None == Datum::TypeOf<std::exception>);
		REQUIRE(Type::Bool == Datum::TypeOf<bool>);
		REQUIRE(Type::Int == Datum::TypeOf<int>);
		REQUIRE(Type::Float == Datum::TypeOf<float>);
		REQUIRE(Type::String == Datum::TypeOf<String>);
		REQUIRE(Type::RTTI == Datum::TypeOf<SharedPtr<Foo>>);

		REQUIRE(Type::Bool == Library::TypeOf(Datum(1, false)[0]));
		REQUIRE(Type::Int == Library::TypeOf(Datum(1, 0)[0]));
		REQUIRE(Type::Float == Library::TypeOf(Datum(1, 0.f)[0]));
		REQUIRE(Type::String == Library::TypeOf(Datum(1, ""s)[0]));
		REQUIRE(Type::RTTI == Library::TypeOf(Datum(1, SharedPtr<Foo>())[0]));

		REQUIRE(std::is_same_v<bool, Datum::TypeOfType<Type::Bool>>);
		REQUIRE(std::is_same_v<int, Datum::TypeOfType<Type::Int>>);
		REQUIRE(std::is_same_v<float, Datum::TypeOfType<Type::Float>>);
		REQUIRE(std::is_same_v<String, Datum::TypeOfType<Type::String>>);
		REQUIRE(std::is_same_v<SharedPtr<RTTI>, Datum::TypeOfType<Type::RTTI>>);
	}

	TEST_NO_TEMPLATE(double)
	{
		Datum d;
		d = 0.5;
		d.EmplaceBack<double>(0.5);
	}

	TEST_NO_TEMPLATE(GetType)
	{
		Datum d;
		d = 1;
		REQUIRE(Datum::Type::Int == d.GetType());
	}

	TEST_NO_TEMPLATE(operator<<)
	{
		Datum d = { 0, 1 };
		std::stringstream stream;
		stream << d;
		REQUIRE(stream.str() == "{ 0, 1 }");
	}
		
#pragma region value_type
	TEST(value_type::operator==)
	{
		SKIP_TYPE(bool);
		Datum a = Random::Next<Array<TestType>>();
		Datum b = Random::Next<Array<bool>>();
		const auto av = a[0];
		const auto bv = b[0];
		REQUIRE(av != bv);
		REQUIRE(av == a[0]);
		REQUIRE(a != b);

		if constexpr (std::is_same_v<std::string, TestType>)
		{
			a.PushFront<TestType>("a"s);
			a.PushFront<TestType>("z"s);
			REQUIRE(a[0] >= a[1]);
		}
	}

	TEST_NO_TEMPLATE(value_type::operator== non comparable types)
	{
		Datum a, b;
		a = 0;
		b = "0"_s;
		REQUIRE(!(a[0] == b[0]));
	}

	TEST_NO_TEMPLATE(value_type::operator<)
	{
		Datum a = { 0, 1 };
		Datum b = { "a"_s, "z"_s };
		Datum c = { 0.f, 1.f };

		// int
		REQUIRE(a[0] < a[1]);
		REQUIRE(b[0] < b[1]);

		// string
		REQUIRE(!(a[0] < b[0]));
		REQUIRE(!(b[0] < a[0]));

		// self
		REQUIRE(!(a[0] < a[0]));

		// conversions
		REQUIRE(c[0] < a[1]);
		REQUIRE(a[0] < c[1]);
	}
	
	TEST(value_type)
	{
		Datum d = Random::Next<Array<TestType>>();
		Datum::value_type val = d[0];
		REQUIRE(val == d.Get<TestType>());
	}
#pragma endregion
		
#pragma region special members
	TEST(PrototypeConstructor)
	{
		const size_t count = Random::Range<size_t>(0, 100);
		const auto t = Random::Next<TestType>();
		const Datum d(count, t);
		for (const TestType& a : d)
		{
			REQUIRE(t == a);
		}
	}

	TEST(IteratorConstructor)
	{
		const size_t count = Random::Range<size_t>(0, 100);
		const auto r = Random::Next<TestType>();
		Array<TestType> a(count, r);

		Datum b(a.begin(), a.end());

		for (TestType& t : b)
		{
			REQUIRE(r == t);
		}
	}

	TEST(CopyCtor)
	{
		const Datum d1 = Random::Next<std::vector<TestType>>();
		Datum d2(d1);
		REQUIRE(d1 == d2);
		d2.PushBack(Random::Next<TestType>());
		REQUIRE(d1 != d2);
	}

	TEST(CopyAssignment)
	{
		Datum d = Random::Next<std::vector<TestType>>();
	}

	TEST(AssignmentOperator)
	{
		Datum a, b;

		auto t = Random::Next<TestType>();
		a.PushBack<TestType>(t);
		// Make sure we hit the code of self-assignment.
		a = a;
		REQUIRE(t == a.Front<TestType>());

		b.PushBack<TestType>(Random::Next<TestType>());
		b = a;

		REQUIRE(a == b);
	}

	TEST(InitializerList)
	{
		auto a = Random::Next<TestType>();
		auto b = Random::Next<TestType>();
		Datum d{ a, b };

		REQUIRE(a == d.Front<TestType>());
		REQUIRE(b == d.Back<TestType>());

		d = { a, b };

		REQUIRE(a == d.Front<TestType>());
		REQUIRE(b == d.Back<TestType>());
	}

	TEST(RangeCtor)
	{
		const auto v = Random::Next<std::vector<TestType>>();
		const Datum d(v);
		REQUIRE(v.size() == d.Size());
		for (size_t i = 0; i < v.size(); i++)
		{
			REQUIRE(v[i] == static_cast<const TestType&>(d[i]));
		}
	}

	TEST(RangeAssignmentOperator)
	{
		const auto v = Random::Next<std::vector<TestType>>();
		Datum d;
		d = v;
		REQUIRE(v.size() == d.Size());
		for (size_t i = 0; i < v.size(); i++)
		{
			REQUIRE(v[i] == static_cast<TestType&>(d[i]));
		}
	}
#pragma endregion

#pragma region iterator
	TEST(iterator)
	{
		SKIP_TYPE(bool);

		using iterator = typename Datum::iterator;
		using difference_type = typename iterator::difference_type;

		REQUIRE(std::random_access_iterator<iterator>);
		
		auto stdContainer = Random::Unique<std::vector<TestType>>(26);
		Datum libC = stdContainer;

		REQUIRE(std::is_same_v<typename std::vector<TestType>::value_type, TestType>);

		iterator it = libC.begin();
		auto stdIt = stdContainer.begin();

		// Make sure pre/post increment behave differently.
		TestType a = static_cast<TestType&>(*++it);
		--it;
		TestType b = (*it++).operator TestType&();
		REQUIRE(a != b);
		++stdIt;
		// Make sure pre/post decrement behave differently.
		a = (*--it).operator TestType&();
		++it;
		b = (*it--).operator TestType&();
		REQUIRE(a != b);
		--stdIt;

		// Manual pass through the iterators to make sure they are the same.
		while (it != libC.end() && stdIt != stdContainer.end())
		{
			REQUIRE((*it++).operator TestType & () == TestType(*stdIt++));
		}

		REQUIRE(it == libC.end());
		REQUIRE(stdIt == stdContainer.end());

		// Now make sure range-based for-loops work.
		it = libC.begin();
		for (TestType& t : libC)
		{
			REQUIRE(t == (*it++).operator TestType&());
		}

		// Make certain we compare some iterators in the middle of the container instead of just at the end.
		it = libC.begin();
		auto jt = libC.begin();
		REQUIRE(++it == ++jt);

		// Addition with difference_type
		it = libC.begin();
		it += 2;
		REQUIRE((*it).operator TestType & () == libC[2].operator TestType & ());

		// Subtraction with size_t
		it = libC.end();
		it -= 2;
		REQUIRE((*it).operator TestType&() == libC[libC.Size() - 2].operator TestType&());

		REQUIRE(libC.end() <= libC.end() + 1_z);
		REQUIRE(libC.begin() >= libC.begin() - 1_z);

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
		REQUIRE(constInitialized[2].operator TestType&() == libC[2].operator TestType&());
	}

	TEST(const_iterator)
	{
		SKIP_TYPE(bool);
		
		using iterator = typename Datum::iterator;
		using const_iterator = typename Datum::const_iterator;
		using difference_type = typename Datum::const_iterator::difference_type;

		REQUIRE(std::random_access_iterator<const_iterator>);

		auto stdContainer = Random::Unique<Array<TestType>>(26);
		Datum libraryContainer = stdContainer;
		const Datum constVector = Random::Next<Array<TestType>>();

		REQUIRE(!bool(Datum().cbegin()));
		REQUIRE(!Datum().cbegin());
		REQUIRE(!bool(const_iterator()));
		REQUIRE(!bool(Datum().cbegin() + 1));
		REQUIRE(Datum().cbegin().IsAtBegin());
		REQUIRE(Datum().cbegin().IsAtEnd());

		const_iterator it = libraryContainer.cbegin();
		auto stdIt = stdContainer.begin();

		REQUIRE(libraryContainer.end() == (it + (libraryContainer.Size() + 1_z)));

		// Make sure pre/post increment behave differently.
		TestType a = *++it;
		--it;
		TestType b = *it++;
		REQUIRE(a != b);
		++stdIt;
		// Make sure pre/post decrement behave differently.
		a = (*--it).operator const TestType&();
		++it;
		b = (*it--).operator const TestType&();
		REQUIRE(a != b);
		--stdIt;

		while (it != libraryContainer.cend() && stdIt != stdContainer.cend())
		{
			REQUIRE(*it++ == *stdIt++);
		}

		REQUIRE(it == libraryContainer.cend());
		REQUIRE(stdIt == stdContainer.cend());

		// Make sure we hit the code of constructing a const_iterator from an iterator.
		auto temp = libraryContainer.begin();
		it = const_iterator(temp);
		// Now make sure range-based for-loops work.
		for (TestType& t : libraryContainer)
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

		// Subtraction with iterator
		REQUIRE(libraryContainer.cend() - libraryContainer.cbegin() == libraryContainer.Size());
		
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

		// operator[]
		const const_iterator constInitialized = libraryContainer.cbegin();
		REQUIRE(constInitialized[2] == libraryContainer[2]);
	}

	TEST_NO_TEMPLATE(ReverseIterator)
	{
		using T = int;
		Datum d = Random::Next<std::vector<T>>();

		size_t i = d.Size();
		for (auto it = d.rbegin(); it != d.rend(); ++it)
		{
			--i;
			REQUIRE(d[i] == (*it));
		}
	}
#pragma endregion

#pragma region Properties
	TEST(IsFull)
	{
		REQUIRE(Datum().IsFull());
	}
#pragma endregion
		
#pragma region Element Access
	TEST(Front)
	{
		Datum d;
		auto t = Random::Next<TestType>();
		d.PushFront<TestType>(t);
		REQUIRE(t == d.Front<TestType>());
		REQUIRE_THROWS_AS(Datum::Construct<TestType>().template Front<TestType>(), std::out_of_range);
	}

	TEST(Back)
	{
		Datum d;
		auto t = Random::Next<TestType>();
		d.PushBack<TestType>(t);
		REQUIRE(t == d.Back<TestType>());
		const Datum cd = d;
		REQUIRE(t == cd.Back<TestType>());
		REQUIRE_THROWS_AS(Datum::Construct<TestType>().template Back<TestType>(), std::out_of_range);
	}

	TEST(Get)
	{
		const Datum d = Random::Next<std::vector<TestType>>();
		const auto& r = d;
		const auto index = 4;
		REQUIRE(d.Get<TestType>(index) == r.Get<TestType>(index));
	}

	TEST(At)
	{
		Datum d = Random::Next<std::vector<TestType>>();
		const auto& r = d;
		const auto index = 4;
		REQUIRE(d.At<TestType>(index) == r.At<TestType>(index));

		if constexpr (std::is_same_v<bool, TestType>)
		{
			REQUIRE_THROWS_AS(d.At<int>(0), Datum::InvalidTypeException);
		}
		else
		{
			REQUIRE_THROWS_AS(d.At<bool>(0), Datum::InvalidTypeException);
		}
	}
#pragma endregion

#pragma region Setters
	TEST(Set)
	{
		Datum d;
		auto r = Random::Next<TestType>();
		d.Set(r);
		REQUIRE(r == d.Front<TestType>());
		d.PushBack<TestType>(r);
		r = Random::Next<TestType>();
		d.Set(r, 1);
		REQUIRE(r == d.Back<TestType>());
	}
#pragma endregion
		
#pragma region Insert
	TEST(InsertIterator)
	{
		Datum d = Random::Next<std::vector<TestType>>();
		const auto it = d.begin() + 2;
		const auto r = Random::Next<TestType>();
		d.Insert(it, r);
		REQUIRE(r == static_cast<TestType&>(*it));
	}

	TEST(InsertIteratorCount)
	{
		Datum d = Random::Next<std::vector<TestType>>();
		const auto it = d.begin() + 2;
		const size_t count = 2;
		const auto r = Random::Next<TestType>();
		d.Insert(it, count, r);
		REQUIRE(r == static_cast<TestType&>(*it));
		REQUIRE(r == static_cast<TestType&>(*(it + 1)));
	}

	TEST(InsertInitializerList)
	{
		std::initializer_list<TestType> list = { Random::Next<TestType>(), Random::Next<TestType>() };
		Datum a = list;
		Datum b;
		b.Insert(b.begin(), list);
		REQUIRE(a == b);
	}
	
	TEST(InsertThreeIterators)
	{
		auto arr = Random::Next<Array<TestType>>();
		Datum a = arr;
		Datum b;
		b.Insert(b.begin(), arr.begin(), arr.end());
		REQUIRE(a == b);
	}

	TEST(Emplace)
	{
		Datum a = Random::Next<std::vector<TestType>>();
		auto it = a.cbegin();
		Datum b = a;
		const auto r = Random::Next<TestType>();
		const TestType& t = a.Emplace<TestType>(it, r);
		b.EmplaceFront<TestType>(t);
		REQUIRE(a == b);

		REQUIRE_THROWS_AS(a.Emplace<TestType>(a.Size() + 1_z, r), std::out_of_range);
	}
	
	TEST(PushBack)
	{			
		Datum d;
		const size_t capacity = d.Capacity();

		auto t = Random::Next<TestType>();
		d.PushBack<TestType>(t);

		REQUIRE(!d.IsEmpty());
		REQUIRE(t == d.Back<TestType>());
		REQUIRE(d.Back<TestType>() == d.Front<TestType>());
		REQUIRE(capacity + 1 == d.Capacity());

		const size_t size = d.Size();
		t = Random::Next<TestType>();
		d.PushBack<TestType>(t);

		REQUIRE(t == d.Back<TestType>());
		REQUIRE(size + 1 == d.Size());

		if constexpr (std::is_same_v<bool, TestType>)
		{
			REQUIRE_THROWS_AS(d.PushBack<int>(0), Datum::InvalidTypeException);
		}
		else
		{
			REQUIRE_THROWS_AS(d.PushBack<bool>(false), Datum::InvalidTypeException);
		}
	}

	TEST(PushFront)
	{
		Datum d;
		const size_t capacity = d.Capacity();
		
		auto t = Random::Next<TestType>();
		d.PushFront<TestType>(t);

		REQUIRE(!d.IsEmpty());
		REQUIRE(t == d.Back<TestType>());
		REQUIRE(d.Back<TestType>() == d.Front<TestType>());
		REQUIRE(capacity + 1 == d.Capacity());

		const size_t size = d.Size();
		t = Random::Next<TestType>();
		d.PushFront<TestType>(t);

		REQUIRE(t == d.Front<TestType>());
		REQUIRE(size + 1 == d.Size());
	}

	TEST(EmplaceBack)
	{
		Datum d;
		auto r = Random::Next<TestType>();
		d.EmplaceBack<TestType>(r);
		REQUIRE(r == d.Back<TestType>());
	}

	TEST(PushFrontIterators)
	{
		const auto v = Random::Next<std::vector<TestType>>();
		const Datum a(v);
		Datum b;
		b.PushFront(v.begin(), v.end());
		REQUIRE(a == b);
	}

	TEST(PushBackIterators)
	{
		const auto v = Random::Next<std::vector<TestType>>();
		const Datum a(v);
		Datum b;
		b.PushBack(v.begin(), v.end());
		REQUIRE(a == b);
	}

	TEST(Fill)
	{
		Datum d;
		d.Reserve<TestType>(10);
		const auto r = Random::Next<TestType>();
		d.Fill<TestType>(r);
		for (size_t i = 0; i < 10; i++)
		{
			REQUIRE(r == d.Get<TestType>(i));
		}
	}
#pragma endregion

#pragma region Remove
	TEST(Remove)
	{
		SKIP_TYPE(bool);
		Datum d = Random::Unique<std::vector<TestType>>(26);
		const auto t = static_cast<TestType&>(Random::Element(d));
		REQUIRE(d.Remove<TestType>(t));
		REQUIRE(!Util::Contains(d, t));
		REQUIRE(!d.Remove<TestType>(t));

		auto begin = d.begin();
		auto end = d.end();
		REQUIRE_THROWS_AS(d.Remove(end, begin), std::invalid_argument);
	}

	TEST(RemoveAll)
	{
		Datum d = Random::Next<std::vector<TestType>>();
		const auto t = static_cast<TestType&>(Random::Element(d));
		d.PushBack<TestType>(t);
		REQUIRE(d.RemoveAll<TestType>(t));
		REQUIRE(!Util::Contains(d, t));
	}

	TEST(RemoveIterator)
	{
		SKIP_TYPE(bool);
		Datum a = Random::Unique<std::vector<TestType>>(26);
		Datum b;
		b.PushBack<TestType>(a.Front<TestType>());
		b.PushBack<TestType>(a.Back<TestType>());

		auto begin = ++a.begin();
		auto end = --a.end();
		a.Remove(begin, end);

		REQUIRE(a == b);

		begin = a.begin();
		end = b.end();
	}

	TEST(RemoveAt)
	{
		SKIP_TYPE(bool);
		Datum d = Random::Unique<std::vector<TestType>>(26);
		const auto t = static_cast<TestType&>(Random::Element(d));
		const size_t index = d.IndexOf(t);
		d.RemoveAt(index);
		REQUIRE(!Util::Contains(d, t));
	}

	TEST(PopBack)
	{
		Datum d;
		d.PushBack(Random::Next<TestType>());
		d.PopBack();
		REQUIRE(d.IsEmpty());
	}

	TEST(PopFront)
	{
		Datum d;
		d.PushFront(Random::Next<TestType>());
		d.PopFront();
		REQUIRE(d.IsEmpty());

		std::vector<TestType> stdVector = Random::Next<std::vector<TestType>>(26);
		d = stdVector;

		while (!stdVector.empty())
		{
			REQUIRE(d.Front<TestType>() == stdVector.front());
			d.PopFront();
			stdVector.erase(stdVector.begin());
		}

		REQUIRE(d.IsEmpty());
	}

	TEST(RemovePredicate)
	{
		Datum d;
		const auto r = Random::Next<TestType>();
		d.PushBack<TestType>(r);
		REQUIRE(d.Remove<TestType>([&r](const auto& a) { return a == r; }));
		REQUIRE(d.IsEmpty());
	}

	TEST(RemoveAllPredicate)
	{
		Datum d;
		const auto r = Random::Next<TestType>();
		d.PushBack<TestType>(r);
		REQUIRE(1_z == d.RemoveAll<TestType>([&r](const auto& a) { return a == r; }));
		REQUIRE(d.IsEmpty());
	}

	TEST(Empty)
	{
		Datum d = Random::Next<std::vector<TestType>>();
		d.Empty();
		REQUIRE(d.IsEmpty());
	}
#pragma endregion
		
#pragma region Query
	TEST(IndexOf)
	{
		SKIP_TYPE(bool);
		const Datum d = Random::Unique<std::vector<TestType>>(26);
		const size_t index = std::min(d.Size() - 1, Random::Range<size_t>(0, d.Size()));
		const TestType& t = d[index];
		REQUIRE(index == d.IndexOf(t));
		REQUIRE(index == d.IndexOf<TestType>([&t](const TestType& a) { return a == t; }));
	}

	TEST(Find)
	{
		const Datum d = Random::Next<std::vector<TestType>>();
		const TestType& t = d[5];
		REQUIRE(Util::Find(d, t) != d.end());
		REQUIRE(Util::Find(d, [](const auto&) { return false; }) == d.end());
	}

	TEST(Exists)
	{
		const Datum d = Random::Next<std::vector<TestType>>();
		const auto& t = Random::Element(d);
		REQUIRE(Util::Exists(d, [&t](const auto& a) { return t == a; }));
	}
	
	TEST(Contains)
	{
		Datum d;
		auto r = Random::Next<TestType>();
		REQUIRE(!Util::Contains(d, r));
		d = r;
		REQUIRE(Util::Contains(d, r));
	}
#pragma endregion

#pragma region Memory
	TEST(ExternalStorage)
	{
		auto array = Random::Next<Array<TestType>>();
		TestType* ptr = array.Data();
		const Datum external(ptr, array.Size());
		const Datum internal(array);

		REQUIRE(external.IsExternal());
		REQUIRE(internal.IsInternal());

		REQUIRE(internal == external);

		// copy ctor
		Datum a = external;
		Datum b = internal;

		REQUIRE(a.IsExternal());
		REQUIRE(b.IsInternal());

		// copy assign
		a = internal;
		b = external;

		REQUIRE(a.IsInternal());
		REQUIRE(b.IsExternal());

		REQUIRE_THROWS_AS(b.PushBack<TestType>(Random::Next<TestType>()), Datum::ExternalStorageException);
	}
	
	TEST(Reserve)
	{
		Datum d;
		d.Reserve<TestType>(10);
		REQUIRE(10 == d.Capacity());
		d.Reserve(0);
		REQUIRE(10 == d.Capacity());
	}

	TEST(Resize)
	{
		Datum d;
		auto r = Random::Next<TestType>();
		d.Resize<TestType>(100, r);

		REQUIRE(d.Size() == d.Capacity());
		REQUIRE(100_z == d.Size());

		for (TestType& t : d)
		{
			REQUIRE(t == r);
		}

		d.Resize<TestType>(10);
		REQUIRE(d.Size() == 10_z);

		d.PushBack<TestType>(Random::Next<TestType>());
		const size_t capacity = d.Capacity();
		d.ShrinkToFit();
		REQUIRE(capacity != d.Capacity());
	}

	TEST(ShrinkToFit)
	{
		Datum d = Datum::Construct<TestType>(100);
		d.PushBack(Random::Next<TestType>());
		d.ShrinkToFit();
		REQUIRE(1 == d.Capacity());
		d.Clear();
		d.ShrinkToFit(0);
		REQUIRE(0 == d.Capacity());
	}

	TEST(Swap)
	{
		Datum a = Random::Next<std::vector<TestType>>();
		Datum b = Random::Next<std::vector<TestType>>();
		const Datum ca = a;
		const Datum cb = b;
		std::swap(a, b);
		REQUIRE(ca == b);
		REQUIRE(cb == a);
	}

	TEST(Sort)
	{
		// T::operator< must be defined.
		if constexpr (std::is_arithmetic_v<TestType>)
		{
			Datum c = Random::Next<std::vector<TestType>>();

			std::sort(c.GetArray<TestType>().begin(), c.GetArray<TestType>().end());
			for (auto it = c.begin(), jt = c.begin() + 1; it != c.end() && jt != c.end(); ++it, ++jt)
			{
				REQUIRE(*it <= *jt);
			}
		}
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
		Datum d = init;
		std::forward_list<TestType> list = init;

		list.reverse();
		d.Reverse();
		REQUIRE(d == Datum(list.begin(), list.end()));
	}
#pragma endregion

#pragma region Operators
	TEST(Equivalence)
	{
		Datum a = Random::Next<std::vector<TestType>>();
		Datum b = a;

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
			if (t != a.Front<TestType>())
			{
				b.PushFront<TestType>(t);
			}
		}

		REQUIRE(a != b);
		REQUIRE(!(a == b));

		a.Clear();
		b.Clear();
		b = Random::Next<std::vector<TestType>>();
		REQUIRE(!(a == b));
	}

	TEST(Subscript)
	{
		Datum d;
		auto t = Random::Next<TestType>();
		d.PushBack<TestType>(t);
		REQUIRE(t == d.Back<TestType>());
		t = Random::Next<TestType>();
		d[0] = t;
		REQUIRE(d[0] == t);

		const Datum cd = d;
		REQUIRE_THROWS_AS([&] { t = cd[-1_z].operator const TestType&(); }(), std::out_of_range);
		REQUIRE(cd[0] == cd.Front<TestType>());
	}
#pragma endregion
}
