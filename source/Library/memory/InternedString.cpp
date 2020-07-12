#include "InternedString.h"

using namespace std::string_literals;

namespace Library
{
	String::Intern::Intern(const std::string& string) noexcept :
		string(string),
		hash(std::hash<std::string>{}(string)) {}

	constexpr hash_t String::Hash::operator()(const SharedIntern& i) const
	{
		return i->hash;
	}

	bool String::KeyEqual::operator()(const SharedIntern& a, const SharedIntern& b) const
	{
		return a->string == b->string;
	}

#pragma region special members
	String::String() noexcept :
		String(""s) {}

	String::String(const char* string) noexcept :
		String(std::string(string)) {}

	String::String(const wchar_t* string) noexcept :
		String(std::wstring(string)) {}
	
	String::String(const std::string& string) noexcept :
		intern(*set.emplace(std::make_shared<Intern>(string)).first) {}

	String::String(const std::wstring& string) noexcept :
		String(std::string(string.begin(), string.end())) {}
	
	String& String::operator=(const String& other) noexcept
	{
		if (this != &other)
		{
			this->~String();
			intern = other.intern;
		}
		return *this;
	}

	String& String::operator=(String&& other) noexcept
	{
		if (this != &other)
		{
			this->~String();
			intern = std::move(other.intern);
		}
		return *this;
	}
	
	String::~String() noexcept
	{
		// If the only reference is ours and the one held by the set.
		if (intern.use_count() <= 2 && intern)
		{
			set.erase(intern);
		}
	}
#pragma endregion

#pragma region properties
	constexpr size_t String::Length() const noexcept
	{
		return intern->string.length();
	}
	
	constexpr bool String::IsEmpty() const noexcept
	{
		return intern->string.empty();
	}

	bool String::IsWhitespace() const noexcept
	{
		for (const char c : *this)
		{
			if (!std::isspace(c))
			{
				return false;
			}
		}
		return !IsEmpty();
	}

	bool String::IsEmptyOrWhitespace() const noexcept
	{
		return IsEmpty() || IsWhitespace();
	}

	bool String::HasAlpha() const noexcept
	{
		for (const char c : *this)
		{
			if (std::isalpha(c))
			{
				return true;
			}
		}
		return false;
	}

	bool String::IsLower() const noexcept
	{
		for (const char c : *this)
		{
			if (std::isalpha(c) && !std::islower(c))
			{
				return false;
			}
		}
		return HasAlpha();
	}

	bool String::IsUpper() const noexcept
	{
		for (const char c : *this)
		{
			if (std::isalpha(c) && !std::isupper(c))
			{
				return false;
			}
		}
		return HasAlpha();
	}
#pragma endregion

#pragma region util
	String String::ToLower() const noexcept
	{
		std::string temp = *this;
		for (char& c : temp)
		{
			c = static_cast<char>(std::tolower(c));
		}
		return temp;
	}

	String String::ToUpper() const noexcept
	{
		std::string temp = *this;
		for (char& c : temp)
		{
			c = static_cast<char>(std::toupper(c));
		}
		return temp;
	}

	String String::RemoveWhitespace() const noexcept
	{
		std::string temp = *this;
		temp.erase(std::remove_if(temp.begin(), temp.end(), isspace), temp.end());
		return temp;
	}

	String String::ReplaceAll(const String& from, const String& to) const
	{
		std::string temp = *this;
	    size_t start_pos = 0;
	    while ((start_pos = temp.find(from, start_pos)) != std::string::npos)
	    {
	        temp.replace(start_pos, from.Length(), to);
	        start_pos += to.Length();
	    }
	    return temp;
	}
#pragma endregion

#pragma region iterator
	String::const_iterator String::begin() const noexcept
	{
		return intern->string.begin();
	}
	
	String::const_iterator String::end() const noexcept
	{
		return intern->string.end();
	}

	String::const_iterator String::cbegin() const noexcept
	{
		return intern->string.cbegin();
	}

	String::const_iterator String::cend() const noexcept
	{
		return intern->string.cend();
	}
#pragma endregion

	size_t String::NumInterned() noexcept
	{
		return set.size();
	}

#pragma region operators
	String String::operator+(const char* string) const noexcept
	{
		return String(intern->string + string);
	}

	String& String::operator+=(const char* string) noexcept
	{
		return *this = *this + string;
	}

	String String::operator+(const String& other) const noexcept
	{
		return String(intern->string + other.intern->string);
	}

	String& String::operator+=(const String& other) noexcept
	{
		return *this = *this + other;
	}
#pragma endregion

#pragma region literals
	String Literals::operator""_s(const char* str, const size_t length) noexcept
	{
		return String(std::string(str, length));
	}

	String Literals::operator ""_s(const wchar_t* str, const size_t length) noexcept
	{
		return String(std::wstring(str, length));
	}
#pragma endregion
}
