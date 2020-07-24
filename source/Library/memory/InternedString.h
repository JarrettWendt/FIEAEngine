#pragma once
#include <string>
#include <unordered_set>

#include "HashMap.h"
#include "SharedPtr.h"

namespace Library
{
	class String
	{
		// TODO: As-is there's 2 pointer indirects:
		// 1: dereferencing the std::shared_ptr
		// 2: std::string's internal pointer indirect
		// This could be mitigated by abandoning std::string and rolling our own solution built-in to Intern.
		// So Intern's memory layout would be...
		// hash, strlen, array of chars (not a pointer, the actual data starts right after strlen)
		// So we could do... reinterpret_cast<char*>(reinterpret_cast<uint8_t*>(&myIntern) + sizeof(size_t) * 2)
		struct Intern
		{
			const std::string string;
			const hash_t hash;

			explicit Intern(const std::string& string) noexcept;
		};

		using SharedIntern = std::shared_ptr<Intern>;

		template<typename T, typename... Ts>
		friend struct Hash;
		
		struct Hash
		{
			constexpr hash_t operator()(const SharedIntern& i) const;
		};

		struct KeyEqual
		{
			bool operator()(const SharedIntern& a, const SharedIntern& b) const;
		};
		
		static inline std::unordered_set<SharedIntern, Hash, KeyEqual> set{};

		SharedIntern intern;

	public:
		String() noexcept;
		String(const char* string) noexcept;
		String(const wchar_t* string) noexcept;
		String(const std::string& string) noexcept;
		String(const std::wstring& string) noexcept;
		String(const String& other) noexcept = default;
		String(String&& other) noexcept = default;
		String& operator=(const String& other) noexcept;
		String& operator=(String&& other) noexcept;
		~String() noexcept;

		constexpr operator const std::string&() const noexcept
		{
			return intern->string;
		}

		constexpr size_t Length() const noexcept;
		constexpr bool IsEmpty() const noexcept;
		bool IsWhitespace() const noexcept;
		bool IsEmptyOrWhitespace() const noexcept;
		bool HasAlpha() const noexcept;
		bool IsLower() const noexcept;
		bool IsUpper() const noexcept;

		String ToLower() const noexcept;
		String ToUpper() const noexcept;
		String RemoveWhitespace() const noexcept;
		String ReplaceAll(const String& from, const String& to) const;

#pragma region element access
		constexpr char operator[](const size_t index) const noexcept
		{
			return intern->string[index];
		}
		
		constexpr char At(const size_t index) const
		{
			return intern->string.at(index);
		}

		constexpr char Front() const noexcept
		{
			return intern->string.front();
		}
		
		constexpr char Back() const noexcept
		{
			return intern->string.back();
		}
#pragma endregion
		
		constexpr const std::string& str() const noexcept
		{
			return intern->string;
		}
		
		constexpr const char* c_str() const noexcept
		{
			return intern->string.c_str();
		}
		
		using const_iterator = std::string::const_iterator;
		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		static size_t NumInterned() noexcept;

		[[nodiscard]] String operator+(const char* string) const noexcept;
		String& operator+=(const char* string) noexcept;
		
		[[nodiscard]] String operator+(const String& other) const noexcept;
		String& operator+=(const String& other) noexcept;

		friend constexpr bool operator==(const String& a, const String& b) noexcept
		{
			return a.intern->hash == b.intern->hash && a.intern->string == b.intern->string;
		}
		
		friend bool operator<(const String& a, const String& b) noexcept
		{
			return a.intern->string < b.intern->string;
		}

		FRIEND_COMPARISONS(String)

		friend std::ostream& operator<<(std::ostream& stream, const String& s)
		{
			return stream << s.intern->string;
		}
	};

	namespace Literals
	{
		String operator""_s(const char* str, size_t length) noexcept;
		String operator""_s(const wchar_t* str, size_t length) noexcept;
	}

	template<>
	struct Hash<String>
	{
		hash_t operator()(const String& s) const
		{
			return s.intern->hash;
		}
	};
}

namespace std
{
	template<>
	struct hash<Library::String>
	{
		size_t operator()(const Library::String& s) const noexcept
		{
			return Library::Hash<Library::String>{}(s);
		}
	};
}

#include "InternedString.inl"
