#include "pch.h"

// Macros which expands to defining a recursive template method.
// The purpose of this is to run a TEST_METHOD but with different data types.
// I managed to make one macro which combined these, but intellisense didn't work for blocks within them.
#define TEMPLATE_TEST_METHOD_BEGIN(methodName)								\
	template<typename... Ts>												\
	typename std::enable_if<sizeof...(Ts) == 0>::type methodName##__() {}	\
	template<typename TKey, typename TValue, typename... Ts>				\
	void methodName##__() {													\
		using Container = HashMap<TKey, TValue>;							\
		using KVP = typename Container::value_type;

#define TEMPLATE_TEST_METHOD_END(methodName)																																																							\
	methodName##__<Ts...>(); }																																																											\
	static const EXPORT_METHOD ::Microsoft::VisualStudio::CppUnitTestFramework::MemberMethodInfo* CALLING_CONVENTION CATNAME(__GetTestMethodInfo_, methodName)() {																										\
		__GetTestClassInfo(); __GetTestVersion();																																																						\
		ALLOCATE_TESTDATA_SECTION_METHOD																																																								\
		static const ::Microsoft::VisualStudio::CppUnitTestFramework::MethodMetadata s_Metadata = {L"TestMethodInfo", L#methodName, reinterpret_cast<const unsigned char*>(__FUNCTION__), reinterpret_cast<const unsigned char*>(__FUNCDNAME__), __WFILE__, __LINE__};	\
		static ::Microsoft::VisualStudio::CppUnitTestFramework::MemberMethodInfo s_Info = {::Microsoft::VisualStudio::CppUnitTestFramework::MemberMethodInfo::TestMethod, NULL, &s_Metadata};																			\
		s_Info.method.pVoidMethod = reinterpret_cast<::Microsoft::VisualStudio::CppUnitTestFramework::TestClassImpl::__voidFunc>(&ThisClass::methodName);																												\
		return &s_Info; }																																																												\
	void methodName() { methodName##__<																																																									\
		int, std::string>();}

using namespace std::string_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;

template<typename T>
using Rand = UnitTests::Random<T>;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashMapTests)
	{
		INITIALIZE_CLEANUP
		
#pragma region Constructors
		TEMPLATE_TEST_METHOD_BEGIN(Constructor)
		{
			Container c1;
			Container c2(5);
		}
		TEMPLATE_TEST_METHOD_END(Constructor)

		TEST_METHOD(RangeConstructor)
		{
			using Container = HashMap<int, std::string>;
			using KVP = typename Container::value_type;
			
			std::vector<KVP> v
			{
				{1, Rand<std::string>::Next() },
				{2, Rand<std::string>::Next() },
				{3, Rand<std::string>::Next() },
				{4, Rand<std::string>::Next() }
			};
			Container c(v);
			Assert::AreEqual(v.size(), c.Size());
			for (const auto& [key, value] : v)
			{
				Assert::AreEqual(value, c.At(key));
			}
		}

		TEST_METHOD(IteratorConstructor)
		{
			using Container = HashMap<int, std::string>;
			using KVP = typename Container::value_type;

			std::vector<KVP> v
			{
				{1, Rand<std::string>::Next() },
				{2, Rand<std::string>::Next() },
				{3, Rand<std::string>::Next() },
				{4, Rand<std::string>::Next() }
			};
			Container c(v.begin(), v.end());
			Assert::AreEqual(v.size(), c.Size());
			for (const auto& [key, value] : v)
			{
				Assert::AreEqual(value, c.At(key));
			}
		}

		TEST_METHOD(RangeAssignmentOperator)
		{
			using Container = HashMap<int, std::string>;
			using KVP = typename Container::value_type;

			std::vector<KVP> v
			{
				{1, Rand<std::string>::Next() },
				{2, Rand<std::string>::Next() },
				{3, Rand<std::string>::Next() },
				{4, Rand<std::string>::Next() }
			};
			Container c;
			c = v;
			Assert::AreEqual(v.size(), c.Size());
			for (const auto& [key, value] : v)
			{
				Assert::AreEqual(value, c.At(key));
			}
		}

		TEMPLATE_TEST_METHOD_BEGIN(CopyConstructor)
		{
			// Ensure we hit the code of copy-constructing self.
			Container map = map;
			map = Rand<Container>::Next();
			Container map2(map);
		}
		TEMPLATE_TEST_METHOD_END(CopyConstructor)

		TEMPLATE_TEST_METHOD_BEGIN(AssignmentOperator)
		{
			Container a, b;

			KVP pair = { Rand<TKey>::Next(), Rand<TValue>::Next() };
			a.Insert(pair);
			// Make sure we hit the code of self-assignment.
			a = a;
			Assert::AreEqual(pair.value, a[pair.key]);

			b.Insert({ Rand<TKey>::Next(), Rand<TValue>::Next() });
			b = a;

			Assert::AreEqual(a, b);
		}
		TEMPLATE_TEST_METHOD_END(AssignmentOperator)
		
		TEMPLATE_TEST_METHOD_BEGIN(InitializerList)
		{
			Array<TKey> unique = RandomUnique<Array<TKey>>(5);
			std::initializer_list<KVP> list
			{
				{ unique[0], Rand<TValue>::Next() },
				{ unique[1], Rand<TValue>::Next() },
				{ unique[2], Rand<TValue>::Next() },
				{ unique[3], Rand<TValue>::Next() },
				{ unique[4], Rand<TValue>::Next() }
			};
			Container c(list);

			Assert::AreEqual(list.size(), c.Size());

			for (const auto& [key, value] : list)
			{
				Assert::IsTrue(c.Contains(key));
			}

			unique = RandomUnique<Array<TKey>>(4);
			list =
			{
				{ unique[0], Rand<TValue>::Next() },
				{ unique[1], Rand<TValue>::Next() },
				{ unique[2], Rand<TValue>::Next() },
				{ unique[3], Rand<TValue>::Next() }
			};
			c = list;

			Assert::AreEqual(list.size(), c.Size());

			for (const auto& [key, value] : list)
			{
				Assert::IsTrue(c.Contains(key));
			}
		}
		TEMPLATE_TEST_METHOD_END(InitializerList)

		TEMPLATE_TEST_METHOD_BEGIN(CopyCtorDifferentReserve)
		{
			using OtherContainer = HashMap<TKey, TValue, Hash<TKey>, std::equal_to<TKey>, Util::DefaultReserveStrategy>;

			const auto other = Rand<OtherContainer>::Next();
			const Container c1(other);
			Container c2(c1);
			Assert::AreEqual(c1, c2);
			
			TKey key;
			do
			{
				key = Rand<TKey>::Next();
			} while (c1.Contains(key));
			
			c2.Insert(key, Rand<TValue>::Next());
			Assert::AreNotEqual(c1, c2);
		}
		TEMPLATE_TEST_METHOD_END(CopyCtorDifferentReserve)

		TEMPLATE_TEST_METHOD_BEGIN(MoveCtorDifferentReserve)
		{
			using OtherContainer = HashMap<TKey, TValue, Hash<TKey>, std::equal_to<TKey>, Util::DefaultReserveStrategy>;

			const Container other = Rand<OtherContainer>::Next();
			Container c(other);
			Assert::AreEqual(other, c);
			c.Insert(Rand<TKey>::NotIn(other), Rand<TValue>::Next());
			Assert::AreNotEqual(other, c);
		}
		TEMPLATE_TEST_METHOD_END(MoveCtorDifferentReserve)

		TEMPLATE_TEST_METHOD_BEGIN(CopyAssignDifferentReserve)
		{
			using OtherContainer = HashMap<TKey, TValue, Hash<TKey>, std::equal_to<TKey>, Util::DefaultReserveStrategy>;

			const OtherContainer other = Rand<OtherContainer>::Next();
			Container c;
			c = other;
			Assert::AreEqual(other, c);

			TKey key;
			do
			{
				key = Rand<TKey>::Next();
			} while (other.Contains(key));
			
			c.Insert(key, Rand<TValue>::Next());
			Assert::AreNotEqual(other, c);
		}
		TEMPLATE_TEST_METHOD_END(CopyAssignDifferentReserve)

		TEMPLATE_TEST_METHOD_BEGIN(MoveAssignDifferentReserve)
		{
			using OtherContainer = HashMap<TKey, TValue, Hash<TKey>, std::equal_to<TKey>, Util::DefaultReserveStrategy>;

			Container other;
			other = Rand<OtherContainer>::Next();
			Container c = other;
			Assert::AreEqual(other, c);
			c.Insert(Rand<TKey>::NotIn(other), Rand<TValue>::Next());
			Assert::AreNotEqual(other, c);
		}
		TEMPLATE_TEST_METHOD_END(MoveAssignDifferentReserve)
#pragma endregion

#pragma region Iterator
		TEMPLATE_TEST_METHOD_BEGIN(Iterator)
		{
			using iterator = typename Container::iterator;
			
			Container map;
			std::unordered_map<TKey, TValue> stdMap;

			FillRandomly(map, stdMap);

			iterator it, jt;
			auto stdIt = stdMap.begin();

			it = map.begin();

			Assert::IsTrue(it);
			Assert::IsFalse(!it);

			// Make sure pre/post increment behave differently.
			KVP a = *++it;
			it = map.begin();
			KVP b = *it++;
			Assert::AreNotEqual(a, b);
			++stdIt;

			//// Manual pass through the iterators to make sure they are the same.
			while (it != map.end() && stdIt != stdMap.end())
			{
				KVP kvp = *it++;
				auto stdKVP = *stdIt++;
				// Can't compare the iterators themselves because we can't be guaranteed of ordering.
				Assert::AreEqual(map[stdKVP.first], stdKVP.second, L"Make sure map has stdMap's values");
				Assert::AreEqual(stdMap[kvp.key], kvp.value, L"Make sure stdMap has map's values");
			}

			Assert::AreEqual(it, map.end(), L"Make sure Iterator is at end");
			Assert::AreEqual(stdIt, stdMap.end(), L"Make sure iterator is at end");

			// Now make sure range-based for-loops work.
			it = map.begin();
			for (const auto& pair : map)
			{
				Assert::AreEqual(pair, *it++);
			}

			// Make certain we compare some iterators in the middle of the container instead of just at the end.
			it = map.begin();
			jt = map.begin();
			Assert::AreEqual(++it, ++jt);
		}
		TEMPLATE_TEST_METHOD_END(Iterator)

		TEMPLATE_TEST_METHOD_BEGIN(ConstIterator)
		{
			using const_iterator = typename Container::const_iterator;
			Container map;
			std::unordered_map<TKey, TValue> stdMap;

			Assert::IsTrue(Container().cbegin().IsAtBegin());
			Assert::IsTrue(Container().cend().IsAtEnd());
			
			FillRandomly(map, stdMap);

			const_iterator it, jt;
			typename std::unordered_map<TKey, TValue>::const_iterator stdIt = stdMap.cbegin();

			it = map.cbegin();

			Assert::IsTrue(it);
			Assert::IsFalse(!it);

			// Make sure pre/post increment behave differently.
			KVP a = *++it;
			it = map.cbegin();
			KVP b = *it++;
			Assert::AreNotEqual(a, b);
			++stdIt;

			//// Manual pass through the iterators to make sure they are the same.
			while (it != map.cend() && stdIt != stdMap.cend())
			{
				KVP kvp = *it++;
				auto stdKVP = *stdIt++;
				// Can't compare the iterators themselves because we can't be guaranteed of ordering.
				Assert::AreEqual(map[stdKVP.first], stdKVP.second, L"Make sure map has stdMap's values");
				Assert::AreEqual(stdMap[kvp.key], kvp.value, L"Make sure stdMap has map's values");
			}

			Assert::AreEqual(it, map.cend(), L"Make sure Iterator is at end");
			Assert::AreEqual(stdIt, stdMap.cend(), L"Make sure iterator is at end");

			// Now make sure range-based for-loops work.
			it = map.cbegin();
			for (const auto& pair : map)
			{
				Assert::AreEqual(pair, *it++);
			}

			// Make certain we compare some iterators in the middle of the container instead of just at the end.
			it = map.cbegin();
			jt = map.cbegin();
			Assert::AreEqual(++it, ++jt);

			Assert::AreEqual(map.cbegin()->key, map.cbegin()->key);
		}
		TEMPLATE_TEST_METHOD_END(ConstIterator)
#pragma endregion

#pragma region Properties
		TEMPLATE_TEST_METHOD_BEGIN(IsEmpty)
		{
			Container map;
			Assert::IsTrue(map.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(IsEmpty)
#pragma endregion
		
#pragma region Query
		TEMPLATE_TEST_METHOD_BEGIN(Contains)
		{
			KVP pair = { Rand<TKey>::Next(), Rand<TValue>::Next() };
			Container map;
			Assert::IsFalse(map.Contains(pair.key));
			map = { pair };
			Assert::IsTrue(map.Contains(pair.key));
		}
		TEMPLATE_TEST_METHOD_END(Contains)

		TEMPLATE_TEST_METHOD_BEGIN(Exists)
		{
			const auto c = Rand<Container>::Next();
			const auto t = Library::Random::Element(c);
			Assert::IsTrue(Util::Exists(c, [&t](const auto& a) { return a == t; }));
		}
		TEMPLATE_TEST_METHOD_END(Exists)

		TEMPLATE_TEST_METHOD_BEGIN(Find)
		{
			Container map;

			Assert::AreEqual(map.end(), map.Find(Rand<TKey>::Next()), L"A map with no storage should always Find end");

			map = Rand<Container>::Next();

			auto pair = Library::Random::Element(map);
			auto it = map.Find(pair.key);

			Assert::AreEqual(pair, *it);

			const Container cmap = Rand<Container>::Next();

			auto cpair = Library::Random::Element(cmap);
			auto cit = cmap.Find(cpair.key);

			Assert::AreEqual(cpair, *cit);
		}
		TEMPLATE_TEST_METHOD_END(Find)
#pragma endregion

#pragma region Insert
		TEMPLATE_TEST_METHOD_BEGIN(Insert)
		{
			Container map;
			KVP pair = { Rand<TKey>::Next(), Rand<TValue>::Next() };
			map.Insert(pair.key, pair.value);
			Assert::IsTrue(map.Contains(pair.key));
		}
		TEMPLATE_TEST_METHOD_END(Insert)
		
		TEMPLATE_TEST_METHOD_BEGIN(InsertPair)
		{
			Container map;
			KVP pair = { Rand<TKey>::Next(), Rand<TValue>::Next() };
			map.Insert(pair);
			Assert::IsTrue(map.Contains(pair.key));
		}
		TEMPLATE_TEST_METHOD_END(InsertPair)

		TEMPLATE_TEST_METHOD_BEGIN(Emplace)
		{
			Container map;
			KVP pair = { Rand<TKey>::Next(), Rand<TValue>::Next() };
			map.Emplace(pair);
			Assert::IsTrue(map.Contains(pair.key));
			
			const auto oldValue = pair.value;
			pair.value = Rand<TValue>::NotEqualTo(oldValue);
			map.Emplace(pair);
			Assert::AreNotEqual(oldValue, map.At(pair.key));
		}
		TEMPLATE_TEST_METHOD_END(Emplace)

		TEMPLATE_TEST_METHOD_BEGIN(TryEmplace)
		{
			Container map;
			KVP pair = { Rand<TKey>::Next(), Rand<TValue>::Next() };
			map.TryEmplace(pair);
			Assert::IsTrue(map.Contains(pair.key));

			const auto oldValue = pair.value;
			pair.value = Rand<TValue>::NotEqualTo(oldValue);
			map.TryEmplace(pair);
			Assert::AreEqual(oldValue, map.At(pair.key));
		}
		TEMPLATE_TEST_METHOD_END(TryEmplace)
#pragma endregion

#pragma region Remove
		TEMPLATE_TEST_METHOD_BEGIN(Clear)
		{
		}
		TEMPLATE_TEST_METHOD_END(Clear)
		
		TEMPLATE_TEST_METHOD_BEGIN(Remove)
		{
			Container map;
			KVP pair = { Rand<TKey>::Next(), Rand<TValue>::Next() };
			Assert::IsFalse(map.Remove(pair.key));
			map.Insert(pair);
			Assert::IsTrue(map.Contains(pair.key));
			Assert::IsTrue(map.Remove(pair.key));
			Assert::IsFalse(map.Contains(pair.key));
		}
		TEMPLATE_TEST_METHOD_END(Remove)

		TEMPLATE_TEST_METHOD_BEGIN(RemoveIterator)
		{
			Container c = Rand<Container>::Next();
			auto [key, value] = Library::Random::Element(c);
			typename Container::const_iterator it = c.Find(key);
			c.Remove(it);
			Assert::IsFalse(c.Contains(key));
		}
		TEMPLATE_TEST_METHOD_END(RemoveIterator)

		TEMPLATE_TEST_METHOD_BEGIN(RemoveRange)
		{
			Container c = Rand<Container>::Next();
			c.Remove(c.begin(), c.end());
			Assert::IsTrue(c.IsEmpty());

			c = Rand<Container>::Next();
			c.Remove(++c.begin(), c.end());
			Assert::IsTrue(c.Size() == 1);
		}
		TEMPLATE_TEST_METHOD_END(RemoveRange)
#pragma endregion

#pragma region Accessors
		TEMPLATE_TEST_METHOD_BEGIN(At)
		{
			Container map;

			TKey key = Rand<TKey>::Next();
			TValue value;
			Assert::ExpectException<std::out_of_range>([&] { value = map.At(key); });

			value = Rand<TValue>::Next();
			map[key] = value;
			Assert::AreEqual(value, map.At(key));

			KVP pair = { Rand<TKey>::Next(), Rand<TValue>::Next() };
			const Container cmap = { pair };
			//key = Rand<TKey>::NotEqualTo(pair.key);
			//Assert::ExpectException<std::out_of_range>([&] { value = cmap.At(key); });
			//Assert::AreEqual(cmap.At(pair.key), pair.value);
		}
		TEMPLATE_TEST_METHOD_END(At)
#pragma endregion

#pragma region Memory
		TEMPLATE_TEST_METHOD_BEGIN(Resize)
		{
			KVP pair = { Rand<TKey>::Next(), Rand<TValue>::Next() };
			Container map(10);
			Assert::AreEqual(10_z, map.BucketCount());

			map.Resize(1);
			Assert::AreEqual(size_t(1), map.BucketCount());

			map = { pair };
			Assert::ExpectException<std::invalid_argument>([&] { map.Resize(0); });

			map.Resize(10);
			Assert::AreEqual(10_z, map.BucketCount());
		}
		TEMPLATE_TEST_METHOD_END(Resize)

		TEMPLATE_TEST_METHOD_BEGIN(PrimeReserveStrategy)
		{
			Container c;
			Assert::AreEqual(1_z, c.BucketCount());
			
			c.Insert({ Rand<TKey>::NotIn(c), Rand<TValue>::Next() });
			c.Insert({ Rand<TKey>::NotIn(c), Rand<TValue>::Next() });
			Assert::AreEqual(3_z, c.BucketCount());

			c.Insert({ Rand<TKey>::NotIn(c), Rand<TValue>::Next() });
			c.Insert({ Rand<TKey>::NotIn(c), Rand<TValue>::Next() });
			Assert::AreEqual(5_z, c.BucketCount());

			c.Insert({ Rand<TKey>::NotIn(c), Rand<TValue>::Next() });
			c.Insert({ Rand<TKey>::NotIn(c), Rand<TValue>::Next() });
			Assert::AreEqual(7_z, c.BucketCount());

			c.Insert({ Rand<TKey>::NotIn(c), Rand<TValue>::Next() });
			c.Insert({ Rand<TKey>::NotIn(c), Rand<TValue>::Next() });
			c.Insert({ Rand<TKey>::NotIn(c), Rand<TValue>::Next() });
			c.Insert({ Rand<TKey>::NotIn(c), Rand<TValue>::Next() });
			Assert::AreEqual(11_z, c.BucketCount());
		}
		TEMPLATE_TEST_METHOD_END(PrimeReserveStrategy)

		TEMPLATE_TEST_METHOD_BEGIN(Swap)
		{
			Container a = Rand<Container>::Next();
			Container b = Rand<Container>::Next();
			const auto& aRef = Library::Random::Element(a);
			const auto& bRef = Library::Random::Element(b);
			a.Swap(b);
			Assert::IsTrue(b.Contains(aRef.key));
			Assert::IsTrue(a.Contains(bRef.key));
		}
		TEMPLATE_TEST_METHOD_END(Swap)

		TEST_METHOD(Merge)
		{
			HashMap<int, std::string> a
			{
				{ 0, "0" },
				{ 1, "1" },
				{ 2, "2" },
				{ 3, "3" },
				{ 4, "4" }
			};
			HashMap<int, std::string> b
			{
				{ 5, "5" },
				{ 6, "6" },
				{ 7, "7" },
				{ 8, "8" },
				{ 9, "9" }
			};
			a.Merge(b);
			Assert::IsTrue(b.IsEmpty());
			Assert::AreEqual("0"s, a[0]);
			Assert::AreEqual("1"s, a[1]);
			Assert::AreEqual("2"s, a[2]);
			Assert::AreEqual("3"s, a[3]);
			Assert::AreEqual("4"s, a[4]);
			Assert::AreEqual("5"s, a[5]);
			Assert::AreEqual("6"s, a[6]);
			Assert::AreEqual("7"s, a[7]);
			Assert::AreEqual("8"s, a[8]);
			Assert::AreEqual("9"s, a[9]);
			
			HashMap<int, std::string> c
			{
				{ 5, "5" },
				{ 6, "6" },
				{ 7, "7" },
				{ 8, "8" },
				{ 9, "9" },
				{ 10, "10" }
			};
			a.Merge(c);
			Assert::AreEqual(5_z, c.Size(), "shouldn't insert duplicate keys");
		}

		TEMPLATE_TEST_METHOD_BEGIN(Invert)
		{
			const auto key = Rand<TKey>::Next();
			const auto value = Rand<TValue>::Next();
			Container c{ { key, value }	};
			const auto inverted = c.Invert();
			Assert::AreEqual(key, inverted[value]);
		}
		TEMPLATE_TEST_METHOD_END(Invert)
#pragma endregion

#pragma region Operators
		TEMPLATE_TEST_METHOD_BEGIN(Subscript)
		{
			Container map;

			TKey key = Rand<TKey>::Next();
			TValue value = map[key];
			Assert::AreEqual(value, TValue(), L"Make sure operator[] default constructs.");

			value = Rand<TValue>::Next();
			map[key] = value;
			Assert::AreEqual(value, map[key], L"Make sure operator[] sets values.");

			const Container cmap = map;
			auto pair = Library::Random::Element(map);
			Assert::AreEqual(cmap[pair.key], pair.value, L"Should be able to find a value in a copy map.");

			const Container emptyCmap;
			Assert::ExpectException<std::out_of_range>([&] { value = emptyCmap[key]; }, L"const operator[] should throw exceptions.");
		}
		TEMPLATE_TEST_METHOD_END(Subscript)

		TEMPLATE_TEST_METHOD_BEGIN(Equivalence)
		{
			// Not using AreEqual and AreNotEqual because I want to make sure operator== and operator!= are called by invoking them manually.
			Container a;

			Assert::IsTrue(a == a);
			Assert::IsFalse(a != a);

			Container b = a;
			const KVP kvp = { Rand<TKey>::Next(), Rand<TValue>::Next() };
			a.Insert(kvp);

			Assert::IsFalse(a == b);
			Assert::IsTrue(a != b);

			b.Insert({ Rand<TKey>::NotEqualTo(kvp.key), Rand<TValue>::Next() });

			Assert::IsFalse(a == b);
			Assert::IsTrue(a != b);
		}
		TEMPLATE_TEST_METHOD_END(Equivalence)
#pragma endregion

#pragma region String
		TEST_METHOD(ToString)
		{
			using Container = HashMap<std::string, int>;
			using KVP = typename Container::value_type;

			const KVP kvp{ "hello", 1 };
			std::stringstream stream;
			stream << kvp;
			Assert::AreEqual(stream.str(), "{ hello, 1 }"s);
		}
#pragma endregion
	};
}
