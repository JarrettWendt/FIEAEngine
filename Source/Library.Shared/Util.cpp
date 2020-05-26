#include "pch.h"
#include "Util.h"
#include "LibMath.h"

namespace Library::Util
{
	size_t DefaultReserveStrategy::operator()(const size_t size, const size_t capacity) noexcept
	{
		return size_t((size < capacity / 2) ? capacity / 1.5 : size * 1.5);
	}

	size_t PrimeReserveStrategy::operator()(const size_t size, const size_t) noexcept
	{
		return Math::NextPrime(size);
	}

#pragma region String
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
		return str.size() > 0;
	}

	bool HasAlpha(const std::string& str) noexcept
	{
		for (const char c : str)
		{
			if (std::isalpha(c))
			{
				return true;
			}
		}
		return false;
	}

	std::string ToLower(std::string str) noexcept
	{
		for (char& c : str)
		{
			c = static_cast<char>(std::tolower(c));
		}
		return str;
	}

	std::string ToUpper(std::string str) noexcept
	{
		for (char& c : str)
		{
			c = static_cast<char>(std::toupper(c));
		}
		return str;
	}

	bool IsUpper(const std::string& str) noexcept
	{
		for (const char c : str)
		{
			if (std::isalpha(c) && !std::isupper(c))
			{
				return false;
			}
		}
		return HasAlpha(str);
	}

	bool IsLower(const std::string& str) noexcept
	{
		for (const char c : str)
		{
			if (std::isalpha(c) && !std::islower(c))
			{
				return false;
			}
		}
		return HasAlpha(str);
	}

	std::string RemoveWhitespace(std::string str) noexcept
	{
		str.erase(std::remove_if(str.begin(), str.end(), std::isspace), str.end());
		return str;
	}
#pragma endregion
}