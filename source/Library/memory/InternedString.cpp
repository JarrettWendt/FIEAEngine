#include "InternedString.h"

using namespace std::string_literals;

namespace Library
{
	String::Intern::Intern(const std::string& string) noexcept :
		string(string),
		hash(std::hash<std::string>{}(string)) {}

	constexpr bool String::Compare::operator()(const std::shared_ptr<Intern>& left, const std::shared_ptr<Intern>& right) const
	{
		return left->hash < right->hash;
	}
	
	String::String() noexcept :
		String(""s) {}

	String::String(const char* string) noexcept :
		String(std::string(string)) {}
	
	String::String(const std::string& string) noexcept :
		intern(*set.emplace(std::make_shared<Intern>(string)).first) {}

	String::~String() noexcept
	{
		// If the only reference is ours and the one held by the set.
		if (intern.use_count() == 2)
		{
			set.erase(intern);
		}
	}
	
	constexpr bool String::IsEmpty() const noexcept
	{
		return intern->string.empty();
	}

	constexpr size_t String::Length() const noexcept
	{
		return intern->string.length();
	}

	constexpr char String::operator[](const size_t index) const noexcept
	{
		return intern->string[index];
	}

	constexpr char String::At(const size_t index) const
	{
		return intern->string.at(index);
	}

	constexpr char String::Front() const noexcept
	{
		return intern->string.front();
	}

	constexpr char String::Back() const noexcept
	{
		return intern->string.back();
	}

	constexpr const char* String::c_str() const noexcept
	{
		return intern->string.c_str();
	}

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

	size_t String::NumInterned() noexcept
	{
		return set.size();
	}

	String Literals::operator""_s(const char* str, const size_t length) noexcept
	{
		return String(std::string(str, length));
	}
}
