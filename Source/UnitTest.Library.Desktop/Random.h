// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "Macros.h"
#include "../Library.Shared/Random.h"

namespace UnitTests
{
	template<typename T, typename... Ts>
	struct Random
	{
		SPECIAL_MEMBERS(Random, delete)
		
		static inline constexpr size_t defaultSize = 10;
		
		static T Next();

		static T Range(T min, T max);

		template<typename Container>
		static T NotIn(const Container& container);

		static T NotEqualTo(const T& t);
	};
	
	template<typename T>
	struct Random<std::shared_ptr<T>>
	{
		using SharedT = std::shared_ptr<T>;
		
		SPECIAL_MEMBERS(Random, delete)

		static inline constexpr size_t defaultSize = 10;

		static SharedT Next()
		{
			return std::make_shared<T>(Random<T>::Next());
		}

		static SharedT Range(SharedT min, SharedT max)
		{
			return std::make_shared<T>(Random<T>::Range(*min, *max));
		}

		template<typename Container>
		static SharedT NotIn(const Container& container)
		{
			SharedT ret;
			do
			{
				ret = std::make_shared<T>(Library::Random::Next<T>());
			} while (Library::Util::Contains(container, ret));
			return ret;
		}

		static SharedT NotEqualTo(const SharedT& t)
		{
			return std::make_shared<T>(Random<T>::NotEqualTo(*t));
		}
	};

	template<typename LibContainer>
	static LibContainer RandomUnique(size_t count = 5);

	template<typename LibContainer, typename StlContainer>
	static void FillRandomlyUnique(LibContainer& libC, StlContainer& stlC, size_t count = 10);

	template<typename StlContainer>
	static void FillRandomlyUnique(Library::Datum& d, StlContainer& stlC, size_t count = 10);

	template<typename LibContainer, typename StlContainer>
	static void FillRandomly(LibContainer& libC, StlContainer& stlC, size_t count = 10);

	template<typename T>
	static void FillRandomly(Library::Array<T>& a, Library::Array<T>& v, size_t count = 10);

	template<typename T>
	static void FillRandomly(Library::SList<T>& a, Library::SList<T>& v, size_t count = 10);

	template<typename TKey, typename TValue>
	static void FillRandomly(Library::HashMap<TKey, TValue>& a, std::unordered_map<TKey, TValue>& v, size_t count = 10);

	template<typename T>
	static Library::Datum RandomDatum();

	template<typename T>
	static Library::Datum RandomDatumUnique();
}

namespace Library::Random
{	
	template<typename T, typename Engine = DefaultEngine>
	std::enable_if_t<std::is_class_v<T>, T>
	static Next(const typename T::size_type size = 10)
	{
		// Instead of inserting into the container directly since we don't know what methods it has
		// We insert into a vector and since most containers can be constructed from iterators we do that.
		std::vector<typename T::value_type> v;
		v.reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			v.push_back(Next<typename T::value_type, Engine>());
		}
		return T(v.begin(), v.end());
	}

	template<typename T, typename Engine = DefaultEngine>
	std::enable_if_t<std::is_same_v<T, std::string>, T>
	inline Next()
	{
		return Range<T, Engine>(0, 10);
	}

	template<typename T, typename Engine = DefaultEngine>
	std::enable_if_t<std::is_same_v<T, std::string>, T>
	inline Next(const size_t maxSize)
	{
		return Range<T, Engine>(0, maxSize);
	}

	template<typename T, typename Engine = DefaultEngine>
	std::enable_if_t<std::is_same_v<T, std::string>, T>
	inline Range(const size_t min, size_t max)
	{
		static Engine rng;
		static const std::uniform_int_distribution<size_t> dist(min, --max);
		std::string ret;
		const auto size = dist(rng);
		ret.reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			ret.push_back(Next<char>());
		}
		return ret;
	}

	template<typename T, typename Engine = DefaultEngine>
	std::enable_if_t<std::is_same_v<T, UnitTests::Foo>, T>
		inline Next()
	{
		return UnitTests::Foo(Random::Next<int>());
	}

	template<typename T, typename Engine = DefaultEngine>
	std::enable_if_t<std::is_same_v<T, std::shared_ptr<UnitTests::Foo>>, T>
	inline Next()
	{
		return std::make_shared<UnitTests::Foo>(Next<UnitTests::Foo>());
	}
}

#include "Random.inl"
