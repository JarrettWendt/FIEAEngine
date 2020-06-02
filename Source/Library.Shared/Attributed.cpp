// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"

#include "Attributed.h"
#include "Registry.h"
#include "Reflection.h"

namespace Library
{
#pragma region Special Members
	Attributed::Attributed(const IDType derived) :
		Scope(Registry::NumAttributes(derived) + 1)
	{
		Populate(derived);
	}

	Attributed::Attributed(const Attributed& other) :
		Scope(other)
	{
		PointerFixup(other.TypeID());
	}

	Attributed::Attributed(Attributed&& other) noexcept :
		Scope(std::move(other))
	{
		PointerFixup(other.TypeID());
	}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		assertm(TypeID() == other.TypeID(), "lhs and rhs should have the same IDType on copy assignment");

		if (this != &other)
		{
			Scope::operator=(other);
			PointerFixup(TypeID());
		}
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& other) noexcept
	{
		assertm(TypeID() == other.TypeID(), "lhs and rhs should have the same IDType on move assignment");

		if (this != &other)
		{
			Scope::operator=(std::move(other));
			PointerFixup(TypeID());
		}
		return *this;
	}

	Attributed::~Attributed() {}
#pragma endregion
	
	void Attributed::Populate(const IDType derived)
	{
		Registry::ForEach(derived, [&](const Registry::Attribute& attribute)
		{
			const auto& [name, ctor, count, byteOffset, type] = attribute;
			if (byteOffset)
			{
				// prescribed with data member
				Scope::Insert(name, Datum(type, ByteOffsetThis(byteOffset), count, count));
			}
			else
			{
				// prescribed without data member
				Scope::Insert(name, Datum(type, count));
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
}
