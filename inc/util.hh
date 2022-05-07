/**
 * Copyright (c) 2019-2022 Jarrett Wendt
 */

#include <cstddef>

namespace fiea
{
	/**
	 * literal operator for size_t
	 *
	 * @param x literal to convert to a size_t
	 *
	 * @returns x as a size_t
	 */
	constexpr size_t operator""_z(unsigned long long int x)
	{
		return size_t(x);
	}

	/**
	 * literal operator for ptrdiff_t
	 *
	 * @param x literal to convert to a ptrdiff_t
	 *
	 * @returns x as a ptrdiff_t
	 */
	constexpr ptrdiff_t operator""_ptrdiff(unsigned long long int x)
	{
		return ptrdiff_t(x);
	}

	/**
	 * @brief determines if the given string is empty or whitespace
	 *
	 * @param str string to test
	 *
	 * @returns if the given string is empty or whitespace
	 */
	bool IsEmptyOrWhitespace(const std::string& str) noexcept;

	/**
	 * @brief determines if the given string is whitespace
	 *
	 * @param str string to test
	 *
	 * @returns if the given string is whitespace
	 */
	bool IsWhitespace(const std::string& str) noexcept;
}
