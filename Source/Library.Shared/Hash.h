// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

namespace Library
{
	using hash_t = size_t;

	namespace HashUtils
	{
		/** the prime used in the AdditiveHash */
		const size_t prime = 31;

		/**
		 * A simple additive hash that iterates through the bytes, multiplies them by a prime, and sums them.
		 *
		 * @param bytes		the bytes of data to hash
		 * @param count		how many bytes there are
		 */
		hash_t AdditiveHash(const uint8_t* bytes, const size_t count) noexcept;
	}

	/**
	 * Default Template Declaration of the Hash Functor.
	 * Casts data types not larger than hash_t (such as most primitives) to a hash_t.
	 * Sums the bytes of data types larger than a hash_t.
	 */
	template<typename T, typename... Ts>
	struct Hash
	{
		hash_t operator()(const T& t) const;
	};

	/**
	 * Pointer specialization.
	 * Returns the address casted to a hash_t.
	 */
	template<typename T>
	struct Hash<T*>
	{
		hash_t operator()(const T* t) const;
	};
}

namespace Library::Concept
{
	template<typename Hash, typename T>
	concept Hasher = requires(Hash hash, T t)
	{
		{ hash(t) }->std::convertible_to<hash_t>;
	};
}

#include "Hash.inl"
