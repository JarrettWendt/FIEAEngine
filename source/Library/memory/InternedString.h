#pragma once
#include <set>
#include <string>

#include "Hash.h"

namespace Library
{
	class String
	{
		struct Intern
		{
			const std::string string;
			const hash_t hash;

			explicit Intern(const std::string& string) noexcept;
		};
		
		struct Compare
		{
			constexpr bool operator()(const std::shared_ptr<Intern>& left, const std::shared_ptr<Intern>& right) const;
		};
		
		static inline std::set<std::shared_ptr<Intern>, Compare> set{};

		std::shared_ptr<Intern> intern;

	public:
		String() noexcept;
		String(const char* string) noexcept;
		String(const std::string& string) noexcept;
		String(const String& other) noexcept = default;
		String(String&& other) noexcept = default;
		String& operator=(const String& other) noexcept = default;
		String& operator=(String&& other) noexcept = default;
		~String() noexcept;

		constexpr bool IsEmpty() const noexcept;
		constexpr size_t Length() const noexcept;

		constexpr char operator[](size_t index) const noexcept;
		constexpr char At(size_t index) const;

		constexpr char Front() const noexcept;
		constexpr char Back() const noexcept;

		constexpr const char* c_str() const noexcept;
		
		using const_iterator = std::string::const_iterator;
		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		static size_t NumInterned() noexcept;
	};

	namespace Literals
	{
		String operator""_s(const char* str, size_t length) noexcept;
	}
}
