#pragma once

#include "Enum.h"
#include "Util.h"
#include "HashMap.h"

namespace UnitTests
{
	enum class Digit
	{
		Zero,
		One,
		Two,
		Three,
		Four,
		Five,
		Six,
		Seven,
		Eight,
		Nine,

		Begin = Zero,
		End = Nine
	};
}

namespace Library
{
	template<>
	inline const std::string& Enum<UnitTests::Digit>::ToString(const UnitTests::Digit t)
	{
		static std::string array[] =
		{
			"Zero",
			"One",
			"Two",
			"Three",
			"Four",
			"Five",
			"Six",
			"Seven",
			"Eight",
			"Nine",
			"DNE"
		};
		return array[static_cast<size_t>(t)];
	}

	template<>
	inline UnitTests::Digit Enum<UnitTests::Digit>::FromString(const std::string& str)
	{
		static HashMap<std::string, UnitTests::Digit> map =
		{
			{ "zero", UnitTests::Digit::Zero},
			{ "one", UnitTests::Digit::One},
			{ "two", UnitTests::Digit::Two},
			{ "three", UnitTests::Digit::Three},
			{ "four", UnitTests::Digit::Four},
			{ "five", UnitTests::Digit::Five},
			{ "six", UnitTests::Digit::Six},
			{ "seven", UnitTests::Digit::Seven},
			{ "eight", UnitTests::Digit::Eight},
			{ "nine", UnitTests::Digit::Nine},
		};
		const std::string key = Util::ToLower(Util::RemoveWhitespace(str));
		if (!map.Contains(key))
		{
			throw std::invalid_argument("Could not parse Digit from " + key);
		}
		return map.At(key);
	}
}
