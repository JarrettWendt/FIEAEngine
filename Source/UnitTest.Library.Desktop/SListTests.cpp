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
	void methodName() { methodName##__<char, int, float, double, std::string, Array<int>>(); }

using namespace std::string_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;

template<typename T>
using Rand = UnitTests::Random<T>;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListTests)
	{
		template<typename T>
		using Container = SList<T>;

		INITIALIZE_CLEANUP

#pragma region Constructors
		TEMPLATE_TEST_METHOD_BEGIN(InitializerList)
		{
			T a = UnitTests::Random<T>::Next();
			T b = UnitTests::Random<T>::Next();
			Container<T> v{ a, b };
			
			Assert::AreEqual(a, v.Front(), "Front Constructor");
			Assert::AreEqual(b, v.Back(), "Back Constructor");

			v = { a, b };

			Assert::AreEqual(a, v.Front(), "Front Assignment");
			Assert::AreEqual(b, v.Back(), "Back Assignment");
		}
		TEMPLATE_TEST_METHOD_END(InitializerList)

		TEMPLATE_TEST_METHOD_BEGIN(RangeCtor)
		{
			const std::vector<T> v = Library::Random::Next<std::vector<T>>();
			const Container<T> a(v);
			Assert::AreEqual(v.size(), a.Size());
			auto it = v.begin();
			auto jt = a.begin();
			while (it != v.end())
			{
				Assert::AreEqual(*it++, *jt++);
			}
		}
		TEMPLATE_TEST_METHOD_END(RangeCtor)

		TEMPLATE_TEST_METHOD_BEGIN(RangeAssignmentOperator)
		{
			const std::vector<T> v = Library::Random::Next<std::vector<T>>();
			Container<T> a;
			a = v;
			Assert::AreEqual(v.size(), a.Size());
			auto it = v.begin();
			auto jt = a.begin();
			while (it != v.end())
			{
				Assert::AreEqual(*it++, *jt++);
			}
		}
		TEMPLATE_TEST_METHOD_END(RangeAssignmentOperator)
#pragma endregion

#pragma region iterator
		TEMPLATE_TEST_METHOD_BEGIN(iterator)
		{
			Container<T> slist;
			std::list<T> list;

			// Fill up the lists with some values.
			FillRandomlyUnique(slist, list);

			typename SList<T>::iterator sit = slist.begin();
			typename std::list<T>::iterator it = list.begin();

			if constexpr (std::is_same_v<std::string, T>)
			{
				Assert::IsTrue(sit->empty() || !sit->empty(), "operator->");
			}

			Assert::IsFalse(!sit);

			// Make sure pre/post increment behave differently.
			T a = *++sit;
			sit = slist.begin();
			T b = *sit++;

			Assert::AreNotEqual(a, b, std::wstringstream() << "Making sure pre/post increments behave differently, received " << Util::ToWString(a) << " from pre and " << Util::ToWString(b) << " from post");
			++it;

			while (sit != slist.end() && it != list.end())
			{
				Assert::AreEqual(*sit++, *it++);
			}

			Assert::AreEqual(sit, slist.end());
			Assert::AreEqual(it, list.end());

			// Now make sure the range-based for loop works.
			it = list.begin();
			for (const T& t : slist)
			{
				Assert::AreEqual(t, *it++);
			}

			Assert::ExpectException<std::out_of_range>([&] { *(slist.end()) = Rand<T>::Next(); });
		}
		TEMPLATE_TEST_METHOD_END(iterator)

		TEMPLATE_TEST_METHOD_BEGIN(const_iterator)
		{
			SList<T> slist;
			std::list<T> list;

			// Fill up the lists with some values.
			FillRandomlyUnique(slist, list);

			// Make const copies.
			const SList<T> cslist = slist;
			const std::list<T> clist = list;

			typename SList<T>::const_iterator sit = cslist.begin();
			typename std::list<T>::const_iterator it = clist.cbegin();

			if constexpr (std::is_same_v<std::string, T>)
			{
				Assert::IsTrue(sit->empty() || !sit->empty(), "operator->");
			}

			Assert::IsTrue(!!sit);
			Assert::IsFalse(!sit);

			Assert::IsFalse(sit.IsAtEnd());

			// Make sure pre/post increment behave differently.
			T a = *++sit;
			sit = cslist.cbegin();
			T b = *sit++;

			Assert::AreNotEqual(a, b, std::wstringstream() << "Making sure pre/post increments behave differently, received " << Util::ToWString(a) << " from pre and " << Util::ToWString(b) << " from post");
			++it;

			while (sit != cslist.cend() && it != clist.cend())
			{
				Assert::AreEqual(*sit++, *it++);
			}

			Assert::AreEqual(sit, cslist.cend(), L"Make sure const_iterator is at end");
			Assert::AreEqual(it, clist.end(), L"Make sure const_iterator is at end");

			// Now make sure the range-based for loop works.
			it = clist.cbegin();
			for (const T& t : cslist)
			{
				Assert::AreEqual(t, *it++);
			}

			// Make sure * throws exceptions.
			Assert::ExpectException<std::out_of_range>([&] { *(cslist.cend()) = Rand<T>::Next(); });
		}
		TEMPLATE_TEST_METHOD_END(const_iterator)

		TEMPLATE_TEST_METHOD_BEGIN(BeforeBegin)
		{
			Container<T> c = Library::Random::Next<Container<T>>();
			const auto t = Library::Random::Next<T>();
			c.InsertAfter(c.cbefore_begin(), t);
			Assert::AreEqual(t, c.Front());
		}
		TEMPLATE_TEST_METHOD_END(BeforeBegin)

		TEMPLATE_TEST_METHOD_BEGIN(BeforeEnd)
		{
			Container<T> c = Library::Random::Next<Container<T>>();
			const auto t = Library::Random::Next<T>();
			c.InsertAfter(c.cbefore_end(), t);
			Assert::AreEqual(t, c.Back());
		}
		TEMPLATE_TEST_METHOD_END(BeforeEnd)
#pragma endregion

#pragma region Query
		TEMPLATE_TEST_METHOD_BEGIN(Contains)
		{
			const T a = Rand<T>::Next();
			Container<T> c = { a };
			Assert::IsTrue(Util::Contains(c, a));
			T b = Rand<T>::NotEqualTo(a);
			Assert::IsFalse(Util::Contains(c, b));
		}
		TEMPLATE_TEST_METHOD_END(Contains)

		TEMPLATE_TEST_METHOD_BEGIN(Exists)
		{
			const auto c = Library::Random::Next<Container<T>>();
			const auto t = Library::Random::Element(c);
			Assert::IsTrue(Util::Exists(c, [&t](const auto& a) { return t == a; }));
		}
		TEMPLATE_TEST_METHOD_END(Exists)

		TEMPLATE_TEST_METHOD_BEGIN(Find)
		{
			const Container<T> c;
			Assert::AreEqual(Util::Find(c, T()), c.begin());
			T t;
			Assert::AreEqual(Util::Find(c, t), c.begin());			
		}
		TEMPLATE_TEST_METHOD_END(Find)

		TEMPLATE_TEST_METHOD_BEGIN(UtilFind)
		{
			Container<T> c = Library::Random::Next<Container<T>>(3);
			const auto t = Library::Random::Element(c);
			Assert::AreEqual(Util::Find(c, t), Util::Find(c, [t](const auto& a) { return t == a; }));
		}
		TEMPLATE_TEST_METHOD_END(UtilFind)

		TEMPLATE_TEST_METHOD_BEGIN(FindPrev)
		{
			const Container<T> c;
			Assert::AreEqual(c.FindPrev(T()), c.before_begin());
			T t;
			Assert::AreEqual(c.FindPrev(t), c.before_begin());
			Assert::AreEqual(c.FindPrev([&t](const auto& a) { return t == a; }), c.before_begin());
		}
		TEMPLATE_TEST_METHOD_END(FindPrev)
#pragma endregion

#pragma region Insert
		TEMPLATE_TEST_METHOD_BEGIN(PushFront)
		{
			Container<T> list;
			T data = Library::Random::Next<T>();
			const T constCopy(data);
			list.PushFront(data);
			// data should not have changed
			Assert::AreEqual(data, constCopy);
			Assert::IsFalse(list.IsEmpty());
			// coverage for rvalue PushFront
			list.PushFront(Library::Random::Next<T>());
		}
		TEMPLATE_TEST_METHOD_END(PushFront)

		TEMPLATE_TEST_METHOD_BEGIN(PushBack)
		{
			Container<T> list;
			T data = Library::Random::Next<T>();
			const T constCopy(data);
			list.PushBack(data);
			// data should not have changed
			Assert::AreEqual(data, constCopy);
			Assert::IsFalse(list.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(PushBack)

		TEMPLATE_TEST_METHOD_BEGIN(InsertAfter)
		{
			Container<T> a = RandomUnique<Container<T>>();
			Container<T> b = RandomUnique<Container<T>>();
			const T elementToInsertAfter = Library::Random::Element(a);
			T elementToInsert = Library::Random::Next<T>();

			auto it = Util::Find(a, elementToInsertAfter);

			a.InsertAfter(it, elementToInsert);

			Assert::AreEqual(*++it, elementToInsert);

			elementToInsert = Library::Random::Next<T>();
			const auto jt = a.InsertAfter(it, { elementToInsert });
			Assert::AreEqual(*++it, elementToInsert, L"make sure the returned iterator is at the newly inserted element");
			Assert::AreEqual(jt, it);
			
			b.Clear();
			Assert::IsTrue(b.IsEmpty(), L"b should be empty after Clear()");
			
			b = a;
			Assert::AreEqual(a, b, L"a and b should be the same after operator=");

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
			std::stringstream stream;
			stream << "Counted " << Util::ToString(aCount) << " elements in a and " << Util::ToString(bCount) << " elements in b after b = a";
			Assert::AreEqual(aCount, bCount, stream);
			stream.str("");

			a.Clear();
			b.Clear();
			b.PushBack(elementToInsert);
			a.InsertAfter(a.end(), elementToInsert);
			Assert::AreEqual(a, b);

			b.PushBack(elementToInsert);
			a.InsertAfter(a.end(), elementToInsert);
			Assert::AreEqual(a, b);

			b.PushBack(elementToInsert);
			a.InsertAfter(a.end(), elementToInsert);
			Assert::AreEqual(a, b);

			b.PushBack(elementToInsert);
			a.InsertAfter(a.end(), elementToInsert);
			Assert::AreEqual(a, b);

			b.PushBack(elementToInsert);
			a.InsertAfter(a.end(), elementToInsert);
			Assert::AreEqual(a, b);

			stream << "InsertAfter(end(), " << Util::ToString(elementToInsert) << ") ";
			stream << "should be the same as PushBack(" << Util::ToString(elementToInsert) << "), ";
			stream << "a.Size() = " << Util::ToString(a.Size()) << ", ";
			stream << "b.Size() = " << Util::ToString(b.Size());
			Assert::AreEqual(a, b, stream);
		}
		TEMPLATE_TEST_METHOD_END(InsertAfter)

		TEMPLATE_TEST_METHOD_BEGIN(InsertAfterForwardIterator)
		{
			Container<T> c = { Library::Random::Next<T>() };
			std::forward_list<T> l = { Rand<T>::Next() };
			c.InsertAfter(c.begin(), l.begin(), l.end());
			Assert::AreEqual(l.front(), *++c.begin());
		}
		TEMPLATE_TEST_METHOD_END(InsertAfterForwardIterator)
		
		TEMPLATE_TEST_METHOD_BEGIN(InsertAfterRValue)
		{
			Container<T> c;
			auto it = c.InsertAfter(c.begin(), Library::Random::Next<T>());
			Assert::AreEqual(c.begin(), it);
		}
		TEMPLATE_TEST_METHOD_END(InsertAfterRValue)

		TEMPLATE_TEST_METHOD_BEGIN(InsertAfterCount)
		{
			Container<T> c = Library::Random::Next<Container<T>>();
			const auto t = Library::Random::Next<T>();
			auto it = c.cbegin();
			for (size_t i = 1; i < c.Size(); i++, ++it);

			c.InsertAfter(it, 5_z, t);
			for (size_t i = 0; i < 5; i++)
			{
				Assert::AreEqual(t, *++it);
			}
		}
		TEMPLATE_TEST_METHOD_END(InsertAfterCount)

		TEMPLATE_TEST_METHOD_BEGIN(InsertAfterRange)
		{
			Container<T> a = Library::Random::Next<Container<T>>();
			Array<T> v = Library::Random::Next<Array<T>>();
			Container<T> b = a;
			
			a.Append(v.begin(), v.end());
			auto it = b.InsertAfter(b.end(), v.begin(), v.end());

			Assert::AreEqual(a, b);
			Assert::AreNotEqual(b.end(), it);
			Assert::AreEqual(b.end(), ++it);
		}
		TEMPLATE_TEST_METHOD_END(InsertAfterRange)

		TEMPLATE_TEST_METHOD_BEGIN(Append)
		{
			std::initializer_list<T> init =
			{
				Library::Random::Next<T>(),
				Library::Random::Next<T>(),
				Library::Random::Next<T>(),
				Library::Random::Next<T>()
			};
			Container<T> a = init, b;
			b.Append(init);
			Assert::AreEqual(a, b);

			b.Clear();
			b.Append(a);
			Assert::AreEqual(a, b);
		}
		TEMPLATE_TEST_METHOD_END(Append)

		TEMPLATE_TEST_METHOD_BEGIN(AppendList)
		{
			Container<T> a = Library::Random::Next<Container<T>>(), b;
			b.Append(a.begin(), a.end());
			Assert::AreEqual(a, b);
		}
		TEMPLATE_TEST_METHOD_END(AppendList)
#pragma endregion

#pragma region Remove
		TEMPLATE_TEST_METHOD_BEGIN(PopFront)
		{
			Container<T> slist;
			std::list<T> list;

			// Make sure nothing goes wrong when popping an empty list.
			slist.PopFront();

			// Insert some values to test their popping.
			FillRandomly(slist, list);

			// Pop those values, make sure they're in the same order.
			while (!slist.IsEmpty())
			{
				Assert::AreEqual(slist.Front(), list.front());
				slist.PopFront();
				list.pop_front();
			}

			// Should be empty now.
			Assert::IsTrue(slist.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(PopFront)

		TEMPLATE_TEST_METHOD_BEGIN(PopBack)
		{
			Container<T> slist;
			std::list<T> list;

			// Make sure nothing goes wrong when popping an empty list.
			slist.PopBack();

			// Insert some values to test their popping.
			FillRandomly(slist, list);

			// Pop those values, make sure they're in the same order.
			while (!slist.IsEmpty())
			{
				Assert::AreEqual(slist.Back(), list.back());
				slist.PopBack();
				list.pop_back();
			}

			// Should be empty now.
			Assert::IsTrue(slist.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(PopBack)

		TEMPLATE_TEST_METHOD_BEGIN(Clear)
		{
			Container<T> list;

			// Nothing should go wrong when clearing an empty list.
			list.Clear();

			// Make sure we can clear a list with one element.
			list.PushBack(T());
			list.Clear();
			Assert::IsTrue(list.IsEmpty());

			// Insert some values so that clearing will work.
			list = Library::Random::Next<Container<T>>();

			// Make sure that we can clear a list with many elements.
			list.Clear();
			Assert::IsTrue(list.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(Clear)

		TEMPLATE_TEST_METHOD_BEGIN(Remove)
		{
			Container<T> c;

			Assert::IsFalse(c.Remove(Library::Random::Next<T>()));

			c = RandomUnique<Container<T>>(5);

			T randT = Library::Random::Element(c);
			// Can't Assert::IsTrue on this first Remove because it could be at end()
			c.Remove(randT);
			Assert::IsFalse(Util::Contains(c, randT));
			Assert::IsFalse(c.Remove(randT));

			Assert::IsTrue(c.Remove(c.Front()), L"Should be able to remove the front");
			//Assert::IsFalse(c.Remove(c.Back()), L"Remove Back will always return false because it returns end()");

			while (c.Size() > 1)
			{
				Assert::IsTrue(c.Remove(c.Front()));
			}
			Assert::IsFalse(c.Remove(c.Front()));
		}
		TEMPLATE_TEST_METHOD_END(Remove)

		TEMPLATE_TEST_METHOD_BEGIN(RemoveAt)
		{
			SList<T> a = RandomUnique<Container<T>>();
			SList<T> b(a);

			auto it = b.end();
			a.PopBack();
			it = b.RemoveAt(it);
			Assert::AreEqual(a, b, L"Removing end should be the same as PopBack");
			Assert::AreEqual(it, b.end(), L"comparing it to end");

			it = b.begin();
			a.PopFront();
			it = b.RemoveAt(it);
			Assert::AreEqual(a, b, L"Removing begin should be the same as PopFront");
			Assert::AreEqual(it, b.begin(), L"comparing it to begin");

			it = a.begin();

			T t = Library::Random::Element(a);
			a.Remove(t);

			it = Util::Find(b, t);
			b.RemoveAt(it);

			Assert::AreEqual(a, b);

			while (!b.IsEmpty())
			{
				it = b.begin();
				t = *it;
				it = b.RemoveAt(it);
				Assert::IsFalse(Util::Contains(b, t));
				Assert::AreEqual(it, b.begin());
			}
			Assert::AreEqual(it, b.end());
		}
		TEMPLATE_TEST_METHOD_END(RemoveAt)

		TEMPLATE_TEST_METHOD_BEGIN(RemoveAll)
		{
			Container<T> c = Library::Random::Next<Container<T>>();
			auto t = c.Front();
			c.PushBack(t);
			Assert::IsTrue(c.RemoveAll(t) >= 2);
			Assert::IsFalse(Util::Contains(c, t));

			t = c.Front();
			c.PushBack(t);
			Assert::IsTrue(c.RemoveAll(std::move(t)) >= 2);
		}
		TEMPLATE_TEST_METHOD_END(RemoveAll)

		TEMPLATE_TEST_METHOD_BEGIN(RemoveAllAfter)
		{
			Container<T> c = Library::Random::Next<Container<T>>();
			c.RemoveAllAfter(c.begin());
			Assert::IsTrue(c.Size() == 1);
		}
		TEMPLATE_TEST_METHOD_END(RemoveAllAfter)

		TEMPLATE_TEST_METHOD_BEGIN(RemoveRange)
		{
			Assert::ExpectException<std::invalid_argument>([] { Container<T>().Remove(100, 0); });
		}
		TEMPLATE_TEST_METHOD_END(RemoveRange)

		TEMPLATE_TEST_METHOD_BEGIN(RemoveIterator)
		{
			Container<T> c = { Library::Random::Next<T>(), Library::Random::Next<T>(), Library::Random::Next<T>() };
			c.Remove(++c.cbegin(), c.cend());
			Assert::IsTrue(c.Size() == 1);

			c = Library::Random::Next<Container<T>>();
			c.Remove(c.cbegin(), c.cbefore_end());
			Assert::IsTrue(c.Size() == 1);
		}
		TEMPLATE_TEST_METHOD_END(RemoveIterator)
#pragma endregion
		
#pragma region Accessors
		TEMPLATE_TEST_METHOD_BEGIN(Front)
		{
			Container<T> v;
			const T t = UnitTests::Random<T>::Next();
			v.PushFront(t);
			Assert::AreEqual(t, v.Front(), L"Not same after PushFront");
			Assert::ExpectException<std::out_of_range>([] { Container<T>().Front(); });
			Assert::AreEqual(v.Front(), const_cast<const Container<T>&>(v).Front());
		}
		TEMPLATE_TEST_METHOD_END(Front)

		TEMPLATE_TEST_METHOD_BEGIN(Back)
		{
			Container<T> v;
			T t = UnitTests::Random<T>::Next();
			v.PushBack(t);
			Assert::AreEqual(t, v.Back(), L"Not same after PushFront");
			Assert::ExpectException<std::out_of_range>([] { Container<T>().Back(); });
			Assert::AreEqual(v.Back(), const_cast<const Container<T>&>(v).Back());
		}
		TEMPLATE_TEST_METHOD_END(Back)

		TEMPLATE_TEST_METHOD_BEGIN(At)
		{
			const Container<T> c = Library::Random::Next<Container<T>>();
			const auto& r = c;
			const auto index = 6;
			Assert::AreEqual(c.At(index), r.At(index));
			Assert::ExpectException<std::out_of_range>([] { Container<T>().At(std::numeric_limits<typename Container<T>::size_type>::max()); });
		}
		TEMPLATE_TEST_METHOD_END(At)
#pragma endregion

#pragma region Memory
		TEMPLATE_TEST_METHOD_BEGIN(ShrinkTo)
		{
			auto c = Library::Random::Next<Container<T>>();
			Assert::ExpectException<std::invalid_argument>([&c] { c.ShrinkTo(c.Size() + 1); });
			const auto size = c.Size() / 2;
			c.ShrinkTo(size);
			Assert::AreEqual(size, c.Size());
		}
		TEMPLATE_TEST_METHOD_END(ShrinkTo)
		
		TEMPLATE_TEST_METHOD_BEGIN(Resize)
		{
			auto c = Library::Random::Next<Container<T>>();
			const auto size = c.Size();
			const T t = Library::Random::Next<T>();
			
			c.Resize(size / 2);
			Assert::AreEqual(size / 2, c.Size(), "Resize should behave the same as ShrinkTo when passing a smaller size");

			c.Resize(size, t);
			Assert::AreEqual(size, c.Size());
			Assert::AreEqual(t, c.Back());
			
		}
		TEMPLATE_TEST_METHOD_END(Resize)
		
		TEMPLATE_TEST_METHOD_BEGIN(Swap)
		{
			auto a = Library::Random::Next<Container<T>>();
			auto b = Library::Random::Next<Container<T>>();
			const auto oldA = a;
			const auto oldB = b;
			a.Swap(b);
			Assert::AreEqual(oldB, a);
			Assert::AreEqual(oldA, b);
		}
		TEMPLATE_TEST_METHOD_END(Swap)

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

			const auto& ref = *++c.begin();
			const auto t = ref;

			list.reverse();
			c.Reverse();
			Assert::AreEqual(t, ref, "reference should not have been invalidated");
			Assert::AreEqual(c, Container<T>(list.begin(), list.end()));
		}
		TEMPLATE_TEST_METHOD_END(Reverse)

		TEMPLATE_TEST_METHOD_BEGIN(Sort)
		{
			// T::operator<= must be defined.
			if constexpr (std::is_arithmetic_v<T>)
			{
				auto a = Library::Random::Next<Array<T>>();
				Container<T> l(a.begin(), a.end());
				std::sort(a.begin(), a.end());
				l.Sort();
				Assert::AreEqual(Container<T>(a.begin(), a.end()), l);
			}
		}
		TEMPLATE_TEST_METHOD_END(Sort)

		TEMPLATE_TEST_METHOD_BEGIN(Merge)
		{
			// T::operator<= must be defined.
			if constexpr (std::is_arithmetic_v<T>)
			{
				auto a = Library::Random::Next<Array<T>>();
				Container<T> l;
				l.Merge(Container<T>(a.begin(), a.end()));
				Assert::AreEqual(Container<T>(a.begin(), a.end()), l);
			}
		}
		TEMPLATE_TEST_METHOD_END(Merge)
#pragma endregion
	};
}
