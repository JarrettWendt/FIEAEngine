// MIT License Copyright (c) 2020 Jarrett Wendt

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
			String name{ "" };

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

			/** how many attributes exist for this derived type in total, including all base types */
			size_t num{ std::numeric_limits<size_t>::max() };
		};

	private:
		/** defined in Registry.generated.cpp */
		static const HashMap<RTTI::IDType, Attributes> registry;

	public:
		/**
		 * O(1) most cases
		 * O(n) on the first ever call, where n is the number of parents this derived type has
		 * 
		 * @param id	ID of an Attributed to query for
		 * @return		how many attributes exist for this type in total, including all base types
		 */
		static size_t NumAttributes(RTTI::IDType id) noexcept;

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
