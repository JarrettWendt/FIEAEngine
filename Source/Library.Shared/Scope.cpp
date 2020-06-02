#include "pch.h"
#include "Scope.h"

namespace Library
{
	Scope::Scope(const size_t capacity) :
		map(capacity) {}

#pragma region iterator
	inline Scope::iterator Scope::begin() noexcept
	{
		return map.begin();
	}

	inline Scope::iterator Scope::end() noexcept
	{
		return map.end();
	}
#pragma endregion

#pragma region Accessors
	Datum& Scope::At(const std::string & name)
	{
		return map.At(name);
	}
	
	const Datum& Scope::At(const std::string& name) const
	{
		return const_cast<Scope*>(this)->At(name);
	}
#pragma endregion
	
#pragma region Query
	const std::string& Scope::NameOf(const Datum& d) const
	{
		for (const auto& [string, datum] : map)
		{
			if (d == datum)
			{
				return string;
			}
		}
		return Util::EmptyString;
	}

	bool Scope::Contains(const Datum& d) const noexcept
	{
		return !NameOf(d).empty();
	}
	
	bool Scope::Contains(const std::string& name) const noexcept
	{
		return map.Contains(name);
	}
	
	Scope::iterator Scope::Find(const std::string& name) noexcept
	{
		return map.Find(name);
	}

	Scope::const_iterator Scope::Find(const std::string& name) const noexcept
	{
		return const_cast<Scope*>(this)->Find(name);
	}
#pragma endregion

#pragma region Insert
	Datum& Scope::Insert(const std::string& name, const Datum& datum)
	{
		ThrowName(name);
		return map.Emplace(name, datum).first->value;
	}

	std::shared_ptr<Scope> Scope::InsertScope(const std::string& name)
	{
		auto ret = Insert(name, Datum{ std::make_shared<Scope>() }).Back<SharedScope>();
		ret->nameInParent = name;
		ret->parent = this;
		return ret;
	}

	std::shared_ptr<Scope> Scope::Adopt(const std::string& name, SharedScope scope)
	{
		ThrowName(name);

		if (scope->parent != this)
		{
			Datum& d = map[name];
			if (d.IsEmpty() || d.IsType<SharedScope>())
			{
				auto child = d.EmplaceBack<SharedScope>(std::move(scope));

				child->Orphan();
				child->parent = this;
				child->nameInParent = name;

				return child;
			}
			throw std::invalid_argument("Datum of type " + Enum<Datum::Type>::ToString(d.GetType()) + "already exists for key " + name);
		}
		// already a child
		return scope;
	}
#pragma endregion

#pragma region Remove
	void Scope::Orphan() noexcept
	{
		if (parent)
		{
			if (const auto it = parent->Find(nameInParent))
			{
				auto& [name, datum] = *it;
				datum.Remove(*this);

				parent = nullptr;
				nameInParent = "";
			}
		}
	}

	void Scope::Remove(const std::string& name) noexcept
	{
		if (const auto it = map.Find(name))
		{
			auto& [_, datum] = *it;
			if (datum.IsType<SharedScope>())
			{
				for (SharedScope& child : datum)
				{
					child->parent = nullptr;
					child->nameInParent = "";
				}
			}
			map.Remove(it);
		}
	}
#pragma endregion
	
	Datum& Scope::operator[](const std::string& name)
	{
		ThrowName(name);
		return map[name];
	}

	const Datum& Scope::operator[](const std::string& name) const
	{
		ThrowName(name);
		return map.At(name);
	}

	void Scope::ThrowName(const std::string& name)
	{
		if (Util::IsEmptyOrWhitespace(name)) [[unlikely]]
		{
			throw InvalidNameException("name cannot be empty or whitespace");
		}
	}
}
