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
	void methodName() { methodName##__<char, int, float, double, uint64_t, std::string, Array<int>, Array<std::string>, SList<int>, SList<std::string>>(); }

using namespace std::string_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;

template<typename T>
using Rand = UnitTests::Random<T>;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ArrayTests)
	{
		template<typename T, typename ReserveStrategy = Util::DefaultReserveStrategy>
		using Container = Array<T, ReserveStrategy>;

		INITIALIZE_CLEANUP
		
		struct TestReserveStrategy final
		{
			size_t operator()(const size_t size, size_t) const noexcept
			{
				return size * 2 + 1;
			}
		};

#pragma region Constructors
		TEMPLATE_TEST_METHOD_BEGIN(PrototypeConstructor)
		{
			const size_t count = Rand<size_t>::Next();
			const T t = Rand<T>::Next();
			Container<T> c(count, t);
			for (const auto& a : c)
			{
				Assert::AreEqual(t, a);
			}
		}
		TEMPLATE_TEST_METHOD_END(PrototypeConstructor)

		TEMPLATE_TEST_METHOD_BEGIN(IteratorConstructor)
		{
			const size_t count = Rand<size_t>::Next();
			const T t = Rand<T>::Next();
			Container<T> c(count, t);

			Container<T> b{ c.begin(), c.end() };

			for (const auto& a : b)
			{
				Assert::AreEqual(t, a);
			}
		}
		TEMPLATE_TEST_METHOD_END(IteratorConstructor)

		TEMPLATE_TEST_METHOD_BEGIN(CopyCtor)
		{
			const Container<T> a1 = Rand<Container<T>>::Next(1);
			Container<T> a2(a1);
			Assert::AreEqual(a1, a2);
			a2.PushBack(UnitTests::Random<T>::Next());
			Assert::AreNotEqual(a1, a2);
		}
		TEMPLATE_TEST_METHOD_END(CopyCtor)

		TEMPLATE_TEST_METHOD_BEGIN(CopyCtorDifferentReserve)
		{
			const auto a = Rand<Container<T, TestReserveStrategy>>::Next();
			const Container<T> a1(a);
			Container<T> a2(a1);
			Assert::AreEqual(a1, a2);
			a2.PushBack(UnitTests::Random<T>::Next());
			Assert::AreNotEqual(a1, a2);
		}
		TEMPLATE_TEST_METHOD_END(CopyCtorDifferentReserve)

		TEMPLATE_TEST_METHOD_BEGIN(MoveCtorDifferentReserve)
		{
			const Container<T> a1 = Rand<Container<T, TestReserveStrategy>>::Next();
			Container<T> a2(a1);
			Assert::AreEqual(a1, a2);
			a2.PushBack(UnitTests::Random<T>::Next());
			Assert::AreNotEqual(a1, a2);
		}
		TEMPLATE_TEST_METHOD_END(MoveCtorDifferentReserve)

		TEMPLATE_TEST_METHOD_BEGIN(CopyAssignDifferentReserve)
		{
			const Container<T, TestReserveStrategy> a1 = Rand<Container<T, TestReserveStrategy>>::Next();
			Container<T> a2;
			a2 = a1;
			// Can't call AreEqual because they're different types.
			Assert::IsTrue(a1 == a2);
			a2.PushBack(UnitTests::Random<T>::Next());
			// Can't call AreNotEqual because they're different types.
			Assert::IsTrue(a1 != a2);
		}
		TEMPLATE_TEST_METHOD_END(CopyAssignDifferentReserve)

		TEMPLATE_TEST_METHOD_BEGIN(MoveAssignDifferentReserve)
		{
			Container<T> a1;
			a1 = Rand<Container<T, TestReserveStrategy>>::Next();
			Container<T> a2 = a1;
			Assert::AreEqual(a1, a2);
			a2.PushBack(UnitTests::Random<T>::Next());
			Assert::AreNotEqual(a1, a2);
		}
		TEMPLATE_TEST_METHOD_END(MoveAssignDifferentReserve)

		TEMPLATE_TEST_METHOD_BEGIN(CopyAssignment)
		{
			Container<T> a = UnitTests::Random<Container<T>>::Next();
		}
		TEMPLATE_TEST_METHOD_END(CopyAssignment)

		TEMPLATE_TEST_METHOD_BEGIN(AssignmentOperator)
		{
			Container<T> a, b;

			T t = UnitTests::Random<T>::Next();
			a.PushBack(t);
			// Make sure we hit the code of self-assignment.
			a = a;
			Assert::AreEqual(t, a.Front());

			b.PushBack(UnitTests::Random<T>::Next());
			b = a;

			Assert::AreEqual(a, b);
		}
		TEMPLATE_TEST_METHOD_END(AssignmentOperator)
		
		TEMPLATE_TEST_METHOD_BEGIN(InitializerList)
		{
			T a = UnitTests::Random<T>::Next();
			T b = UnitTests::Random<T>::Next();
			Container<T> v{ a, b };

			Assert::AreEqual(a, v.Front(), L"Front Constructor");
			Assert::AreEqual(b, v.Back(), L"Back Constructor");

			v = { a, b };

			Assert::AreEqual(a, v.Front(), L"Front Assignment");
			Assert::AreEqual(b, v.Back(), L"Back Assignment");
		}
		TEMPLATE_TEST_METHOD_END(InitializerList)

		TEMPLATE_TEST_METHOD_BEGIN(RangeCtor)
		{
			const std::vector<T> v = Library::Random::Next<std::vector<T>>();
			const Container<T> a(v);
			Assert::AreEqual(v.size(), a.Size());
			for (size_t i = 0; i < v.size(); i++)
			{
				Assert::AreEqual(v[i], a[i]);
			}
		}
		TEMPLATE_TEST_METHOD_END(RangeCtor)

		TEMPLATE_TEST_METHOD_BEGIN(RangeAssignmentOperator)
		{
			const std::vector<T> v = Library::Random::Next<std::vector<T>>();
			Container<T> a;
			a = v;
			Assert::AreEqual(v.size(), a.Size());
			for (size_t i = 0; i < v.size(); i++)
			{
				Assert::AreEqual(v[i], a[i]);
			}
		}
		TEMPLATE_TEST_METHOD_END(RangeAssignmentOperator)
#pragma endregion

#pragma region iterator
		TEMPLATE_TEST_METHOD_BEGIN(Iterator)
		{
			using iterator = typename Container<T>::iterator;
			using difference_type = typename iterator::difference_type;

			Assert::IsTrue(std::random_access_iterator<iterator>);
			
			Container<T> libC;
			std::vector<T> stdContainer;
			
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
			T a = *++it;
			--it;
			T b = *it++;
			Assert::AreNotEqual(a, b);
			++stdIt;
			// Make sure pre/post decrement behave differently.
			a = *--it;
			++it;
			b = *it--;
			Assert::AreNotEqual(a, b);
			--stdIt;

			// Manual pass through the iterators to make sure they are the same.
			while (it != libC.end() && stdIt != stdContainer.end())
			{
				Assert::AreEqual(*it++, *stdIt++);
			}

			Assert::AreEqual(it, libC.end(), L"Make sure iterator is at end");
			Assert::AreEqual(stdIt, stdContainer.end(), L"Make sure iterator is at end");

			// Now make sure range-based for-loops work.
			it = libC.begin();
			for (const T& t : libC)
			{
				Assert::AreEqual(t, *it++);
			}

			// Make certain we compare some iterators in the middle of the container instead of just at the end.
			it = libC.begin();
			auto jt = libC.begin();
			Assert::AreEqual(++it, ++jt);

			// Addition with difference_type
			it = libC.begin();
			it += 2;
			Assert::AreEqual(*it, libC[2]);

			// Subtraction with size_t
			it = libC.end();
			it -= 2;
			Assert::AreEqual(*it, libC[libC.Size() - 2]);

			Assert::IsTrue(libC.end() < libC.end() + 1_z);
			Assert::IsTrue(libC.begin() > libC.begin() - 1_z);

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
			const typename Container<T>::iterator constInitialized = libC.begin();
			Assert::AreEqual(constInitialized[2], libC[2]);
		}
		TEMPLATE_TEST_METHOD_END(Iterator)

		TEMPLATE_TEST_METHOD_BEGIN(ConstIterator)
		{
			using iterator = typename Container<T>::iterator;
			using const_iterator = typename Container<T>::const_iterator;
			using difference_type = typename const_iterator::difference_type;

			Assert::IsTrue(std::random_access_iterator<const_iterator>);
			
			Container<T> libraryContainer;
			std::vector<T> stdContainer;
			const Container<T> constVector = Rand<Container<T>>::Next();

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

			T a, b;

			// Make sure pre/post increment behave differently.
			a = *++it;
			--it;
			b = *it++;
			Assert::AreNotEqual(a, b);
			++stdIt;
			// Make sure pre/post decrement behave differently.
			a = *--it;
			++it;
			b = *it--;
			Assert::AreNotEqual(a, b);
			--stdIt;

			while (it != libraryContainer.cend() && stdIt != stdContainer.cend())
			{
				Assert::AreEqual(*it++, *stdIt++, L"Make sure iterators remain the same");
			}

			Assert::AreEqual(it, libraryContainer.cend(), L"Make sure const_iterator is at end");
			Assert::AreEqual(stdIt, stdContainer.cend(), L"Make sure const_iterator is at end");

			// Make sure we hit the code of constructing a const_iterator from an iterator.
			auto temp = libraryContainer.begin();
			it = const_iterator(temp);
			// Now make sure range-based for-loops work.
			for (const T& t : libraryContainer)
			{
				Assert::AreEqual(t, *it++);
			}

			// Make certain we compare some iterators in the middle of the container instead of just at the end.
			it = libraryContainer.cbegin();
			auto jt = libraryContainer.cbegin();
			Assert::AreEqual(++it, ++jt);

			// Addition with size_t
			it = libraryContainer.cbegin();
			it += 2;
			Assert::AreEqual(*it, libraryContainer[2]);
			
			// Subtraction with size_t
			it = libraryContainer.cend();
			it -= 2;
			Assert::AreEqual(*it, libraryContainer[libraryContainer.Size() - 2]);
			
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

			// operator->
			if constexpr (std::is_same_v<std::string, T>)
			{
				auto _ = libraryContainer.cbegin();
				Assert::IsTrue(_->length() >= 0);
			}

			// operator[]
			const const_iterator constInitialized = libraryContainer.cbegin();
			Assert::AreEqual(constInitialized[2], libraryContainer[2]);
		}
		TEMPLATE_TEST_METHOD_END(ConstIterator)

		TEST_METHOD(ReverseIterator)
		{
			using T = int;
			Container<T> c = Library::Random::Next<Container<T>>(10);

			size_t i = c.Size();
			for (auto it = c.rbegin(); it != c.rend(); ++it)
			{
				--i;
				Assert::AreEqual(c[i], *it, (L"index "s + std::to_wstring(i)).c_str());
			}
		}
#pragma endregion
		
#pragma region Element Access
		TEMPLATE_TEST_METHOD_BEGIN(Front)
		{
			Container<T> v(10);
			T t = UnitTests::Random<T>::Next();
			v.PushFront(t);
			Assert::AreEqual(t, v.Front(), L"Not same after PushFront");
			Assert::ExpectException<std::out_of_range>([] { Container<T>().Front(); });
		}
		TEMPLATE_TEST_METHOD_END(Front)

		TEMPLATE_TEST_METHOD_BEGIN(Back)
		{
			Container<T> v(10);
			T t = UnitTests::Random<T>::Next();
			v.PushBack(t);
			Assert::AreEqual(t, v.Back(), L"Not same after PushFront");
			Assert::ExpectException<std::out_of_range>([] { Container<T>().Back(); });
		}
		TEMPLATE_TEST_METHOD_END(Back)

		TEMPLATE_TEST_METHOD_BEGIN(At)
		{
			Container<T> c = Rand<Container<T>>::Next(5);
			const auto& r = c;
			const auto index = 4;
			Assert::AreEqual(c.At(index), r.At(index));
		}
		TEMPLATE_TEST_METHOD_END(At)
#pragma endregion

#pragma region Query
		TEMPLATE_TEST_METHOD_BEGIN(IndexOf)
		{
			const Container<T> c = Rand<Container<T>>::AllUnique(10);
			const size_t index = std::min(c.Size() - 1, Rand<size_t>::Range(0, c.Size()));
			const T& t = c[index];
			Assert::AreEqual(index, c.IndexOf(t), L"IndexOf(T)");
			Assert::AreEqual(index, c.IndexOf([&t](const auto& a) { return t == a; }), L"IndexOf(Predicate)");
		}
		TEMPLATE_TEST_METHOD_END(IndexOf)
		
		TEMPLATE_TEST_METHOD_BEGIN(Find)
		{
			const Container<T> v = UnitTests::Random<Container<T>>::Next();
			const T& t = v[5];
			Assert::AreNotEqual(Util::Find(v, t), v.end());
			Assert::AreEqual(Util::Find(v, [](const T&) { return false; }), v.end());
		}
		TEMPLATE_TEST_METHOD_END(Find)

		TEMPLATE_TEST_METHOD_BEGIN(Exists)
		{
			const Container<T> c = Rand<Container<T>>::Next();
			const T& t = Library::Random::Element(c);
			Assert::IsTrue(Util::Exists(c, [&t](const auto& a) { return t == a; }));
		}
		TEMPLATE_TEST_METHOD_END(Exists)
#pragma endregion

#pragma region Insert
		TEMPLATE_TEST_METHOD_BEGIN(InsertThreeIterators)
		{
			Container<T> a = Rand<Container<T>>::Next();
			Container<T> b;
			b.Insert(b.begin(), a.begin(), a.end());
			Assert::AreEqual(a, b);
		}
		TEMPLATE_TEST_METHOD_END(InsertThreeIterators)

		TEMPLATE_TEST_METHOD_BEGIN(InsertIteratorLValue)
		{
			Container<T> a = Rand<Container<T>>::Next();
			T t = Rand<T>::Next();
			a.Insert(a.begin(), t);
			Assert::AreEqual(t, a.Front());
		}
		TEMPLATE_TEST_METHOD_END(InsertIteratorLValue)

		TEMPLATE_TEST_METHOD_BEGIN(InsertIteratorRValue)
		{
			Container<T> a = Rand<Container<T>>::Next();
			const T t = a.Front();
			a.Insert(a.cbegin(), Rand<T>::NotEqualTo(t));
			Assert::AreNotEqual(t, a.Front());
		}
		TEMPLATE_TEST_METHOD_END(InsertIteratorRValue)
		
		TEMPLATE_TEST_METHOD_BEGIN(InsertIteratorCountPrototype)
		{
			Container<T> a = Rand<Container<T>>::Next();
			T t = Rand<T>::Next();
			size_t count = Rand<size_t>::Next();
			a.Insert(a.begin(), count, t);
			for (size_t i = 0; i < count; i++)
			{
				Assert::AreEqual(t, a[i]);
			}
		}
		TEMPLATE_TEST_METHOD_END(InsertIteratorCountPrototype)
		
		TEMPLATE_TEST_METHOD_BEGIN(InsertInitializerList)
		{
			std::initializer_list<T> list = { Rand<T>::Next(), Rand<T>::Next() };
			Container<T> a = list;
			Container<T> b;
			b.Insert(b.begin(), list);
			Assert::AreEqual(a, b);
		}
		TEMPLATE_TEST_METHOD_END(InsertInitializerList)
				
		TEMPLATE_TEST_METHOD_BEGIN(Emplace)
		{
			Container<T> a = Rand<Container<T>>::Next();
			auto it = a.cbegin();
			Container<T> b = a;
			const T& t = a.Emplace(it, Rand<T>::Next());
			b.EmplaceFront(t);
			Assert::AreEqual(a, b);

			Assert::ExpectException<std::out_of_range>([&a] { a.Emplace(a.Size() + 1, Rand<T>::Next()); });
		}
		TEMPLATE_TEST_METHOD_END(Emplace)
		
		TEMPLATE_TEST_METHOD_BEGIN(PushBack)
		{
			Container<T> v;
			const size_t capacity = v.Capacity();

			T t = UnitTests::Random<T>::Next();
			v.PushBack(t);

			// Make sure the case of inserting into an empty Vector works.
			Assert::AreEqual(v.Back(), t, L"Comparing Back on first Push");
			Assert::AreEqual(capacity + 1, v.Capacity(), L"Comparing Capacity on first Push");

			const size_t size = v.Size();
			t = UnitTests::Random<T>::Next();
			v.PushBack(t);

			// Make sure the case of inserting into a non-empty Vector works.
			Assert::AreEqual(v.Back(), t, L"Comparing Back on second Push");
			Assert::AreEqual(size + 1, v.Size(), L"Comparing Size on second Push");
		}
		TEMPLATE_TEST_METHOD_END(PushBack)

		TEMPLATE_TEST_METHOD_BEGIN(PushFront)
		{
			Container<T> v;
			const size_t capacity = v.Capacity();

			T t = UnitTests::Random<T>::Next();
			v.PushFront(t);

			// Make sure the case of inserting into an empty Vector works.
			Assert::AreEqual(v.Front(), t, L"Comparing Front after first Push");
			Assert::AreNotEqual(capacity, v.Capacity(), L"Comparing Capacity after first Push");

			const size_t size = v.Size();
			t = UnitTests::Random<T>::Next();
			v.PushFront(t);

			// Make sure the case of inserting into a non-empty Vector works.
			Assert::AreEqual(v.Front(), t, L"Comparing Front on second Push");
			Assert::AreEqual(size + 1, v.Size(), L"Comparing Size on second Push");
		}
		TEMPLATE_TEST_METHOD_END(PushFront)

		TEMPLATE_TEST_METHOD_BEGIN(PushFrontIterators)
		{
			const std::vector<T> v = Library::Random::Next<std::vector<T>>();
			const Container<T> c1(v);
			Container<T> c2;
			c2.PushFront(v.begin(), v.end());
			Assert::AreEqual(c1, c2);
		}
		TEMPLATE_TEST_METHOD_END(PushFrontIterators)
#pragma endregion

#pragma region Remove
		TEMPLATE_TEST_METHOD_BEGIN(Remove)
		{
			Container<T> a = UnitTests::Random<Container<T>>::AllUnique(10);
			const T t = Library::Random::Element(a);
			Assert::IsTrue(a.Remove(t));
			Assert::IsFalse(Util::Contains(a, t));
			Assert::IsFalse(a.Remove(t));

			auto begin = a.begin();
			auto end = a.end();
			Assert::ExpectException<std::invalid_argument>([&] { a.Remove(end, begin); });
			//Assert::ExpectException<std::invalid_argument>([&] { a.Remove(begin, end); });
		}
		TEMPLATE_TEST_METHOD_END(Remove)

		TEMPLATE_TEST_METHOD_BEGIN(RemoveAll)
		{
			Container<T> a = Rand<Container<T>>::Next(10);
			const T t = Library::Random::Element(a);
			a.PushBack(t);
			Assert::IsTrue(a.RemoveAll(t), L"RemoveAll should succeed");
			Assert::IsFalse(Util::Contains(a, t), L"RemoveAll should be correct");
		}
		TEMPLATE_TEST_METHOD_END(RemoveAll)

		TEMPLATE_TEST_METHOD_BEGIN(RemoveIterator)
		{
			Container<T> a = Rand<Container<T>>::AllUnique();
			Container<T> b;
			b.PushBack(a.Front());
			b.PushBack(a.Back());

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
			Container<T> c = Rand<Container<T>>::AllUnique(10);
			const T t = Library::Random::Element(c);
			const size_t index = c.IndexOf(t);
			c.RemoveAt(index);
			Assert::IsFalse(Util::Contains(c, t));
		}
		TEMPLATE_TEST_METHOD_END(RemoveAt)
		
		TEMPLATE_TEST_METHOD_BEGIN(PopBack)
		{
			Container<T> v;
			v.PushBack(UnitTests::Random<T>::Next());
			v.PopBack();
			Assert::IsTrue(v.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(PopBack)

		TEMPLATE_TEST_METHOD_BEGIN(PopFront)
		{
			Container<T> v;
			v.PushFront(UnitTests::Random<T>::Next());
			v.PopFront();
			Assert::IsTrue(v.IsEmpty());

			std::vector<T> stdVector;
			if constexpr (std::is_same_v<T, char>)
			{
				FillRandomly(v, stdVector, 26);
			}
			else
			{
				FillRandomly(v, stdVector);
			}
			
			while (!stdVector.empty())
			{
				std::wstringstream stream(L"Make sure fronts are the same (");
				stream << "Vector size: " << v.Size() << ", std::vector size: " << stdVector.size() << ")";
				std::wstring s;
				std::getline(stream, s);

				Assert::AreEqual(v.Front(), stdVector.front(), s.c_str());
				v.PopFront();
				stdVector.erase(stdVector.begin());
			}

			Assert::IsTrue(v.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(PopFront)
#pragma endregion

#pragma region Memory
		TEMPLATE_TEST_METHOD_BEGIN(Reserve)
		{
			Container<T> v;
			v.Reserve(10);
			Assert::AreEqual(size_t(10), v.Capacity());
			v.Reserve(0);
			Assert::AreEqual(size_t(10), v.Capacity());
		}
		TEMPLATE_TEST_METHOD_END(Reserve)

		TEMPLATE_TEST_METHOD_BEGIN(Resize)
		{
			Container<T> v;
			T r = Rand<T>::Next();
			v.Resize(100, r);

			Assert::AreEqual(v.Size(), v.Capacity());
			Assert::AreEqual(size_t(100), v.Size());

			for (const T& t : v)
			{
				Assert::AreEqual(t, r);
			}

			v.Resize(10);
			Assert::AreEqual(v.Size(), size_t(10));

			v.PushBack(Rand<T>::Next());
			const size_t capacity = v.Capacity();
			v.ShrinkToFit();
			Assert::AreNotEqual(capacity, v.Capacity());
		}
		TEMPLATE_TEST_METHOD_END(Resize)

		TEMPLATE_TEST_METHOD_BEGIN(ShrinkToFit)
		{
			Container<T> v(100);
			v.PushBack(Rand<T>::Next());
			v.ShrinkToFit();
			Assert::AreEqual(size_t(1), v.Capacity());
			v.Clear();
			v.ShrinkToFit();
			Assert::AreEqual(size_t(0), v.Capacity());
		}
		TEMPLATE_TEST_METHOD_END(ShrinkToFit)

		TEMPLATE_TEST_METHOD_BEGIN(Swap)
		{
			Container<T> a = Rand<Container<T>>::Next();
			Container<T> b = Rand<Container<T>>::Next();
			const Container<T> ca = a;
			const Container<T> cb = b;
			a.Swap(b);
			Assert::AreEqual(ca, b);
			Assert::AreEqual(cb, a);
		}
		TEMPLATE_TEST_METHOD_END(Swap)

		TEMPLATE_TEST_METHOD_BEGIN(Sort)
		{
			// T::operator< must be defined.
			if constexpr (std::is_arithmetic_v<T>)
			{
				auto c = Library::Random::Next<Container<T>>();

				// std::iter_swap must work for std::sort to work
				const auto front = c.Front();
				const auto back = c.Back();
				std::iter_swap(c.begin(), c.end() - 1);
				Assert::AreEqual(front, c.Back());
				Assert::AreEqual(back, c.Front());

				std::sort(c.begin(), c.end());
				for (auto it = c.begin(), jt = c.begin() + 1; it != c.end() && jt != c.end(); ++it, ++jt)
				{
					Assert::IsTrue(*it <= *jt, std::stringstream() << *it << " > " << *jt);
				}
			}
		}
		TEMPLATE_TEST_METHOD_END(Sort)

		TEMPLATE_TEST_METHOD_BEGIN(Data)
		{
			Container<T> a;
			Assert::AreEqual(*reinterpret_cast<const T**>(const_cast<Container<T>*>(&a)), a.Data());

			const Container<T> copy = a;
			Container<T> b;
			b.SetData(a.TakeData());

			Assert::AreEqual(copy, b);
		}
		TEMPLATE_TEST_METHOD_END(Data)

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
			Container<T> c = init;
			std::forward_list<T> list = init;

			list.reverse();
			c.Reverse();
			Assert::AreEqual(c, Container<T>(list.begin(), list.end()));
		}
		TEMPLATE_TEST_METHOD_END(Reverse)
#pragma endregion

#pragma region Operators
		TEMPLATE_TEST_METHOD_BEGIN(Equivalence)
		{
			// Not calling AreEqual and AreNotEqual here because I'm specifically testing operator== and operator!=
			Container<T> a, b;
			if constexpr (std::is_same_v<T, char>)
			{
				FillRandomly(a, b, 26);
			}
			else
			{
				FillRandomly(a, b);
			}

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
				T t = Rand<T>::Next();
				if (t != a.Front())
				{
					b.PushFront(t);
				}
			}
			std::wstringstream stream;
			stream << L"b.push a != b, a = " << ToString(a) << ", b = " << ToString(b);

			Assert::IsTrue(a != b, stream);
			Assert::IsFalse(a == b, L"b.push !(a == b)");

			a.Clear();
			b.Clear();
			b = Rand<Container<T>>::Next();
			Assert::IsFalse(a == b, L"");
		}
		TEMPLATE_TEST_METHOD_END(Equivalence)

		TEMPLATE_TEST_METHOD_BEGIN(Subscript)
		{
			Container<T> v;
			T t;
			Assert::ExpectException<std::out_of_range>([&] { t = v[0]; });
			t = Rand<T>::Next();
			v.PushBack(t);
			Assert::AreEqual(t, v.Back());
			t = Rand<T>::Next();
			v[0] = t;
			Assert::AreEqual(v[0], t);

			const Container<T> cv = v;
			Assert::ExpectException<std::out_of_range>([&] { t = cv[size_t(-1)]; });
			Assert::AreEqual(cv[0], cv.Front());
		}
		TEMPLATE_TEST_METHOD_END(Subscript)
#pragma endregion
	};
}
