// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "Hash.h"
#include "Util.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Transform.h"

namespace Library
{
	template<typename T, typename ...Ts>
	inline hash_t Hash<T, Ts...>::operator()(const T& t) const
	{
		if constexpr (sizeof(T) <= sizeof(hash_t))
		{
			// Knowing that T is the same size or smaller than a hash_t, it should be suitable to just cast it to a hash_t.
			// None of C or C++'s casts can be guaranteed to work here.
			return Util::UnionCast<hash_t>(t);
		}
		else
		{
			return HashUtils::AdditiveHash(reinterpret_cast<uint8_t*>(const_cast<T*>(&t)), sizeof(T*));
		}
	}

	template<typename T>
	inline hash_t Hash<T*>::operator()(const T* t) const
	{
		// Pointers and hash_t should be the same size, so casting it to a hash_t should be suitable.
		return reinterpret_cast<hash_t>(t);
	}
	
#pragma region Specializations
	// TODO: These explicit specializations should be able to be moved to a .cpp file but they for some reason don't link.

#pragma region string
	template<>
	struct Hash<std::string>
	{
		hash_t operator()(const std::string& str) const
		{
			// Just do an additive hash over all the chars.
			return HashUtils::AdditiveHash(reinterpret_cast<uint8_t*>(const_cast<char*>(str.c_str())), str.length());
		}
	};

	template<>
	struct Hash<char*>
	{
		hash_t operator()(const char* str) const
		{
			// Shouldn't call AdditiveHash because that requires a count.
			// Why call strlen when we can look for the null terminator ourselves.
			hash_t ret{};
			while (*str)
			{
				ret += HashUtils::prime * *str++;
			}
			return ret;
		}
	};

	template<>
	struct Hash<const char*>
	{
		hash_t operator()(const char* str) const
		{
			return Hash<char*>{}(const_cast<char*>(str));
		}
	};

	template<>
	struct Hash<char* const>
	{
		hash_t operator()(char* const str) const
		{
			return Hash<char*>{}(const_cast<char*>(str));
		}
	};

	template<>
	struct Hash<const char* const>
	{
		hash_t operator()(const char* const str) const
		{
			return Hash<char*>{}(const_cast<char*>(str));
		}
	};
#pragma endregion

	template<>
	struct Hash<Vector2>
	{
		hash_t operator()(const Vector2& m) const
		{
			return HashUtils::AdditiveHash(reinterpret_cast<uint8_t*>(const_cast<Vector2*>(&m)), sizeof(Vector2));
		}
	};

	template<>
	struct Hash<Vector3>
	{
		hash_t operator()(const Vector3& m) const
		{
			return HashUtils::AdditiveHash(reinterpret_cast<uint8_t*>(const_cast<Vector3*>(&m)), sizeof(Vector3));
		}
	};

	template<>
	struct Hash<Vector4>
	{
		hash_t operator()(const Vector4& m) const
		{
			return HashUtils::AdditiveHash(reinterpret_cast<uint8_t*>(const_cast<Vector4*>(&m)), sizeof(Vector4));
		}
	};

	template<>
	struct Hash<Quaternion>
	{
		hash_t operator()(const Quaternion& m) const
		{
			return HashUtils::AdditiveHash(reinterpret_cast<uint8_t*>(const_cast<Quaternion*>(&m)), sizeof(Quaternion));
		}
	};

	template<>
	struct Hash<Matrix>
	{
		hash_t operator()(const Matrix& m) const
		{
			return HashUtils::AdditiveHash(reinterpret_cast<uint8_t*>(const_cast<Matrix*>(&m)), sizeof(Matrix));
		}
	};

	template<>
	struct Hash<Library::Transform>
	{
		hash_t operator()(const Transform& t) const
		{
			return HashUtils::AdditiveHash(reinterpret_cast<uint8_t*>(const_cast<Transform*>(&t)), sizeof(Transform));
		}
	};
#pragma endregion
}
