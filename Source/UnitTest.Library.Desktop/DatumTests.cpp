// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"

// Macros which expands to defining a recursive template method.
// The purpose of this is to run a TEST_METHOD but with different data types.
// I managed to make one macro which combined these, but intellisense didn't work for blocks within them.
#define TEMPLATE_TEST_METHOD_BEGIN(methodName)								\
	template<typename... Ts>												\
	typename std::enable_if<sizeof...(Ts) == 0>::type methodName##__() {}	\
	template<typename T, typename... Ts>									\
	void methodName##__() {

#define TEMPLATE_TEST_METHOD_END(methodName)																																																							\
	methodName##__<Ts...>(); }																																																											\
	static const EXPORT_METHOD ::Microsoft::VisualStudio::CppUnitTestFramework::MemberMethodInfo* CALLING_CONVENTION CATNAME(__GetTestMethodInfo_, methodName)() {																										\
		__GetTestClassInfo(); __GetTestVersion();																																																						\
		ALLOCATE_TESTDATA_SECTION_METHOD																																																								\
		static const ::Microsoft::VisualStudio::CppUnitTestFramework::MethodMetadata s_Metadata = {L"TestMethodInfo", L#methodName, reinterpret_cast<const unsigned char*>(__FUNCTION__), reinterpret_cast<const unsigned char*>(__FUNCDNAME__), __WFILE__, __LINE__};	\
		static ::Microsoft::VisualStudio::CppUnitTestFramework::MemberMethodInfo s_Info = {::Microsoft::VisualStudio::CppUnitTestFramework::MemberMethodInfo::TestMethod, NULL, &s_Metadata};																			\
		s_Info.method.pVoidMethod = reinterpret_cast<::Microsoft::VisualStudio::CppUnitTestFramework::TestClassImpl::__voidFunc>(&ThisClass::methodName);																												\
		return &s_Info; }																																																												\
	void methodName() { methodName##__<int, float, std::string, std::shared_ptr<Foo>>(); }

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;
using namespace UnitTests;

template<typename T>
using Rand = UnitTests::Random<T>;

namespace UnitTestLibraryDesktop
{	
	TEST_CLASS(DatumTests)
	{
		INITIALIZE_CLEANUP

		TEST_METHOD(TypeOf)
		{
			Assert::AreEqual(Datum::Type::None, Datum::TypeOf<std::exception>);
			Assert::AreEqual(Datum::Type::Bool, Datum::TypeOf<bool>);
			Assert::AreEqual(Datum::Type::Int, Datum::TypeOf<int>);
			Assert::AreEqual(Datum::Type::Float, Datum::TypeOf<float>);
			Assert::AreEqual(Datum::Type::String, Datum::TypeOf<std::string>);
			Assert::AreEqual(Datum::Type::RTTI, Datum::TypeOf<std::shared_ptr<Foo>>);

			Assert::AreEqual(Datum::Type::Bool, Library::TypeOf(Datum(1, false)[0]));
			Assert::AreEqual(Datum::Type::Int, Library::TypeOf(Datum(1, 0)[0]));
			Assert::AreEqual(Datum::Type::Float, Library::TypeOf(Datum(1, 0.f)[0]));
			Assert::AreEqual(Datum::Type::String, Library::TypeOf(Datum(1, ""s)[0]));
			Assert::AreEqual(Datum::Type::RTTI, Library::TypeOf(Datum(1, std::make_shared<Foo>(0))[0]));
		}

		TEST_METHOD(Double)
		{
			Datum d;
			d = 0.5;
			d.EmplaceBack<double>(0.5);
		}
		
#pragma region value_type
		TEMPLATE_TEST_METHOD_BEGIN(ValueTypeComparison)
		{
			Datum a = RandomDatum<T>(), b;
			if constexpr (std::is_same_v<bool, T>)
			{
				b = RandomDatum<int>();
			}
			else
			{
				b = RandomDatum<bool>();
			}
			const auto av = a[0];
			const auto bv = b[0];
			Assert::AreNotEqual(av, bv);
			Assert::AreEqual(av, a[0]);
			Assert::AreNotEqual(a, b);

			if constexpr (std::is_same_v<std::string, T>)
			{
				a.PushFront<T>("a"s);
				a.PushFront<T>("z"s);
				Assert::IsTrue(a[0] >= a[1]);
			}
		}
		TEMPLATE_TEST_METHOD_END(ValueTypeComparison)

		TEST_METHOD(ValueTypeLess)
		{
			Datum a = { 0, 1 };
			Datum b = { "a"s, "z"s };
			Datum c = { 0.f, 1.f };

			// int
			Assert::IsTrue(a[0] < a[1]);
			Assert::IsTrue(b[0] < b[1]);

			// string
			Assert::IsFalse(a[0] < b[0]);
			Assert::IsFalse(b[0] < a[0]);

			// self
			Assert::IsFalse(a[0] < a[0]);

			// conversions
			Assert::IsTrue(c[0] < a[1]);
			Assert::IsTrue(a[0] < c[1]);
		}
		
		TEMPLATE_TEST_METHOD_BEGIN(ValueType)
		{
			Datum d = RandomDatum<T>();
			Datum::value_type val = d[0];
			Assert::AreEqual(val, d.Get<T>());
		}
		TEMPLATE_TEST_METHOD_END(ValueType)
#pragma endregion
		
#pragma region Special Members
		TEMPLATE_TEST_METHOD_BEGIN(PrototypeConstructor)
		{
			const size_t count = Rand<size_t>::Next();
			const T t = Library::Random::Next<T>();
			const Datum d(count, t);
			for (const T& a : d)
			{
				Assert::AreEqual(t, a);
			}
		}
		TEMPLATE_TEST_METHOD_END(PrototypeConstructor)

		TEMPLATE_TEST_METHOD_BEGIN(IteratorConstructor)
		{
			const size_t count = Rand<size_t>::Next();
			const T r = Library::Random::Next<T>();
			Array<T> a(count, r);

			Datum b(a.begin(), a.end());

			for (T& t : b)
			{
				Assert::AreEqual(r, t);
			}
		}
		TEMPLATE_TEST_METHOD_END(IteratorConstructor)

		TEMPLATE_TEST_METHOD_BEGIN(CopyCtor)
		{
			const Datum d1 = RandomDatum<T>();
			Datum d2(d1);
			Assert::AreEqual(d1, d2);
			d2.PushBack(Library::Random::Next<T>());
			Assert::AreNotEqual(d1, d2);
		}
		TEMPLATE_TEST_METHOD_END(CopyCtor)

		TEMPLATE_TEST_METHOD_BEGIN(CopyAssignment)
		{
			Datum d = RandomDatum<T>();
		}
		TEMPLATE_TEST_METHOD_END(CopyAssignment)

		TEMPLATE_TEST_METHOD_BEGIN(AssignmentOperator)
		{
			Datum a, b;

			T t = Library::Random::Next<T>();
			a.PushBack<T>(t);
			// Make sure we hit the code of self-assignment.
			a = a;
			Assert::AreEqual(t, a.Front<T>());

			b.PushBack<T>(Library::Random::Next<T>());
			b = a;

			Assert::AreEqual(a, b);
		}
		TEMPLATE_TEST_METHOD_END(AssignmentOperator)

		TEMPLATE_TEST_METHOD_BEGIN(InitializerList)
		{
			T a = Library::Random::Next<T>();
			T b = Library::Random::Next<T>();
			Datum d{ a, b };

			Assert::AreEqual(a, d.Front<T>(), L"Front Constructor");
			Assert::AreEqual(b, d.Back<T>(), L"Back Constructor");

			d = { a, b };

			Assert::AreEqual(a, d.Front<T>(), L"Front Assignment");
			Assert::AreEqual(b, d.Back<T>(), L"Back Assignment");
		}
		TEMPLATE_TEST_METHOD_END(InitializerList)

		TEMPLATE_TEST_METHOD_BEGIN(RangeCtor)
		{
			const std::vector<T> v = Library::Random::Next<std::vector<T>>();
			const Datum d(v);
			Assert::AreEqual(v.size(), d.Size());
			for (size_t i = 0; i < v.size(); i++)
			{
				Assert::AreEqual(v[i], static_cast<const T&>(d[i]));
			}
		}
		TEMPLATE_TEST_METHOD_END(RangeCtor)

		TEMPLATE_TEST_METHOD_BEGIN(RangeAssignmentOperator)
		{
			const std::vector<T> v = Library::Random::Next<std::vector<T>>();
			Datum d;
			d = v;
			Assert::AreEqual(v.size(), d.Size());
			for (size_t i = 0; i < v.size(); i++)
			{
				Assert::AreEqual(v[i], static_cast<T&>(d[i]));
			}
		}
		TEMPLATE_TEST_METHOD_END(RangeAssignmentOperator)
#pragma endregion

#pragma region iterator
		TEMPLATE_TEST_METHOD_BEGIN(Iterator)
		{
			using iterator = typename Datum::iterator;
			using difference_type = typename iterator::difference_type;

			Assert::IsTrue(std::random_access_iterator<iterator>);

			Datum libC;
			std::vector<T> stdContainer;

			Assert::IsTrue(std::is_same_v < std::vector<T>::value_type, T>);

			if constexpr (std::is_same_v<char, T>)
			{
				FillRandomlyUnique(libC, stdContainer, 26);
			}
			else
			{
				FillRandomlyUnique(libC, stdContainer);
			}

			iterator it = libC.begin();
			auto stdIt = stdContainer.begin();

			// Make sure pre/post increment behave differently.
			T a = static_cast<T&>(*++it);
			--it;
			T b = (*it++).operator T&();
			Assert::AreNotEqual(a, b);
			++stdIt;
			// Make sure pre/post decrement behave differently.
			a = (*--it).operator T&();
			++it;
			b = (*it--).operator T&();
			Assert::AreNotEqual(a, b);
			--stdIt;

			// Manual pass through the iterators to make sure they are the same.
			while (it != libC.end() && stdIt != stdContainer.end())
			{
				Assert::AreEqual((*it++).operator T & (), T(*stdIt++));
			}

			Assert::AreEqual(it, libC.end(), L"Make sure iterator is at end");
			Assert::AreEqual(stdIt, stdContainer.end(), L"Make sure iterator is at end");

			// Now make sure range-based for-loops work.
			it = libC.begin();
			for (T& t : libC)
			{
				Assert::AreEqual(t, (*it++).operator T&());
			}

			// Make certain we compare some iterators in the middle of the container instead of just at the end.
			it = libC.begin();
			auto jt = libC.begin();
			Assert::AreEqual(++it, ++jt);

			if constexpr (!std::is_same_v<T, bool>)
			{
				// Addition with difference_type
				it = libC.begin();
				it += 2;
				Assert::AreEqual((*it).operator T & (), libC[2].operator T & ());

				// Subtraction with size_t
				it = libC.end();
				it -= 2;
				Assert::AreEqual((*it).operator T&(), libC[libC.Size() - 2].operator T&());

				Assert::IsTrue(libC.end() <= libC.end() + 1_z);
				Assert::IsTrue(libC.begin() >= libC.begin() - 1_z);

				// >=
				Assert::IsTrue(libC.end() >= libC.begin());
				Assert::IsTrue(libC.end() >= libC.end());
				Assert::IsTrue(libC.end() - 1_z >= libC.begin() + 1_z);
				Assert::IsTrue(libC.begin() + 2_z >= libC.begin() + 1_z);
				Assert::IsTrue(libC.begin() + 2_z >= libC.begin() + 2_z);

				// <=
				Assert::IsTrue(libC.begin() <= libC.end());
				Assert::IsTrue(libC.end() <= libC.end());
				Assert::IsTrue(libC.begin() + 1_z <= libC.end() - 1_z);
				Assert::IsTrue(libC.begin() + 1_z <= libC.begin() + 2_z);
				Assert::IsTrue(libC.begin() + 2_z <= libC.begin() + 2_z);

				// operator[]
				const iterator constInitialized = libC.begin();
				Assert::AreEqual(constInitialized[2].operator T&(), libC[2].operator T&());
			}
		}
		TEMPLATE_TEST_METHOD_END(Iterator)

		TEMPLATE_TEST_METHOD_BEGIN(ConstIterator)
		{
			using iterator = typename Datum::iterator;
			using const_iterator = typename Datum::const_iterator;
			using difference_type = typename Datum::const_iterator::difference_type;

			Assert::IsTrue(std::random_access_iterator<const_iterator>);

			Datum libraryContainer;
			std::vector<T> stdContainer;
			const Datum constVector = RandomDatum<T>();

			Assert::IsFalse(bool(Datum().cbegin()));
			Assert::IsTrue(!Datum().cbegin());
			Assert::IsFalse(bool(const_iterator()));
			Assert::IsFalse(bool(Datum().cbegin() + 1));
			Assert::IsTrue(Datum().cbegin().IsAtBegin());
			Assert::IsTrue(Datum().cbegin().IsAtEnd());

			if constexpr (std::is_same_v<char, T>)
			{
				FillRandomlyUnique(libraryContainer, stdContainer, 26);
			}
			else
			{
				FillRandomlyUnique(libraryContainer, stdContainer);
			}

			const_iterator it = libraryContainer.cbegin();
			auto stdIt = stdContainer.begin();

			// Make sure pre/post increment behave differently.
			T a = (*++it).operator const T & ();
			--it;
			T b = (*it++).operator const  T & ();
			Assert::AreNotEqual(a, b);
			++stdIt;
			// Make sure pre/post decrement behave differently.
			a = (*--it).operator const  T & ();
			++it;
			b = (*it--).operator const T & ();
			Assert::AreNotEqual(a, b);
			--stdIt;

			while (it != libraryContainer.cend() && stdIt != stdContainer.cend())
			{
				Assert::AreEqual((*it++).operator const T& (), *stdIt++, L"Make sure iterators remain the same");
			}

			Assert::AreEqual(it, libraryContainer.cend(), L"Make sure const_iterator is at end");
			Assert::AreEqual(stdIt, stdContainer.cend(), L"Make sure const_iterator is at end");

			// Make sure we hit the code of constructing a const_iterator from an iterator.
			auto temp = libraryContainer.begin();
			it = const_iterator(temp);
			// Now make sure range-based for-loops work.
			for (T& t : libraryContainer)
			{
				Assert::AreEqual(t, (*it++).operator const T & ());
			}

			// Make certain we compare some iterators in the middle of the container instead of just at the end.
			it = libraryContainer.cbegin();
			auto jt = libraryContainer.cbegin();
			Assert::AreEqual(++it, ++jt);

			if constexpr (!std::is_same_v<bool, T>)
			{
				// Addition with size_t
				it = libraryContainer.cbegin();
				it += 2;
				Assert::AreEqual((*it).operator const T & (), libraryContainer[2].operator const T & ());

				// Subtraction with size_t
				it = libraryContainer.cend();
				it -= 2;
				Assert::AreEqual((*it).operator const T& (), libraryContainer[libraryContainer.Size() - 2].operator T & ());

				// >=
				Assert::IsTrue(libraryContainer.cend() >= libraryContainer.cbegin());
				Assert::IsTrue(libraryContainer.cend() >= libraryContainer.cend());
				Assert::IsTrue(libraryContainer.cend() - 1_z >= libraryContainer.cbegin() + 1);
				Assert::IsTrue(libraryContainer.cbegin() + 2_z >= libraryContainer.cbegin() + 1);
				Assert::IsTrue(libraryContainer.cbegin() + 2_z >= libraryContainer.cbegin() + 2);

				// >
				Assert::IsTrue(libraryContainer.cend() > libraryContainer.cbegin());

				// <=
				Assert::IsTrue(libraryContainer.cbegin() <= libraryContainer.cend());
				Assert::IsTrue(libraryContainer.cend() <= libraryContainer.cend());
				Assert::IsTrue(libraryContainer.cbegin() + 1_z <= libraryContainer.cend() - 1);
				Assert::IsTrue(libraryContainer.cbegin() + 1_z <= libraryContainer.cbegin() + 2);
				Assert::IsTrue(libraryContainer.cbegin() + 2_z <= libraryContainer.cbegin() + 2);

				// <
				Assert::IsTrue(libraryContainer.cbegin() < libraryContainer.cend());

				// operator[]
				const const_iterator constInitialized = libraryContainer.cbegin();
				Assert::AreEqual(constInitialized[2].operator const T & (), libraryContainer[2].operator const T & ());
			}
		}
		TEMPLATE_TEST_METHOD_END(ConstIterator)

		TEST_METHOD(ReverseIterator)
		{
			using T = int;
			Datum d = RandomDatum<T>();

			size_t i = d.Size();
			for (auto it = d.rbegin(); it != d.rend(); ++it)
			{
				--i;
				Assert::AreEqual(d[i], (*it), std::stringstream() << "index " << std::to_string(i));
			}
		}
#pragma endregion

#pragma region Properties
	TEMPLATE_TEST_METHOD_BEGIN(IsFull)
	{
		Assert::IsTrue(Datum().IsFull());
	}
	TEMPLATE_TEST_METHOD_END(IsFull)
#pragma endregion
		
#pragma region Element Access
		TEMPLATE_TEST_METHOD_BEGIN(Front)
		{
			Datum d;
			T t = Library::Random::Next<T>();
			d.PushFront<T>(t);
			Assert::AreEqual(t, d.Front<T>(), L"Not same after PushFront");
			Assert::ExpectException<std::out_of_range>([] { Datum::Construct<T>().Front<T>(); });
		}
		TEMPLATE_TEST_METHOD_END(Front)

		TEMPLATE_TEST_METHOD_BEGIN(Back)
		{
			Datum d;
			T t = Library::Random::Next<T>();
			d.PushBack<T>(t);
			Assert::AreEqual(t, d.Back<T>(), L"Not same after PushFront");
			const Datum cd = d;
			Assert::AreEqual(t, cd.Back<T>());
			Assert::ExpectException<std::out_of_range>([] { Datum::Construct<T>().Back<T>(); });
		}
		TEMPLATE_TEST_METHOD_END(Back)

		TEMPLATE_TEST_METHOD_BEGIN(Get)
		{
			Datum d;
			d.PushBack(Library::Random::Next<T>());
			d.PushBack(Library::Random::Next<T>());
			d.PushBack(Library::Random::Next<T>());
			d.PushBack(Library::Random::Next<T>());
			d.PushBack(Library::Random::Next<T>());
			const auto& r = d;
			const auto index = 4;
			Assert::AreEqual(d.Get<T>(index), r.Get<T>(index));
		}
		TEMPLATE_TEST_METHOD_END(Get)

		TEMPLATE_TEST_METHOD_BEGIN(At)
		{
			Datum d = RandomDatum<T>();
			const auto& r = d;
			const auto index = 4;
			Assert::AreEqual(d.At<T>(index), r.At<T>(index));

			if constexpr (std::is_same_v<bool, T>)
			{
				Assert::ExpectException<Datum::InvalidTypeException>([&d] { d.At<int>(0); });
			}
			else
			{
				Assert::ExpectException<Datum::InvalidTypeException>([&d] { d.At<bool>(0); });
			}
		}
		TEMPLATE_TEST_METHOD_END(At)
#pragma endregion

#pragma region Setters
		TEMPLATE_TEST_METHOD_BEGIN(Set)
		{
			Datum d;
			T r = Library::Random::Next<T>();
			d.Set(r);
			Assert::AreEqual(r, d.Front<T>());
			d.PushBack<T>(r);
			r = Library::Random::Next<T>();
			d.Set(r, 1);
			Assert::AreEqual(r, d.Back<T>());
		}
		TEMPLATE_TEST_METHOD_END(Set)
#pragma endregion
		
#pragma region Insert
		TEMPLATE_TEST_METHOD_BEGIN(InsertIterator)
		{
			Datum d = RandomDatum<T>();
			const auto it = d.begin() + 2;
			const T r = Library::Random::Next<T>();
			d.Insert(it, r);
			Assert::AreEqual(r, static_cast<T&>(*it));
		}
		TEMPLATE_TEST_METHOD_END(InsertIterator)

		TEMPLATE_TEST_METHOD_BEGIN(InsertIteratorCount)
		{
			Datum d = RandomDatum<T>();
			const auto it = d.begin() + 2;
			const size_t count = 2;
			const T r = Library::Random::Next<T>();
			d.Insert(it, count, r);
			Assert::AreEqual(r, static_cast<T&>(*it));
			Assert::AreEqual(r, static_cast<T&>(*(it + 1)));
		}
		TEMPLATE_TEST_METHOD_END(InsertIteratorCount)

		TEMPLATE_TEST_METHOD_BEGIN(InsertInitializerList)
		{
			std::initializer_list<T> list = { Library::Random::Next<T>(), Library::Random::Next<T>() };
			Datum a = list;
			Datum b;
			b.Insert(b.begin(), list);
			Assert::AreEqual(a, b);
		}
		TEMPLATE_TEST_METHOD_END(InsertInitializerList)
		
		TEMPLATE_TEST_METHOD_BEGIN(InsertThreeIterators)
		{
			Array<T> arr = Library::Random::Next<Array<T>>();
			Datum a = arr;
			Datum b;
			b.Insert(b.begin(), arr.begin(), arr.end());
			Assert::AreEqual(a, b);
		}
		TEMPLATE_TEST_METHOD_END(InsertThreeIterators)

		TEMPLATE_TEST_METHOD_BEGIN(Emplace)
		{
			Datum a = RandomDatum<T>();
			auto it = a.cbegin();
			Datum b = a;
			const T r = Library::Random::Next<T>();
			const T& t = a.Emplace<T>(it, r);
			b.EmplaceFront<T>(t);
			Assert::AreEqual(a, b);

			Assert::ExpectException<std::out_of_range>([&] { a.Emplace<T>(a.Size() + 1_z, r); });
		}
		TEMPLATE_TEST_METHOD_END(Emplace)
		
		TEMPLATE_TEST_METHOD_BEGIN(PushBack)
		{			
			Datum d;
			const size_t capacity = d.Capacity();

			T t = Library::Random::Next<T>();
			d.PushBack<T>(t);

			Assert::IsFalse(d.IsEmpty());
			Assert::AreEqual(t, d.Back<T>());
			Assert::AreEqual(d.Back<T>(), d.Front<T>());
			Assert::AreEqual(capacity + 1, d.Capacity());

			const size_t size = d.Size();
			t = Library::Random::Next<T>();
			d.PushBack<T>(t);

			Assert::AreEqual(t, d.Back<T>());
			Assert::AreEqual(size + 1, d.Size());

			if constexpr (std::is_same_v<bool, T>)
			{
				Assert::ExpectException<Datum::InvalidTypeException>([&d] { d.PushBack<int>(0); });
			}
			else
			{
				Assert::ExpectException<Datum::InvalidTypeException>([&d] { d.PushBack<bool>(false); });
			}
		}
		TEMPLATE_TEST_METHOD_END(PushBack)

		TEMPLATE_TEST_METHOD_BEGIN(PushFront)
		{
			Datum d;
			const size_t capacity = d.Capacity();

			T t = Library::Random::Next<T>();
			d.PushFront<T>(t);

			Assert::IsFalse(d.IsEmpty());
			Assert::AreEqual(t, d.Back<T>(), "make sure back is the value we pushed");
			Assert::AreEqual(d.Back<T>(), d.Front<T>(), "make sure front and back are the same");
			Assert::AreEqual(capacity + 1, d.Capacity(), "make sure capacity went up");

			const size_t size = d.Size();
			t = Library::Random::Next<T>();
			d.PushFront<T>(t);

			Assert::AreEqual(t, d.Front<T>(), "make sure front is the value we pushed");
			Assert::AreEqual(size + 1, d.Size(), "make sure size went up");
		}
		TEMPLATE_TEST_METHOD_END(PushFront)

		TEMPLATE_TEST_METHOD_BEGIN(EmplaceBack)
		{
			Datum d;
			const T r = Library::Random::Next<T>();
			d.EmplaceBack<T>(r);
			Assert::AreEqual(r, d.Back<T>());
		}
		TEMPLATE_TEST_METHOD_END(EmplaceBack)

		TEMPLATE_TEST_METHOD_BEGIN(PushFrontIterators)
		{
			const std::vector<T> v = Library::Random::Next<std::vector<T>>();
			const Datum a(v);
			Datum b;
			b.PushFront(v.begin(), v.end());
			Assert::AreEqual(a, b);
		}
		TEMPLATE_TEST_METHOD_END(PushFrontIterators)

		TEMPLATE_TEST_METHOD_BEGIN(PushBackIterators)
		{
			const std::vector<T> v = Library::Random::Next<std::vector<T>>();
			const Datum a(v);
			Datum b;
			b.PushBack(v.begin(), v.end());
			Assert::AreEqual(a, b);
		}
		TEMPLATE_TEST_METHOD_END(PushBackIterators)

		TEMPLATE_TEST_METHOD_BEGIN(Fill)
		{
			Datum d;
			d.Reserve<T>(10);
			const T r = Library::Random::Next<T>();
			d.Fill<T>(r);
			for (size_t i = 0; i < 10; i++)
			{
				Assert::AreEqual(r, d.Get<T>(i));
			}
		}
		TEMPLATE_TEST_METHOD_END(Fill)
#pragma endregion

#pragma region Remove
		TEMPLATE_TEST_METHOD_BEGIN(Remove)
		{
			Datum d = RandomDatumUnique<T>();
			const T t = static_cast<T&>(Library::Random::Element(d));
			Assert::IsTrue(d.Remove<T>(t));
			Assert::IsFalse(Util::Contains(d, t));
			Assert::IsFalse(d.Remove<T>(t));

			auto begin = d.begin();
			auto end = d.end();
			Assert::ExpectException<std::invalid_argument>([&] { d.Remove(end, begin); });
			//Assert::ExpectException<std::invalid_argument>([&] { a.Remove(begin, end); });
		}
		TEMPLATE_TEST_METHOD_END(Remove)

		TEMPLATE_TEST_METHOD_BEGIN(RemoveAll)
		{
			Datum d = RandomDatum<T>();
			const T t = static_cast<T&>(Library::Random::Element(d));
			d.PushBack<T>(t);
			Assert::IsTrue(d.RemoveAll<T>(t), L"RemoveAll should succeed");
			Assert::IsFalse(Util::Contains(d, t), L"RemoveAll should be correct");
		}
		TEMPLATE_TEST_METHOD_END(RemoveAll)

		TEMPLATE_TEST_METHOD_BEGIN(RemoveIterator)
		{
			Datum a = RandomDatumUnique<T>();
			Datum b;
			b.PushBack<T>(a.Front<T>());
			b.PushBack<T>(a.Back<T>());

			auto begin = ++a.begin();
			auto end = --a.end();
			a.Remove(begin, end);

			Assert::AreEqual(a, b, L"The containers should be the same");

			begin = a.begin();
			end = b.end();
			//Assert::ExpectException<std::invalid_argument>([&] { a.Remove(begin, end); }, L"Shouldn't be able to remove with iterators from other containers");
		}
		TEMPLATE_TEST_METHOD_END(RemoveIterator)

		TEMPLATE_TEST_METHOD_BEGIN(RemoveAt)
		{
			Datum d = RandomDatumUnique<T>();
			const T t = static_cast<T&>(Library::Random::Element(d));
			const size_t index = d.IndexOf(t);
			d.RemoveAt(index);
			Assert::IsFalse(Util::Contains(d, t));
		}
		TEMPLATE_TEST_METHOD_END(RemoveAt)

		TEMPLATE_TEST_METHOD_BEGIN(PopBack)
		{
			Datum d;
			d.PushBack(Library::Random::Next<T>());
			d.PopBack();
			Assert::IsTrue(d.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(PopBack)

		TEMPLATE_TEST_METHOD_BEGIN(PopFront)
		{
			Datum d;
			d.PushFront(Library::Random::Next<T>());
			d.PopFront();
			Assert::IsTrue(d.IsEmpty());

			std::vector<T> stdVector;
			if constexpr (std::is_same_v<T, char>)
			{
				FillRandomly(d, stdVector, 26);
			}
			else
			{
				FillRandomly(d, stdVector);
			}

			while (!stdVector.empty())
			{
				std::wstringstream stream(L"Make sure fronts are the same (");
				stream << "Vector size: " << d.Size() << ", std::vector size: " << stdVector.size() << ")";

				Assert::AreEqual(d.Front<T>(), stdVector.front(), stream);
				d.PopFront();
				stdVector.erase(stdVector.begin());
			}

			Assert::IsTrue(d.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(PopFront)

		TEMPLATE_TEST_METHOD_BEGIN(RemovePredicate)
		{
			Datum d;
			const T r = Library::Random::Next<T>();
			d.PushBack<T>(r);
			Assert::IsTrue(d.Remove<T>([&r](const T& a) { return a == r; }));
			Assert::IsTrue(d.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(RemovePredicate)

		TEMPLATE_TEST_METHOD_BEGIN(RemoveAllPredicate)
		{
			Datum d;
			const T r = Library::Random::Next<T>();
			d.PushBack<T>(r);
			Assert::AreEqual(1_z, d.RemoveAll<T>([&r](const T& a) { return a == r; }));
			Assert::IsTrue(d.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(RemoveAllPredicate)

		TEMPLATE_TEST_METHOD_BEGIN(Empty)
		{
			Datum d = RandomDatum<T>();
			d.Empty();
			Assert::IsTrue(d.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(Empty)
#pragma endregion
		
#pragma region Query
		TEMPLATE_TEST_METHOD_BEGIN(IndexOf)
		{
			if constexpr (!std::is_same_v<bool, T>)
			{
				const Datum d = RandomDatumUnique<T>();
				const size_t index = std::min(d.Size() - 1, Rand<size_t>::Range(0, d.Size()));
				const T& t = d[index];
				Assert::AreEqual(index, d.IndexOf(t), L"IndexOf(T)");
				Assert::AreEqual(index, d.IndexOf<T>([&t](const T& a) { return a == t; }), L"IndexOf(Predicate)");
			}
		}
		TEMPLATE_TEST_METHOD_END(IndexOf)

		TEMPLATE_TEST_METHOD_BEGIN(Find)
		{
			const Datum d = RandomDatum<T>();
			const T& t = d[5];
			Assert::AreNotEqual(Util::Find(d, t), d.end());
			Assert::AreEqual(Util::Find(d, [](const auto&) { return false; }), d.end());
		}
		TEMPLATE_TEST_METHOD_END(Find)

		TEMPLATE_TEST_METHOD_BEGIN(Exists)
		{
			const Datum d = RandomDatum<T>();
			const T& t = Library::Random::Element(d);
			Assert::IsTrue(Util::Exists(d, [&t](const auto& a) { return t == a; }));
		}
		TEMPLATE_TEST_METHOD_END(Exists)
		
		TEMPLATE_TEST_METHOD_BEGIN(Contains)
		{
			Datum d;
			T r = Library::Random::Next<T>();
			Assert::IsFalse(Util::Contains(d, r));
			d = r;
			Assert::IsTrue(Util::Contains(d, r));
		}
		TEMPLATE_TEST_METHOD_END(Contains)
#pragma endregion

#pragma region Memory
		TEMPLATE_TEST_METHOD_BEGIN(ExternalStorage)
		{
			Array<T> array = Library::Random::Next<Array<T>>();
			T* ptr = array.Data();
			const Datum external(ptr, array.Size());
			const Datum internal(array);

			Assert::IsTrue(external.IsExternal());
			Assert::IsTrue(internal.IsInternal());

			Assert::AreEqual(internal, external);

			// copy ctor
			Datum a = external;
			Datum b = internal;

			Assert::IsTrue(a.IsExternal());
			Assert::IsTrue(b.IsInternal());

			// copy assign
			a = internal;
			b = external;

			Assert::IsTrue(a.IsInternal());
			Assert::IsTrue(b.IsExternal());

			Assert::ExpectException<Datum::ExternalStorageException>([&] { b.PushBack<T>(Library::Random::Next<T>()); });
		}
		TEMPLATE_TEST_METHOD_END(ExternalStorage)
		
		TEMPLATE_TEST_METHOD_BEGIN(Reserve)
		{
			Datum d;
			d.Reserve<T>(10);
			Assert::AreEqual(size_t(10), d.Capacity());
			d.Reserve(0);
			Assert::AreEqual(size_t(10), d.Capacity());
		}
		TEMPLATE_TEST_METHOD_END(Reserve)

		TEMPLATE_TEST_METHOD_BEGIN(Resize)
		{
			Datum d;
			T r = Library::Random::Next<T>();
			d.Resize<T>(100, r);

			Assert::AreEqual(d.Size(), d.Capacity());
			Assert::AreEqual(100_z, d.Size());

			for (T& t : d)
			{
				Assert::AreEqual(t, r);
			}

			d.Resize<T>(10);
			Assert::AreEqual(d.Size(), 10_z);

			d.PushBack<T>(Library::Random::Next<T>());
			const size_t capacity = d.Capacity();
			d.ShrinkToFit();
			Assert::AreNotEqual(capacity, d.Capacity());
		}
		TEMPLATE_TEST_METHOD_END(Resize)

		TEMPLATE_TEST_METHOD_BEGIN(ShrinkToFit)
		{
			Datum d = Datum::Construct<T>(100);
			d.PushBack(Library::Random::Next<T>());
			d.ShrinkToFit();
			Assert::AreEqual(size_t(1), d.Capacity());
			d.Clear();
			d.ShrinkToFit(0);
			Assert::AreEqual(size_t(0), d.Capacity());
		}
		TEMPLATE_TEST_METHOD_END(ShrinkToFit)

		TEMPLATE_TEST_METHOD_BEGIN(Swap)
		{
			Datum a = RandomDatum<T>();
			Datum b = RandomDatum<T>();
			const Datum ca = a;
			const Datum cb = b;
			std::swap(a, b);
			Assert::AreEqual(ca, b);
			Assert::AreEqual(cb, a);
		}
		TEMPLATE_TEST_METHOD_END(Swap)

		TEMPLATE_TEST_METHOD_BEGIN(Sort)
		{
			// T::operator< must be defined.
			if constexpr (std::is_arithmetic_v<T>)
			{
				auto c = RandomDatum<T>();

				std::sort(c.GetArray<T>().begin(), c.GetArray<T>().end());
				for (auto it = c.begin(), jt = c.begin() + 1; it != c.end() && jt != c.end(); ++it, ++jt)
				{
					Assert::IsTrue(*it <= *jt, std::stringstream() << *it << " > " << *jt);
				}
			}
		}
		TEMPLATE_TEST_METHOD_END(Sort)

		TEMPLATE_TEST_METHOD_BEGIN(Reverse)
		{
			std::initializer_list<T> init =
			{
				Library::Random::Next<T>(),
				Library::Random::Next<T>(),
				Library::Random::Next<T>(),
				Library::Random::Next<T>(),
				Library::Random::Next<T>(),
				Library::Random::Next<T>(),
				Library::Random::Next<T>()
			};
			Datum d = init;
			std::forward_list<T> list = init;

			list.reverse();
			d.Reverse();
			Assert::AreEqual(d, Datum(list.begin(), list.end()));
		}
		TEMPLATE_TEST_METHOD_END(Reverse)
#pragma endregion

#pragma region Operators
		TEMPLATE_TEST_METHOD_BEGIN(Equivalence)
		{
			// Not calling AreEqual and AreNotEqual here because I'm specifically testing operator== and operator!=
			Datum a = RandomDatum<T>();
			Datum b = a;

			Assert::IsTrue(a == a, L"Newly filled a == a");
			Assert::IsTrue(a == b, L"Newly filled a == b");
			Assert::IsFalse(a != b, L"Newly filled !(a != b)");

			// Now they'll have a different size.
			b.PopFront();
			Assert::IsTrue(a != b, L"b.pop a != b");
			Assert::IsFalse(a == b, L"b.pop !(a == b)");

			// Now they'll have the same size but different elements.
			while (a.Size() != b.Size())
			{
				T t = Library::Random::Next<T>();
				if (t != a.Front<T>())
				{
					b.PushFront<T>(t);
				}
			}
			std::wstringstream stream;
			stream << "b.push a != b, a = " << ToString(a) << ", b = " << ToString(b);

			Assert::IsTrue(a != b, stream);
			Assert::IsFalse(a == b, "b.push !(a == b)");

			a.Clear();
			b.Clear();
			b = RandomDatum<T>();
			Assert::IsFalse(a == b, L"");
		}
		TEMPLATE_TEST_METHOD_END(Equivalence)

		TEMPLATE_TEST_METHOD_BEGIN(Subscript)
		{
			Datum d;
			T t = Library::Random::Next<T>();
			d.PushBack<T>(t);
			Assert::AreEqual(t, d.Back<T>());
			t = Library::Random::Next<T>();
			d[0] = t;
			Assert::AreEqual(d[0], t);

			const Datum cd = d;
			Assert::ExpectException<std::out_of_range>([&] { const T& _ = cd[size_t(-1)]; });
			Assert::AreEqual(cd[0], cd.Front<T>());
		}
		TEMPLATE_TEST_METHOD_END(Subscript)
#pragma endregion
	};
}
