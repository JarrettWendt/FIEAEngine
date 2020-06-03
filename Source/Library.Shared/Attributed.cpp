// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"

#include "Attributed.h"
#include "Registry.h"
#include "Reflection.h"

namespace Library
{
#pragma region Special Members
	Attributed::Attributed(const IDType derived) :
		attributes(Registry::NumAttributes(derived) + 1)
	{
		Populate(derived);
	}

	Attributed::Attributed(const Attributed& other) :
		attributes(other.attributes)
	{
		PointerFixup(other.TypeID());
	}

	Attributed::Attributed(Attributed&& other) noexcept :
		attributes(std::move(other.attributes))
	{
		PointerFixup(other.TypeID());
	}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		assertm(TypeID() == other.TypeID(), "lhs and rhs should have the same IDType on copy assignment");

		if (this != &other)
		{
			attributes = other.attributes;
			PointerFixup(TypeID());
		}
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& other) noexcept
	{
		assertm(TypeID() == other.TypeID(), "lhs and rhs should have the same IDType on move assignment");

		if (this != &other)
		{
			attributes = std::move(other.attributes);
			PointerFixup(TypeID());
		}
		return *this;
	}

	Attributed::~Attributed() {};
#pragma endregion

#pragma region iterator
	Attributed::iterator Attributed::begin() noexcept
	{
		return attributes.begin();
	}

	Attributed::iterator Attributed::end() noexcept
	{
		return attributes.end();
	}
#pragma endregion

#pragma region Accessors
	Datum& Attributed::Attribute(const std::string& name)
	{
		return attributes.At(name);
	}
	
	const Datum& Attributed::Attribute(const std::string& name) const
	{
		return const_cast<Attributed*>(this)->Attribute(name);
	}
#pragma endregion

#pragma region Query
	bool Attributed::HasAttribute(const std::string& name) const noexcept
	{
		return attributes.Contains(name);
	}
	
	Attributed::iterator Attributed::Find(const std::string& name) noexcept
	{
		return attributes.Find(name);
	}
	
	Attributed::const_iterator Attributed::Find(const std::string& name) const noexcept
	{
		return const_cast<Attributed*>(this)->Find(name);
	}
#pragma endregion

#pragma region Insert
	Datum& Attributed::AddAttribute(const std::string& name, const Datum& datum)
	{
		ThrowName(name);
		return attributes.Insert(name, datum).first->value;
	}
	
	Datum& Attributed::AddAttribute(const std::string& name, Datum&& datum)
	{
		ThrowName(name);
		return attributes.Insert(name, std::move(datum)).first->value;
	}
#pragma endregion

	bool Attributed::RemoveAttribute(const std::string& name) noexcept
	{
		return attributes.Remove(name);
	}

#pragma region operators
	bool operator==(const Attributed& a, const Attributed& b) noexcept
	{
		return a.attributes == b.attributes;
	}
	
	bool operator!=(const Attributed& a, const Attributed& b) noexcept
	{
		return !operator==(a, b);
	}

	Datum& Attributed::operator[](const std::string& name)
	{
		return attributes[name];
	}
	
	const Datum& Attributed::operator[](const std::string& name) const
	{
		return attributes[name];
	}
#pragma endregion

#pragma region Helpers
	void Attributed::ThrowName(const std::string& name)
	{
		if (Util::IsEmptyOrWhitespace(name)) [[unlikely]]
		{
			throw InvalidNameException("name cannot be empty or whitespace");
		}
	}
	
	void Attributed::Populate(const IDType derived)
	{
		Registry::ForEach(derived, [&](const Registry::Attribute& attribute)
		{
			const auto& [name, ctor, count, byteOffset, type] = attribute;
			if (byteOffset)
			{
				// prescribed with data member
				AddAttribute(name, Datum(type, ByteOffsetThis(byteOffset), count, count));
			}
			else
			{
				// prescribed without data member
				AddAttribute(name, Datum(type, count));
			}
		});
	}
	
	void Attributed::PointerFixup(const IDType derived) noexcept
	{
		for (auto& [name, datum] : *this)
		{
			if (datum.IsExternal())
			{
				datum.SetStorage(ByteOffsetThis(Registry::Find(derived, name).byteOffset));
			}
		}
	}
#pragma endregion
}
