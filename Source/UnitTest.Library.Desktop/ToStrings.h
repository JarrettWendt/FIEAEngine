// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace UnitTests;

#define SPECIALIZE_TO_STRING_REF(Q)															\
template<>																					\
inline std::wstring ToString<Q>(const Q& q)													\
{																							\
	return CppUnitTestFramework::ToString(Stringify<Q>::std(q));							\
}

#define SPECIALIZE_TO_STRING_PTR(Q)															\
template<>																					\
inline std::wstring ToString<Q>(Q* q)														\
{																							\
	return q ? CppUnitTestFramework::ToString(Stringify<Q*>::std(q)) : L"nullptr";			\
}

#define SPECIALIZE_TO_STRING_CONST_PTR(Q)													\
template<>																					\
inline std::wstring ToString<Q>(const Q* q)													\
{																							\
	return q ? CppUnitTestFramework::ToString(Stringify<Q*>::std(q)) : L"const nullptr";	\
}

#define SPECIALIZE_TO_STRING_SHARED_PTR(Q)													\
template<>																					\
inline std::wstring ToString<std::shared_ptr<Q>>(const std::shared_ptr<Q>& q)				\
{																							\
	return CppUnitTestFramework::ToString(Stringify<std::shared_ptr<Q>>::std(q));			\
}

#define SPECIALIZE_TO_STRING(Q)																\
SPECIALIZE_TO_STRING_REF(SINGLE_ARG(Q))														\
SPECIALIZE_TO_STRING_PTR(SINGLE_ARG(Q))														\
SPECIALIZE_TO_STRING_CONST_PTR(SINGLE_ARG(Q))												\
SPECIALIZE_TO_STRING_SHARED_PTR(SINGLE_ARG(Q))												\
SPECIALIZE_TO_STRING_SHARED_PTR(SINGLE_ARG(const Q))

#define SPECIALIZE_TO_STRING_IT(Q)															\
template<>																					\
inline std::wstring ToString<Q>(const Q& q)													\
{																							\
	return L"iterator at " + CppUnitTestFramework::ToString(*q);							\
}																							\
template<>																					\
inline std::wstring ToString<Q>(Q*)															\
{																							\
	return L"iterator pointer";																\
}																							\
template<>																					\
inline std::wstring ToString<Q>(const Q*)													\
{																							\
	return L"const iterator pointer";														\
}

#define SPECIALIZE_CONTAINER_(Container)													\
	SPECIALIZE_TO_STRING(SINGLE_ARG(Container))												\
	SPECIALIZE_TO_STRING_IT(SINGLE_ARG(Container::iterator))								\
	SPECIALIZE_TO_STRING_IT(SINGLE_ARG(Container::const_iterator))

#define SPECIALIZE_CONTAINER(Container, Q)													\
	SPECIALIZE_CONTAINER_(SINGLE_ARG(Container<Q>))

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<typename T>
	struct Stringify
	{		
		inline static std::string std(const T& t)
		{
			return std::to_string(t);
		}
	};

	template<typename T>
	struct Stringify<T*>
	{
		inline static std::string std(const T* t)
		{
			return Stringify<T>::std(*t);
		}
	};

	template<typename T>
	struct Stringify<std::shared_ptr<T>>
	{
		inline static std::string std(const std::shared_ptr<T> t)
		{
			return t == nullptr ? "nullptr shared_ptr" : Stringify<T>::std(*t);
		}
	};

	template<>
	struct Stringify<Attributed>
	{
		inline static std::string std(const Attributed&)
		{
			return "Attributed";
		}
	};

	template<>
	struct Stringify<Entity>
	{
		inline static std::string std(const Entity&)
		{
			return "Entity";
		}
	};

	//SPECIALIZE_TO_STRING(Vector2)
	//SPECIALIZE_TO_STRING(Vector3)
	//SPECIALIZE_TO_STRING(Vector4)
	//SPECIALIZE_TO_STRING(Quaternion)
	//SPECIALIZE_TO_STRING(Matrix)
	//SPECIALIZE_TO_STRING(Transform)

	SPECIALIZE_TO_STRING(Input::KeyCode)
	SPECIALIZE_TO_STRING(Input::KeyState)
	SPECIALIZE_TO_STRING(Datum::Type)
	SPECIALIZE_TO_STRING(Digit)
	SPECIALIZE_TO_STRING(SignedDigit)
	SPECIALIZE_TO_STRING(Foo)
	SPECIALIZE_TO_STRING(Attributed)
	SPECIALIZE_CONTAINER_(Entity)

	SPECIALIZE_TO_STRING_PTR(std::string)
	SPECIALIZE_TO_STRING_CONST_PTR(std::string)

	SPECIALIZE_CONTAINER(Array, bool)
	SPECIALIZE_CONTAINER(Array, char)
	SPECIALIZE_CONTAINER(Array, int)
	SPECIALIZE_CONTAINER(Array, float)
	SPECIALIZE_CONTAINER(Array, double)
	SPECIALIZE_CONTAINER(Array, uint64_t)
	SPECIALIZE_CONTAINER(Array, std::string)
	SPECIALIZE_CONTAINER(Array, Array<int>)
	SPECIALIZE_CONTAINER(Array, Array<std::string>)
	SPECIALIZE_CONTAINER(Array, std::shared_ptr<Foo>)

	SPECIALIZE_CONTAINER(SList, bool)
	SPECIALIZE_CONTAINER(SList, char)
	SPECIALIZE_CONTAINER(SList, int)
	SPECIALIZE_CONTAINER(SList, float)
	SPECIALIZE_CONTAINER(SList, double)
	SPECIALIZE_CONTAINER(SList, uint64_t)
	SPECIALIZE_CONTAINER(SList, std::string)
	SPECIALIZE_CONTAINER(SList, Array<int>)
	SPECIALIZE_CONTAINER(SList, Array<std::string>)
	SPECIALIZE_CONTAINER(SList, SList<int>)
	SPECIALIZE_CONTAINER(SList, SList<std::string>)
	SPECIALIZE_CONTAINER(SList, std::shared_ptr<Foo>)

	// These can't be with the other Array specializations because they require SList
	SPECIALIZE_CONTAINER(Array, SList<int>)
	SPECIALIZE_CONTAINER(Array, SList<std::string>)

	// KeyValuePair specialization must come first because HashMap invokes it.
	SPECIALIZE_TO_STRING(SINGLE_ARG(typename HashMap<int, std::string>::value_type))
	SPECIALIZE_CONTAINER(HashMap, SINGLE_ARG(int, std::string))
	SPECIALIZE_CONTAINER(HashMap, SINGLE_ARG(int, std::string, Hash<int>, std::equal_to<int>, Util::DefaultReserveStrategy))

	SPECIALIZE_TO_STRING(SINGLE_ARG(typename HashMap<std::string, Datum>::value_type))
	SPECIALIZE_CONTAINER(HashMap, SINGLE_ARG(std::string, Datum))
	SPECIALIZE_CONTAINER(HashMap, SINGLE_ARG(std::string, Datum, Hash<std::string>, std::equal_to<std::string>, Util::DefaultReserveStrategy))

	using BoolRefWrapper = std::_Vb_reference<std::_Wrap_alloc<std::allocator<unsigned int>>>;
	template<>
	inline std::wstring ToString<BoolRefWrapper>(const BoolRefWrapper&)
	{
		return L"BoolRefWrapper";
	}
	
	SPECIALIZE_CONTAINER(std::vector, bool)
	SPECIALIZE_CONTAINER(std::vector, char)
	SPECIALIZE_CONTAINER(std::vector, int)
	SPECIALIZE_CONTAINER(std::vector, float)
	SPECIALIZE_CONTAINER(std::vector, double)
	SPECIALIZE_CONTAINER(std::vector, uint64_t)
	SPECIALIZE_CONTAINER(std::vector, std::string)
	SPECIALIZE_CONTAINER(std::vector, Array<int>)
	SPECIALIZE_CONTAINER(std::vector, Array<std::string>)
	SPECIALIZE_CONTAINER(std::vector, SList<int>)
	SPECIALIZE_CONTAINER(std::vector, SList<std::string>)
	SPECIALIZE_CONTAINER(std::vector, std::shared_ptr<Foo>)

	SPECIALIZE_CONTAINER(std::forward_list, bool)
	SPECIALIZE_CONTAINER(std::forward_list, char)
	SPECIALIZE_CONTAINER(std::forward_list, int)
	SPECIALIZE_CONTAINER(std::forward_list, float)
	SPECIALIZE_CONTAINER(std::forward_list, double)
	SPECIALIZE_CONTAINER(std::forward_list, uint64_t)
	SPECIALIZE_CONTAINER(std::forward_list, std::string)
	SPECIALIZE_CONTAINER(std::forward_list, Array<int>)
	SPECIALIZE_CONTAINER(std::forward_list, Array<std::string>)
	SPECIALIZE_CONTAINER(std::forward_list, SList<int>)
	SPECIALIZE_CONTAINER(std::forward_list, SList<std::string>)
	SPECIALIZE_CONTAINER(std::forward_list, std::shared_ptr<Foo>)

	SPECIALIZE_CONTAINER(std::list, bool)
	SPECIALIZE_CONTAINER(std::list, char)
	SPECIALIZE_CONTAINER(std::list, int)
	SPECIALIZE_CONTAINER(std::list, float)
	SPECIALIZE_CONTAINER(std::list, double)
	SPECIALIZE_CONTAINER(std::list, uint64_t)
	SPECIALIZE_CONTAINER(std::list, std::string)
	SPECIALIZE_CONTAINER(std::list, Array<int>)
	SPECIALIZE_CONTAINER(std::list, Array<std::string>)
	SPECIALIZE_CONTAINER(std::list, SList<int>)
	SPECIALIZE_CONTAINER(std::list, SList<std::string>)
	SPECIALIZE_CONTAINER(std::list, std::shared_ptr<Foo>)

	SPECIALIZE_TO_STRING(SINGLE_ARG(typename std::unordered_map<int, std::string>::value_type))
	SPECIALIZE_CONTAINER(std::unordered_map, SINGLE_ARG(int, std::string))

	template<>																	
	inline std::wstring ToString<Datum::value_type>(const Datum::value_type& q)
	{
		std::stringstream stream;
		using namespace Library;
		stream << q;
		return ToString(stream.str());
	}
	
	SPECIALIZE_CONTAINER_(Datum)
}
