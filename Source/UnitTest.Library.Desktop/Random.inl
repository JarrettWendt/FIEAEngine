#pragma once

#include <algorithm>

#include "Random.h"
#include "Datum.h"
#include "Array.h"
#include "Util.h"

namespace UnitTests
{
	template<typename T>
	size_t EnsureValidCount(size_t count)
	{
		if constexpr (std::is_integral_v<T>)
		{
			if constexpr (std::_Is_character_or_byte<T>::value)
			{
				count = std::min(26_z, count);
			}
			else
			{
				count = std::min(size_t(std::numeric_limits<T>::max()), count);
			}
			count = std::max(1_z, count);
		}
		return count;
	}
	
	template<typename T, typename... Ts>
	template<typename Container>
	inline T Random<T, Ts...>::NotIn(const Container& container)
	{
		T ret;
		do
		{
			ret = Library::Random::Next<T>();
		} while (Library::Util::Contains(container, ret));
		return ret;
	}
	
	template<typename T, typename... Ts>
	inline T Random<T, Ts...>::NotEqualTo(const T& t)
	{
		std::remove_const_t<T> ret;
		do
		{
			ret = Random<T>::Next();
		} while (t == ret);
		return ret;
	}

	template <typename T, typename... Ts>
	inline T Random<T, Ts...>::Next()
	{
		return Range(T(0), T(defaultSize));
	}

	template <typename T, typename... Ts>
	inline T Random<T, Ts...>::Range(const T min, T max)
	{
		if constexpr (!std::is_same_v<bool, T> && std::is_arithmetic_v<T>)
		{
			--max;
		}
		static std::default_random_engine rng;
		if constexpr (std::is_floating_point_v<T>)
		{
			const std::uniform_real_distribution<T> dist(min, max);
			return dist(rng);
		}
		else
		{
			if constexpr (std::is_same_v<char, T>)
			{
				const std::uniform_int_distribution<short> dist(min, max);
				return char(dist(rng));
			}
			else
			{
				const std::uniform_int_distribution<T> dist(min, max);
				return dist(rng);
			}
		}
	}

#pragma region string
	template <>
	inline char Random<char>::Next()
	{
		return Range('a', 'z' + 1);
	}

	template<>
	inline std::string Random<std::string>::Next()
	{
		static std::default_random_engine rng;
		static std::uniform_int_distribution<size_t> dist(0, 10);
		const size_t count = dist(rng);
		std::stringstream stream;
		for (size_t i = 0; i < count; i++)
		{
			stream << Random<char>::Next();
		}
		return stream.str();
	}

	template<>
	inline char* Random<char*>::Next()
	{
		static std::default_random_engine rng;
		static std::uniform_int_distribution<int> dist(0, 10);
		const int count = dist(rng);
		char* ret = new char[count]();
		for (int i = 0; i < count - 1; i++)
		{
			ret[i] = Random<char>::Next();
		}
		return ret;
	}

	template<>
	inline const char* Random<const char*>::Next()
	{
		return Random<char*>::Next();
	}

	template<>
	inline char* const Random<char* const>::Next()
	{
		return Random<char*>::Next();
	}

	template<>
	inline const char* const Random<const char* const>::Next()
	{
		return Random<char*>::Next();
	}
#pragma endregion

#pragma region Random Specializations
	template<typename T, typename ReserveStrategy>
	struct Random<Library::Array<T, ReserveStrategy>>
	{
		using Container = Library::Array<T, ReserveStrategy>;
		static inline size_t defaultSize = 10;
		
		static Container Next(size_t count = defaultSize)
		{
			count = EnsureValidCount<T>(count);
			Container ret(count);
			for (size_t i = 0; i < count; i++)
			{
				if constexpr (std::is_same_v<T, SList<int>>)
				{
					ret.PushBack(Random<T>::Next(5));
				}
				else
				{
					ret.PushBack(Random<T>::Next());
				}
			}
			return ret;
		}
		
		static Container AllUnique(size_t count = defaultSize)
		{
			count = EnsureValidCount<T>(count);
			Container ret(count);
			for (size_t i = 0; i < count; i++)
			{
				ret.PushBack(Random<T>::NotIn(ret));
			}
			return ret;
		}

		template<typename OtherContainer>
		static inline Container NotIn(const OtherContainer& container)
		{
			Container ret;
			do
			{
				ret = Random<Container>::Next();
			} while (Library::Util::Contains(container, ret));
			return ret;
		}

		static inline Container NotEqualTo(const Container& t)
		{
			Container ret;
			do
			{
				ret = Random<Container>::Next();
			} while (t == ret);
			return ret;
		}
	};

	template<>
	struct Random<bool>
	{
		static inline size_t defaultSize = 10;

		static bool Next()
		{
			std::default_random_engine rng;
			const std::uniform_int_distribution<short> dist(0, 1);
			return dist(rng);
		}

		template<typename Container>
		static inline bool NotIn(const Container& container)
		{
			bool ret;
			do
			{
				ret = Random<bool>::Next();
			} while (Library::Util::Contains(container, ret));
			return ret;
		}
	};
	
	template<typename T>
	struct Random<Library::SList<T>>
	{
		using Container = Library::SList<T>;
		static inline size_t defaultSize = 10;

		static Container Next(size_t count = defaultSize)
		{
			count = EnsureValidCount<T>(count);
			Container ret;
			for (size_t i = 0; i < count; i++)
			{
				ret.PushBack(Random<T>::Next());
			}
			return ret;
		}

		static Container AllUnique(size_t count = defaultSize)
		{
			count = EnsureValidCount<T>(count);
			Container ret;
			for (size_t i = 0; i < count; i++)
			{
				ret.PushBack(Random<T>::NotIn(ret));
			}
			return ret;
		}

		template<typename OtherContainer>
		static inline Container NotIn(const OtherContainer& container)
		{
			Container ret;
			do
			{
				ret = Random<Container>::Next();
			} while (Library::Util::Contains(container, ret));
			return ret;
		}

		static inline Container NotEqualTo(const Container& t)
		{
			Container ret;
			do
			{
				ret = Random<Container>::Next();
			} while (t == ret);
			return ret;
		}
	};
	
	template<typename TKey, typename TValue, typename Hash, typename KeyEqual, typename ReserveStrategy>
	struct Random<Library::HashMap<TKey, TValue, Hash, KeyEqual, ReserveStrategy>>
	{
		using Container = Library::HashMap<TKey, TValue, Hash, KeyEqual, ReserveStrategy>;
		using KVP = typename Container::value_type;
		static inline size_t defaultSize = 10;

		static Container Next(size_t count = defaultSize)
		{
			count = EnsureValidCount<TKey>(count);
			Container ret(count);
			for (size_t i = 0; i < count; i++)
			{
				ret.Insert(Random<TKey>::Next(), Random<TValue>::Next());
			}
			return ret;
		}

		static Container AllUnique(size_t count = defaultSize)
		{
			count = EnsureValidCount<T>(count);
			Container ret;
			for (size_t i = 0; i < count; i++)
			{
				ret.Insert(Random<TKey>::NotIn(ret), Random<TValue>::Next());
			}
			return ret;
		}

		template<typename OtherContainer>
		static inline Container NotIn(const OtherContainer& container)
		{
			Container ret;
			do
			{
				ret = Next();
			} while (container.Contains(ret));
			return ret;
		}

		static inline Container NotEqualTo(const Container& t)
		{
			Container ret;
			do
			{
				ret = Next();
			} while (t == ret);
			return ret;
		}
	};
#pragma endregion
	
	template<typename LibContainer>
	LibContainer RandomUnique(const size_t count)
	{
		LibContainer ret;
		// TODO: Use SFINAE to determine if LibContainer::Reserve exists.
		//ret.Reserve(count);
		for (size_t i = 0; i < count; i++)
		{
			ret.PushBack(Random<typename LibContainer::value_type>::NotIn(ret));
		}
		return ret;
	}

	template<typename LibContainer, typename StlContainer>
	void FillRandomlyUnique(LibContainer& libC, StlContainer& stlC, size_t count)
	{
		using T = typename StlContainer::value_type;
		count = EnsureValidCount<T>(count);
		// TODO: Use SFINAE to determine if LibContainer::Reserve and StlContainer::reserve exist.
		//libC.Reserve(count);
		//stlC.reserve(count);
		for (size_t i = 0; i < count; i++)
		{
			const T t = Random<T>::NotIn(libC);
			libC.PushBack(t);
			stlC.push_back(t);
		}
	}

	template <typename StlContainer>
	void FillRandomlyUnique(Library::Datum& d, StlContainer& stlC, [[maybe_unused]] size_t count)
	{
		using T = typename StlContainer::value_type;
		
		if constexpr (std::is_same_v<T, bool>)
		{
			d = { true, false };
			stlC = { true, false };
		}
		else
		{
			count = EnsureValidCount<T>(count);
			//// TODO: Use SFINAE to determine if LibContainer::Reserve and StlContainer::reserve exist.
			////libC.Reserve(count);
			////stlC.reserve(count);
			for (size_t i = 0; i < count; i++)
			{
				const T t = Random<T>::NotIn(d);
				d.PushBack<T>(t);
				stlC.push_back(t);
			}
		}
	}

	template<typename LibContainer, typename StlContainer>
	void FillRandomly(LibContainer& libC, StlContainer& stlC, size_t count)
	{
		using T = typename StlContainer::value_type;
		count = EnsureValidCount<T>(count);
		// TODO: Use SFINAE to determine if LibContainer::Reserve and StlContainer::reserve exist.
		//libC.Reserve(count);
		//stlC.reserve(count);
		for (size_t i = 0; i < count; i++)
		{
			const T t = Library::Random::Next<T>();
			libC.PushBack(t);
			stlC.push_back(t);
		}
	}

	template <typename T>
	void FillRandomly(Library::Array<T>& a, Library::Array<T>& v, size_t count)
	{
		count = EnsureValidCount<T>(count);
		a.Reserve(count);
		v.Reserve(count);
		for (size_t i = 0; i < a.Capacity(); i++)
		{
			const T t = Random<T>::Next();
			a.PushBack(t);
			v.PushBack(t);
		}
	}
	
	template<typename T>
	void FillRandomly(Library::SList<T>& a, Library::SList<T>& v, size_t count)
	{
		count = EnsureValidCount<T>(count);
		for (size_t i = 0; i < a.Capacity(); i++)
		{
			const T t = Random<T>::Next();
			a.PushBack(t);
			v.PushBack(t);
		}
	}
	
	template<typename TKey, typename TValue>
	void FillRandomly(Library::HashMap<TKey, TValue>& a, std::unordered_map<TKey, TValue>& v, size_t count)
	{
		count = EnsureValidCount<TKey>(count);
		for (size_t i = 0; i < count; i++)
		{
			const auto key = Random<TKey>::Next();
			const auto value = Random<TValue>::Next();
			a.Insert({ key, value });
			v.insert({ key, value });
		}
	}

	template <typename T>
	Library::Datum RandomDatum()
	{
		Library::Array a = Library::Random::Next<Library::Array<T>>();
		Library::Datum ret(a.begin(), a.end());
		return ret;
	}
	
	template<typename T>
	Library::Datum RandomDatumUnique()
	{
		if constexpr (std::is_same_v<bool, T>)
		{
			return { true, false };
		}
		else
		{
			Library::Array a = RandomUnique<Library::Array<T>>();
			Library::Datum ret(a.begin(), a.end());
			return ret;
		}
	}
}
