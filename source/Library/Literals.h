// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

namespace Library::Literals
{	
	constexpr std::size_t operator""_z(unsigned long long int x);

	constexpr std::ptrdiff_t operator""_ptrdiff(unsigned long long int x);

	namespace
	{
		// https://gist.github.com/mattbierner/5c698972de0cdd9de86a
		
		constexpr unsigned digit_to_value(const char c)
		{
			if (c >= 'a' && c <= 'f') return c - 'a' + 10;
			if (c >= 'A' && c <= 'F') return c - 'A' + 10;
			if (c >= '0' && c <= '9') return c - '0';
			throw std::invalid_argument("c");
		}

		template <typename sum, char... digits>
		struct GetDigits
		{
			using type = sum;
		};

		template<unsigned... existing, char... xs>
		struct GetDigits<std::integer_sequence<unsigned, existing...>, '\'', xs...> : GetDigits<std::integer_sequence<unsigned, existing...>, xs...>{};

		template<unsigned... existing, char x, char... xs>
		struct GetDigits<std::integer_sequence<unsigned, existing...>, x, xs...> : GetDigits<std::integer_sequence<unsigned, existing..., digit_to_value(x)>, xs...>{};

		template<unsigned b, char... d>
		struct BaseAndDigits
		{
			static constexpr unsigned base = b;
			using digits = typename GetDigits<std::integer_sequence<unsigned>, d...>::type;
		};

		template<char... digits>
		struct ParseNumber : BaseAndDigits<10, digits...>{};

		template<char... digits>
		struct ParseNumber<'0', 'X', digits...> : BaseAndDigits<16, digits...>{};
		template<char... digits>
		struct ParseNumber<'0', 'x', digits...> : BaseAndDigits<16, digits...>{};

		template<char... digits>
		struct ParseNumber<'0', digits...> : BaseAndDigits<8, digits...>{};

		template<char... digits>
		struct ParseNumber<'0', 'b', digits...> : BaseAndDigits<2, digits...>{};
		template<char... digits>
		struct ParseNumber<'0', 'B', digits...> : BaseAndDigits<2, digits...>{};

		template<typename T, char... values>
		struct ConstantFromString
		{
			using number = ParseNumber<values...>;

			template<unsigned x, unsigned... xs>
			static constexpr unsigned long long fold(unsigned long sum, std::integer_sequence<unsigned, x, xs...>)
			{
				return fold(x + number::base * sum, std::integer_sequence<unsigned, xs...>{});
			}

			static constexpr unsigned long long fold(const unsigned long sum, std::integer_sequence<unsigned>)
			{
				return sum;
			}

			using type = std::integral_constant<T, static_cast<T>(fold(0, typename number::digits{}))>;
		};
	}

	template<char... digits>
	constexpr auto operator""_zc()
	{
	    return typename ConstantFromString<size_t, digits...>::type{};
	}
}

#include "Literals.inl"
