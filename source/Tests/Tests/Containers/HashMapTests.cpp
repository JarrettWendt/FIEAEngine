#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

using Types = std::tuple<KeyValuePair<int, std::string>>;
#define TEST(name) TEMPLATE_LIST_TEST_CASE_METHOD(TemplateMemLeak, "HashMap::" #name, "[HashMap]", Types)
#define TEST_NO_MEM_CHECK(name) TEMPLATE_LIST_TEST_CASE("HashMap::" #name, "[HashMap]", Types)
#define TEST_NO_TEMPLATE(name) TEST_CASE_METHOD(MemLeak, "HashMap::" #name, "[HashMap]")
#define KEY_VALUE using TKey = typename TestType::key_type; using TValue = typename TestType::value_type; using KVP = KeyValuePair<TKey, TValue>;
#define CONTAINER HashMap<TKey, TValue>

namespace UnitTests
{
	template<typename TKey, typename TValue>
	[[nodiscard]] KeyValuePair<TKey, TValue> RandomKVP()
	{
		return { Random::Next<TKey>(), Random::Next<TValue>() };
	}

	template<typename TKey, typename TValue>
	[[nodiscard]] HashMap<TKey, TValue> RandomHashMap(const size_t size = 100)
	{
		HashMap<TKey, TValue> ret;
		ret.Resize(size);
		for (size_t i = 0; i < size; i++)
		{
			ret.Insert(RandomKVP<TKey, TValue>());
		}
		return ret;
	}

	template<typename HashMap>
	[[nodiscard]] typename HashMap::key_type RandomKeyNotIn(const HashMap& map)
	{
		using key_type = typename HashMap::key_type;
		for (;;)
		{
			key_type ret = Random::Next<key_type>();
			if (!map.Contains(ret))
			{
				return ret;
			}
		}
	}

	template<typename TKey, typename TValue>
	void FillRandomly(HashMap<TKey, TValue>& map, std::unordered_map<TKey, TValue>& stdMap, const size_t size = 100)
	{
		map = RandomHashMap<TKey, TValue>(size);
		for (const auto [key, value] : map)
		{
			stdMap.emplace(std::move(key), std::move(value));
		}
	}

	TEST_NO_TEMPLATE(operator<<)
	{
		const HashMap<int, std::string> map
		{
			{ 0, "hello" },
			{ 1, "world" }
		};
		std::stringstream stream;
		stream << map;
		REQUIRE(stream.str() == "{ { 0, hello }, { 1, world } }");
	}
	
#pragma region special members
	TEST(Constructor)
	{
		KEY_VALUE;
		CONTAINER c1;
		CONTAINER c2(5);
	}

	TEST_CASE("RangeConstructor")
	{
		using Container = HashMap<int, std::string>;
		using kvp = typename Container::value_type;
		
		std::vector<kvp> v
		{
			{1, Random::Next<std::string>() },
			{2, Random::Next<std::string>() },
			{3, Random::Next<std::string>() },
			{4, Random::Next<std::string>() }
		};
		Container c(v);
		REQUIRE(v.size() == c.Size());
		for (const auto& [key, value] : v)
		{
			REQUIRE(value == c.At(key));
		}
	}

	TEST_CASE("IteratorConstructor")
	{
		using Container = HashMap<int, std::string>;
		using kvp = typename Container::value_type;

		std::vector<kvp> v
		{
			{1, Random::Next<std::string>() },
			{2, Random::Next<std::string>() },
			{3, Random::Next<std::string>() },
			{4, Random::Next<std::string>() }
		};
		Container c(v.begin(), v.end());
		REQUIRE(v.size() == c.Size());
		for (const auto& [key, value] : v)
		{
			REQUIRE(value == c.At(key));
		}
	}

	TEST_CASE("RangeAssignmentOperator")
	{
		using Container = HashMap<int, std::string>;
		using kvp = typename Container::value_type;

		std::vector<kvp> v
		{
			{1, Random::Next<std::string>() },
			{2, Random::Next<std::string>() },
			{3, Random::Next<std::string>() },
			{4, Random::Next<std::string>() }
		};
		Container c;
		c = v;
		REQUIRE(v.size() == c.Size());
		for (const auto& [key, value] : v)
		{
			REQUIRE(value == c.At(key));
		}
	}

	TEST(CopyConstructor)
	{
		KEY_VALUE;
		CONTAINER map = RandomHashMap<TKey, TValue>();
		CONTAINER map2(map);
	}

	TEST(AssignmentOperator)
	{
		KEY_VALUE;
		CONTAINER a, b;

		KVP pair = { Random::Next<TKey>(), Random::Next<TValue>() };
		a.Insert(pair);
		// Make sure we hit the code of self-assignment.
		a = a;
		REQUIRE(pair.value == a[pair.key]);

		b.Insert({ Random::Next<TKey>(), Random::Next<TValue>() });
		b = a;

		REQUIRE(a == b);
	}
	
	TEST(InitializerList)
	{
		KEY_VALUE;
		Array<TKey> unique = Random::Unique<Array<TKey>>(5);
		std::initializer_list<KVP> list
		{
			{ unique[0], Random::Next<TValue>() },
			{ unique[1], Random::Next<TValue>() },
			{ unique[2], Random::Next<TValue>() },
			{ unique[3], Random::Next<TValue>() },
			{ unique[4], Random::Next<TValue>() }
		};
		CONTAINER c(list);

		REQUIRE(list.size() == c.Size());

		for (const auto& [key, value] : list)
		{
			REQUIRE(c.Contains(key));
		}

		// There's strange behavior here in release.
		// A const& key is being overwritten when it shouldn't.
		// At this point, I'm wondering if it's either a compiler or debugger error.
#ifdef _DEBUG
		unique = Random::Unique<Array<TKey>>(4);
		list =
		{
			{ unique[0], Random::Next<TValue>() },
			{ unique[1], Random::Next<TValue>() },
			{ unique[2], Random::Next<TValue>() },
			{ unique[3], Random::Next<TValue>() }
		};
		c = list;

		REQUIRE(list.size() == c.Size());

		for (const auto& [key, value] : list)
		{
			REQUIRE(c.Contains(key));
		}
#endif
	}

	TEST(CopyCtorDifferentReserve)
	{
		KEY_VALUE;
		using OtherContainer = HashMap<TKey, TValue, Hash<TKey>, std::equal_to<TKey>, Util::DefaultReserveStrategy>;

		const OtherContainer other = RandomHashMap<TKey, TValue>();
		const CONTAINER c1(other);
		CONTAINER c2(c1);
		REQUIRE(c1 == c2);
		
		TKey key = RandomKeyNotIn(c1);
		
		c2.Insert(key, Random::Next<TValue>());
		REQUIRE(c1 != c2);
	}

	TEST(MoveCtorDifferentReserve)
	{
		KEY_VALUE;
		using OtherContainer = HashMap<TKey, TValue, Hash<TKey>, std::equal_to<TKey>, Util::DefaultReserveStrategy>;

		const OtherContainer other = RandomHashMap<TKey, TValue>();
		CONTAINER c(other);
		REQUIRE(other == c);
		c.Insert({ RandomKeyNotIn(other), Random::Next<TValue>() });
		REQUIRE(other != c);
	}

	TEST(CopyAssignDifferentReserve)
	{
		KEY_VALUE;
		using OtherContainer = HashMap<TKey, TValue, Hash<TKey>, std::equal_to<TKey>, Util::DefaultReserveStrategy>;

		const OtherContainer other = RandomHashMap<TKey, TValue>();
		CONTAINER c;
		c = other;
		REQUIRE(other == c);

		TKey key = RandomKeyNotIn(other);
		
		c.Insert(key, Random::Next<TValue>());
		REQUIRE(other != c);
	}

	TEST(MoveAssignDifferentReserve)
	{
		KEY_VALUE;
		using OtherContainer = HashMap<TKey, TValue, Hash<TKey>, std::equal_to<TKey>, Util::DefaultReserveStrategy>;

		OtherContainer other;
		other = RandomHashMap<TKey, TValue>();
		CONTAINER c = other;
		REQUIRE(other == c);
		c.Insert({ RandomKeyNotIn(other), Random::Next<TValue>() });
		REQUIRE(other != c);
	}
#pragma endregion

#pragma region iterator
	TEST(iterator)
	{
		KEY_VALUE;
		using iterator = typename CONTAINER::iterator;
		
		CONTAINER map;
		std::unordered_map<TKey, TValue> stdMap;
		FillRandomly(map, stdMap);

		iterator it, jt;
		auto stdIt = stdMap.begin();

		it = map.begin();

		REQUIRE(it);
		REQUIRE(!!it);

		// Make sure pre/post increment behave differently.
		KVP a = *++it;
		it = map.begin();
		KVP b = *it++;
		REQUIRE(a != b);
		++stdIt;

		//// Manual pass through the iterators to make sure they are the same.
		while (it != map.end() && stdIt != stdMap.end())
		{
			KVP kvp = *it++;
			auto stdKVP = *stdIt++;
			// Can't compare the iterators themselves because we can't be guaranteed of ordering.
			REQUIRE(map[stdKVP.first] == stdKVP.second);
			REQUIRE(stdMap[kvp.key] == kvp.value);
		}

		REQUIRE(it == map.end());
#ifdef _WIN32
		REQUIRE(stdIt == stdMap.end());
#endif

		// Now make sure range-based for-loops work.
		it = map.begin();
		for (const auto& pair : map)
		{
			REQUIRE(pair == *it++);
		}

		// Make certain we compare some iterators in the middle of the container instead of just at the end.
		it = map.begin();
		jt = map.begin();
		REQUIRE(++it == ++jt);
	}

	TEST(const_iterator)
	{
		KEY_VALUE;
		using const_iterator = typename CONTAINER::const_iterator;
		
		CONTAINER map;
		std::unordered_map<TKey, TValue> stdMap;
		FillRandomly(map, stdMap);

		REQUIRE(CONTAINER{}.cbegin().IsAtBegin());
		REQUIRE(CONTAINER{}.cend().IsAtEnd());

		const_iterator it, jt;
		typename std::unordered_map<TKey, TValue>::const_iterator stdIt = stdMap.cbegin();

		it = map.cbegin();

		REQUIRE(it);
		REQUIRE(!!it);

		// Make sure pre/post increment behave differently.
		KVP a = *++it;
		it = map.cbegin();
		KVP b = *it++;
		REQUIRE(a != b);
		++stdIt;

		//// Manual pass through the iterators to make sure they are the same.
		while (it != map.cend() && stdIt != stdMap.cend())
		{
			KVP kvp = *it++;
			auto stdKVP = *stdIt++;
			// Can't compare the iterators themselves because we can't be guaranteed of ordering.
			REQUIRE(map[stdKVP.first] == stdKVP.second);
			REQUIRE(stdMap[kvp.key] == kvp.value);
		}

		REQUIRE(it == map.cend());
#ifdef _WIN32
		REQUIRE(stdIt == stdMap.cend());
#endif
		
		// Now make sure range-based for-loops work.
		it = map.cbegin();
		for (const auto& pair : map)
		{
			REQUIRE(pair == *it++);
		}

		// Make certain we compare some iterators in the middle of the container instead of just at the end.
		it = map.cbegin();
		jt = map.cbegin();
		REQUIRE(++it == ++jt);

		REQUIRE(map.cbegin()->key == map.cbegin()->key);
	}
#pragma endregion
	
#pragma region properties
	TEST(IsEmpty)
	{
		KEY_VALUE;
		CONTAINER map;
		REQUIRE(map.IsEmpty());
	}
#pragma endregion
		
#pragma region query
	TEST(Contains)
	{
		KEY_VALUE;
		KVP pair = { Random::Next<TKey>(), Random::Next<TValue>() };
		CONTAINER map;
		REQUIRE(!map.Contains(pair.key));
		map = { pair };
		REQUIRE(map.Contains(pair.key));
	}

	TEST(Exists)
	{
		KEY_VALUE;
		const auto c = RandomHashMap<TKey, TValue>();
		const auto t = Random::Element(c);
		REQUIRE(Util::Exists(c, [&t](const auto& a) { return a == t; }));
	}

	TEST(Find)
	{
		KEY_VALUE;
		CONTAINER map;

		REQUIRE(map.end() == map.Find(Random::Next<TKey>()));

		map = RandomHashMap<TKey, TValue>();

		auto pair = Random::Element(map);
		auto it = map.Find(pair.key);

		REQUIRE(pair == *it);

		const auto cmap = RandomHashMap<TKey, TValue>();

		auto cpair = Random::Element(cmap);
		auto cit = cmap.Find(cpair.key);

		REQUIRE(cpair == *cit);
	}
#pragma endregion

#pragma region Insert
	TEST(Insert)
	{
		KEY_VALUE;
		CONTAINER map;
		KVP pair = { Random::Next<TKey>(), Random::Next<TValue>() };
		map.Insert(pair.key, pair.value);
		REQUIRE(map.Contains(pair.key));
	}
	
	TEST(InsertPair)
	{
		KEY_VALUE;
		CONTAINER map;
		KVP pair = { Random::Next<TKey>(), Random::Next<TValue>() };
		map.Insert(pair);
		REQUIRE(map.Contains(pair.key));
	}

	TEST(Emplace)
	{
		KEY_VALUE;
		CONTAINER map;
		KVP pair = { Random::Next<TKey>(), Random::Next<TValue>() };
		map.Emplace(pair);
		REQUIRE(map.Contains(pair.key));
		
		const auto oldValue = pair.value;
		pair.value = Random::NotEqualTo(oldValue);
		map.Emplace(pair);
		REQUIRE(oldValue != map.At(pair.key));
	}

	TEST(TryEmplace)
	{
		KEY_VALUE;
		CONTAINER map;
		KVP pair = { Random::Next<TKey>(), Random::Next<TValue>() };
		map.TryEmplace(pair);
		REQUIRE(map.Contains(pair.key));

		const auto oldValue = pair.value;
		pair.value = Random::NotEqualTo(oldValue);
		map.TryEmplace(pair);
		REQUIRE(oldValue == map.At(pair.key));
	}
#pragma endregion

#pragma region Remove
	TEST(RemoveMidIterate)
	{
		KEY_VALUE;
		// Test inspired from example code on https://en.cppreference.com/w/cpp/container/unordered_map/erase
		using Container = HashMap<int, std::string>;
		Container c
		{
			{1, "one"},
			{2, "two"},
			{3, "three"},
			{4, "four"},
			{5, "five"},
			{6, "six"}
		};

		// erase all odd numbers from c
		for (auto it = c.begin(); it != c.end(); )
		{
			if (it->key % 2)
			{
				it = c.Remove(it);
			}
			else
			{
				++it;
			}
		}

		REQUIRE(Container
			{
				{2, "two"},
				{4, "four"},
				{6, "six"}
			} == c);
	}
	
	TEST(Remove)
	{
		KEY_VALUE;
		CONTAINER map;
		KVP pair = { Random::Next<TKey>(), Random::Next<TValue>() };
		REQUIRE(!map.Remove(pair.key));
		map.Insert(pair);
		REQUIRE(map.Contains(pair.key));
		REQUIRE(map.Remove(pair.key));
		REQUIRE(!map.Contains(pair.key));
	}

	TEST(RemoveIterator)
	{
		KEY_VALUE;
		auto c = RandomHashMap<TKey, TValue>();
		auto [key, value] = Random::Element(c);
		typename CONTAINER::const_iterator it = c.Find(key);
		c.Remove(it);
		REQUIRE(!c.Contains(key));
	}

	TEST(RemoveRange)
	{
		//KEY_VALUE;
		//auto c = RandomHashMap<TKey, TValue>();
		//c.Remove(c.begin(), c.end());
		//REQUIRE(c.IsEmpty());

		//c = RandomHashMap<TKey, TValue>();
		//c.Remove(++c.begin(), c.end());
		//REQUIRE(c.Size() == 1);
	}
#pragma endregion

#pragma region Accessors
	TEST(At)
	{
		KEY_VALUE;
		CONTAINER map;

		TKey key = Random::Next<TKey>();
		TValue value;
		REQUIRE_THROWS_AS([&] { value = map.At(key); }(), std::out_of_range);

		value = Random::Next<TValue>();
		map[key] = value;
		REQUIRE(value == map.At(key));

		KVP pair = { Random::Next<TKey>(), Random::Next<TValue>() };
		const CONTAINER cmap = { pair };
	}
#pragma endregion

#pragma region Memory
	TEST(Resize)
	{
		KEY_VALUE;
		KVP pair = { Random::Next<TKey>(), Random::Next<TValue>() };
		CONTAINER map(10);
		REQUIRE(10_z == map.BucketCount());

		map.Resize(1);
		REQUIRE(1 == map.BucketCount());

		map = { pair };
		REQUIRE_THROWS_AS(map.Resize(0), std::invalid_argument);

		map.Resize(10);
		REQUIRE(10_z == map.BucketCount());
	}

	TEST(PrimeReserveStrategy)
	{
		KEY_VALUE;
		CONTAINER c;
		REQUIRE(1_z == c.BucketCount());
		
		c.Insert({ RandomKeyNotIn(c), Random::Next<TValue>() });
		c.Insert({ RandomKeyNotIn(c), Random::Next<TValue>() });
		REQUIRE(3_z == c.BucketCount());
		
		c.Insert({ RandomKeyNotIn(c), Random::Next<TValue>() });
		c.Insert({ RandomKeyNotIn(c), Random::Next<TValue>() });
		REQUIRE(5_z == c.BucketCount());
		
		c.Insert({ RandomKeyNotIn(c), Random::Next<TValue>() });
		c.Insert({ RandomKeyNotIn(c), Random::Next<TValue>() });
		REQUIRE(7_z == c.BucketCount());
		
		c.Insert({ RandomKeyNotIn(c), Random::Next<TValue>() });
		c.Insert({ RandomKeyNotIn(c), Random::Next<TValue>() });
		c.Insert({ RandomKeyNotIn(c), Random::Next<TValue>() });
		c.Insert({ RandomKeyNotIn(c), Random::Next<TValue>() });
		REQUIRE(11_z == c.BucketCount());
	}

	TEST(Swap)
	{
		KEY_VALUE;
		auto a = RandomHashMap<TKey, TValue>();
		auto b = RandomHashMap<TKey, TValue>();
		const auto& aRef = Random::Element(a);
		const auto& bRef = Random::Element(b);
		a.Swap(b);
		REQUIRE(b.Contains(aRef.key));
		REQUIRE(a.Contains(bRef.key));
	}
	
	TEST(Merge)
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
		REQUIRE(b.IsEmpty());
		REQUIRE("0"s == a[0]);
		REQUIRE("1"s == a[1]);
		REQUIRE("2"s == a[2]);
		REQUIRE("3"s == a[3]);
		REQUIRE("4"s == a[4]);
		REQUIRE("5"s == a[5]);
		REQUIRE("6"s == a[6]);
		REQUIRE("7"s == a[7]);
		REQUIRE("8"s == a[8]);
		REQUIRE("9"s == a[9]);
		
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
		REQUIRE(5_z == c.Size());
	}

	TEST(Invert)
	{
		KEY_VALUE;
		const auto key = Random::Next<TKey>();
		const auto value = Random::Next<TValue>();
		CONTAINER c{ { key, value }	};
		const auto inverted = c.Invert();
		REQUIRE(key == inverted[value]);
	}
#pragma endregion

#pragma region Operators
	TEST(Subscript)
	{
		KEY_VALUE;
		CONTAINER map;

		TKey key = Random::Next<TKey>();
		TValue value = map[key];
		REQUIRE(value == TValue());

		value = Random::Next<TValue>();
		map[key] = value;
		REQUIRE(value == map[key]);

		const CONTAINER cmap = map;
		auto pair = Random::Element(map);
		REQUIRE(cmap[pair.key] == pair.value);

		const CONTAINER emptyCmap;
		REQUIRE_THROWS_AS(emptyCmap[key], std::out_of_range);
	}

	TEST(Equivalence)
	{
		KEY_VALUE;
		CONTAINER a;

		REQUIRE(a == a);
		REQUIRE(!(a != a));

		CONTAINER b = a;
		const KVP kvp = { Random::Next<TKey>(), Random::Next<TValue>() };
		a.Insert(kvp);

		REQUIRE(!(a == b));
		REQUIRE(a != b);

		b.Insert({ Random::NotEqualTo(kvp.key), Random::Next<TValue>() });

		REQUIRE(!(a == b));
		REQUIRE(a != b);
	}
#pragma endregion

#pragma region string
	TEST(ToString)
	{
		using Container = HashMap<std::string, int>;
		using KVP = typename Container::value_type;

		const KVP kvp{ "hello", 1 };
		std::stringstream stream;
		stream << kvp;
		REQUIRE(stream.str() == "{ hello, 1 }"s);
	}
#pragma endregion
}
