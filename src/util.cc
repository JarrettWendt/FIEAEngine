/**
 * Copyright (c) 2019-2022 Jarrett Wendt
 */

#include <string>

#include "util.hh"

namespace fiea
{
	bool IsEmptyOrWhitespace(const std::string& str) noexcept
	{
		return str.empty() || IsWhitespace(str);
	}

	bool IsWhitespace(const std::string& str) noexcept
	{
		for (const char c : str)
		{
			if (!std::isspace(c))
			{
				return false;
			}
		}
		return !str.empty();
	}
}
