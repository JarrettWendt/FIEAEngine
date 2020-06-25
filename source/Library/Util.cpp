// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Util.h"
#include "LibMath.h"

using namespace std::string_literals;

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
		str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
		return str;
	}
	
	std::string ReplaceAll(std::string str, const std::string& from, const std::string& to)
	{
	    size_t start_pos = 0;
	    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	    {
	        str.replace(start_pos, from.length(), to);
	        start_pos += to.length();
	    }
	    return str;
	}

	std::string FixDirectorySeparators(std::string filepath) noexcept
	{
		const std::string delim = std::to_string(char(std::filesystem::path::preferred_separator));
		const std::string notdelim = delim == "/" ? "\\" : "/";
		
		filepath = ReplaceAll(filepath, "\\\\", delim);
		filepath = ReplaceAll(filepath, "//", delim);
		filepath = ReplaceAll(filepath, notdelim, delim);
		
		return filepath;
	}
	
	std::string WindowsToWSLDir(std::string absfilepath) noexcept
	{
		if (absfilepath[0] != '/')
		{
			const char driveLetter = absfilepath[0];
			absfilepath.replace(0, 2, "/mnt/"s + char(std::tolower(driveLetter)));
		}		
		return absfilepath;
	}
#pragma endregion
}