// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "Datum.h"
#include "RTTI.h"
#include "HashMap.h"

namespace Library
{
	/**
	 * A Scope can only exist as a shared_ptr 
	 */
	[[Reflectable]];
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(RTTI)
		friend class Attributed;

	private:
		using SharedScope = std::shared_ptr<Scope>;
		using MapType = HashMap<std::string, Datum>;

		/** where all the data is stored */
		MapType map{};

	public:
		class InvalidNameException final : public std::invalid_argument
		{
		public:
			explicit InvalidNameException(const std::string& str) : std::invalid_argument(str) {};
			SPECIAL_MEMBERS(InvalidNameException, default)
		};
		
		/**
		 * @param capacity		initial capacity to give the inernal HashMap
		 */
		explicit Scope(size_t capacity);
		
		SPECIAL_MEMBERS(Scope, default)

		using iterator = typename MapType::iterator;
		using const_iterator = typename MapType::const_iterator;

		BEGIN_END(iterator, const_iterator, Scope)

#pragma region Properties
		/**
		 * @returns		how many Datums are in this Scope
		 */
		[[nodiscard]] constexpr size_t Size() const noexcept;

		/**
		 * @returns		if this Scope has no Datums
		 */
		[[nodiscard]] constexpr bool IsEmpty() const noexcept;
#pragma endregion

#pragma region Accessors
		/**
		 * O(1)
		 *
		 * @param name	key to query for
		 * @returns		Datum at that key
		 *
		 * @throws std::out_of_range if no Datum with that key exists
		 */
		[[nodiscard]] Datum& At(const std::string& name);

		/**
		 * O(1)
		 *
		 * @param name	key to query for
		 * @returns		Datum at that key
		 *
		 * @throws std::out_of_range if no Datum with that key exists
		 */
		[[nodiscard]] const Datum& At(const std::string& name) const;
#pragma endregion

#pragma region Query
		/**
		 * O(n)
		 * 
		 * @param d		Datum to query for
		 * @returns		name of d in this Scope, or empty string if no such Datum exists
		 */
		const std::string& NameOf(const Datum& d) const;

		/**
		 * O(n)
		 * 
		 * @param d		Datum to query for
		 * @returns		whether or not this Scope contains that Datum
		 */
		bool Contains(const Datum& d) const noexcept;

		/**
		 * O(1)
		 * 
		 * @param name	key to query for
		 * @returns		whether or not a Datum of that key exists in this Scope
		 */
		bool Contains(const std::string& name) const noexcept;

		/**
		 * O(1)
		 * Searches within this Scope, not children or parents.
		 * 
		 * @param name		name of child to query for
		 * @returns			iterator at that Scope, or end() if no such child exists
		 */
		[[nodiscard]] iterator Find(const std::string& name) noexcept;

		/**
		 * O(1)
		 * Searches within this Scope, not children or parents.
		 *
		 * @param name		name of child to query for
		 * @returns			iterator at that Scope, or end() if no such child exists
		 */
		[[nodiscard]] const_iterator Find(const std::string& name) const noexcept;
#pragma endregion

#pragma region Insert
		// TODO: Emplace
		
		/**
		 * Does not overwrite any Datum already assocated with name.
		 * O(1)
		 * 
		 * @param name		name of the Datum
		 * @param datum		Datum to append, a copy of which will be made
		 * @returns			reference to newly inserted Datum
		 *
		 * @throws InvalidNameException
		 */
		Datum& Insert(const std::string& name, const Datum& datum = {});

		/**
		 * Does not overwrite any Datum already assocated with name.
		 * O(1)
		 *
		 * @param name		name of the Datum
		 * @param datum		Datum to append, a copy of which will be made
		 * @returns			reference to newly inserted Datum
		 *
		 * @throws InvalidNameException
		 */
		Datum& Insert(const std::string& name, Datum&& datum);
#pragma endregion

#pragma region Remove
		/**
		 * Does nothing if no Datum by the specified name exists in this Scope.
		 * Orphans any children if the associated Datum holds Scopes.
		 * O(1)
		 * 
		 * @param name		removes the Datum with the associated name
		 */
		void Remove(const std::string& name) noexcept;
#pragma endregion

		/**
		 * O(n)
		 * 
		 * @param a		rhs
		 * @param b		lhs
		 * @returns		whether these Scopes are the same
		 */
		[[nodiscard]] friend bool operator==(const Scope& a, const Scope& b) noexcept
		{
			return a.map == b.map;
		}

		/**
		 * O(n)
		 *
		 * @param a		rhs
		 * @param b		lhs
		 * @returns		whether these Scopes are not the same
		 */
		[[nodiscard]] friend bool operator!=(const Scope& a, const Scope& b) noexcept
		{
			return !operator==(a, b);
		}

		/**
		 * Will Append a default constructed Datum if none exists with the specified key.
		 * O(1)
		 * 
		 * @param name	name for Datum
		 * @returns		Datum at that name
		 *
		 * @throws InvalidNameException
		 */
		[[nodiscard]] Datum& operator[](const std::string& name);

		/**
		 * O(1)
		 *
		 * @param name	name for Datum
		 * @returns		Datum at that name
		 *
		 * @throws InvalidNameException
		 */
		[[nodiscard]] const Datum& operator[](const std::string& name) const;

	protected:
		/**
		 * @param name	the name to check
		 * @throws InvalidNameException
		 */
		static void ThrowName(const std::string& name);
	};
}

#include "Scope.inl"
