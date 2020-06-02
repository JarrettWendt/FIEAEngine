// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Datum.h"

#include "HashMap.h"

namespace Library
{
#pragma region Datum
	Datum::Datum(const Datum& other)
	{
		if (other.IsExternal())
		{
			Memory::Memcpy(this, &other);
		}
		else
		{
			Base::operator=(static_cast<const Base&>(other));
		}
	}
	
	Datum& Datum::operator=(const Datum& other)
	{
		if (this != &other)
		{
			if (other.IsExternal())
			{
				if (IsInternal())
				{
					Empty();
				}
				Memory::Memcpy(this, &other);
			}
			else
			{
				if (IsExternal())
				{
					// zero everything so Array::operator= won't free anything
					Memory::Memset(this, 0);
				}
				Base::operator=(static_cast<const Base&>(other));
			}
		}
		return *this;
	}

	Datum::~Datum()
	{
		if (IsExternal())
		{
			// zero everything so ~Array() won't do anything
			Memory::Memset(this, 0);
		}
	}

	inline constexpr bool Datum::IsExternal() const noexcept
	{
		return isExternal;
	}
	
	void Datum::Empty() noexcept
	{
		ThrowExternal();
		Base::Empty();
	}

#pragma region Memory
	void Datum::Reserve(const size_t capacity)
	{
		ThrowExternal();
		Base::Reserve(capacity);
	}

	void Datum::ShrinkToFit()
	{
		ThrowExternal();
		Base::ShrinkToFit();
	}

	void Datum::ShrinkToFit(const size_t count)
	{
		ThrowExternal();
		Base::ShrinkToFit(count);
	}
#pragma endregion

	void Datum::ThrowExternalIfFull() const
	{
		if (IsFull())
		{
			ThrowExternal();
		}
	}

	inline void Datum::ThrowExternal() const
	{
		if (IsExternal())
		{
			throw ExternalStorageException("Attempted to mutate memory of external storage Datum.");
		}
	}
#pragma endregion

	Datum::Type TypeOf(const Datum::value_type val) noexcept
	{
		return Datum::Type(val.TypeID());
	}

	const std::string& Enum<Datum::Type>::ToString(const Datum::Type t)
	{
		static std::string array[] =
		{
			"None",

			"Bool",
			"Int",
			"Float",
			"String",
			"RTTI",
		};
		return array[static_cast<size_t>(t)];
	}

	Datum::Type Enum<Datum::Type>::FromString(const std::string& str)
	{
		using Type = Datum::Type;
		static HashMap<std::string, Type> map =
		{
			{ "None", Type::None },

			{ "Bool", Type::Bool },
			{ "Int", Type::Int },
			{ "Float", Type::Float },
			{ "String", Type::String },
			{ "RTTI", Type::RTTI },
		};
		const auto it = map.Find(Util::ToLower(Util::RemoveWhitespace(str)));
		return it ? it->value : Type::None;
	}
}
