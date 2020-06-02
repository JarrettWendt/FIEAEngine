#pragma once

#include "Attributed.h"
#include "Reflection.h"

namespace Library
{
	class Registry final
	{
	public:
		STATIC_CLASS(Registry)
		
		struct Attribute final
		{
			/** the name of this data member */
			std::string name{ "" };

			/** pointer to a constructor in the Reflection static class */
			const Reflection::ConstructorWrapper* constructor{ nullptr };

			/** how many of these there are */
			size_t count{ 0 };

			/** the offsetof this data member */
			size_t byteOffset{ 0 };

			/** the type of this data member */
			Datum::Type type{ Datum::Type::None };
		};

		struct Attributes final
		{
			/** used for recursively getting base Attributes */
			RTTI::IDType base;

			/** the Attributes for this Attributed */
			Array<Attribute> attributes;
		};

	private:
		/** defined in Registry.generated.cpp */
		static const HashMap<RTTI::IDType, Attributes> registry;

	public:
		/**
		 * O(1)
		 * 
		 * @param id	ID of an Attributed to query for
		 * @returns		if an Attributed of this ID is in the registry
		 */
		//static bool Contains(RTTI::IDType id) noexcept;

		/**
		 * O(n) where n is the number of Attributes for this Attributed
		 * 
		 * @param id	ID of an Attributed to query for
		 * @param name	name of an Attribute on that Attributed
		 * @returns		whether or not that ID/name pair exist in the registry
		 */
		//static bool Contains(RTTI::IDType id, const std::string& name) noexcept;

		/**
		 * O(n) where n is the number of Attributes for this Attributed
		 * 
		 * @param id	ID of an Attributed to query for
		 * @param name	name of an Attribute on that Attributed
		 * @returns		the Attributed matching the ID/name pair
		 *
		 * @throws std::out_of_range if no Attribute of that ID/name pair exists
		 */
		static Attribute& Find(RTTI::IDType id, const std::string& name);

		/**
		 * O(1)
		 * 
		 * @param id	ID of an Attributed to query for
		 * @return		the Attributes for that Attributed
		 *
		 * @throws std::invalid_argument if no Attribute of that ID exists
		 */
		//static const Attributes& Find(RTTI::IDType id);

		/**
		 * Loops through every Attribute for the given id.
		 * Recursively does so for base types too.
		 * 
		 * @param id	ID of an Attributed to query for
		 * @param func	functor to invoke on every Attribute
		 */
		template<typename Invokable>
		static void ForEach(RTTI::IDType id, Invokable func);
	};
}

#include "Registry.inl"
