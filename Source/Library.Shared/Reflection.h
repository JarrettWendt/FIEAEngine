#pragma once

#include "HashMap.h"

namespace Library
{
	/**
	 * Put `[[Reflectable]];` above any class or struct you wish to be default-constructable by this static utility class.
	 */
	class Reflection final
	{
		friend class Registry;

		STATIC_CLASS(Reflection)

		/**
		 * @returns		shared_ptr of void that was make_shared of a real type
		 */
		using ConstructorWrapper = std::function<std::shared_ptr<void>()>;

		/** defined in Reflection.generated.cpp */
		static const HashMap<std::string, ConstructorWrapper> constructors;
		
	public:
		/**
		 * Default constructs an instance the class with the given name.
		 * O(1)
		 *
		 * @param <T>			the type to get your pointer as
		 * @param className		the name of the class you're looking for
		 * @returns				pointer to newly allocated type, or nullptr if the type could not be found
		 *
		 * @throws				whatever the constructor does
		 */
		template<typename T>
		static std::shared_ptr<T> Construct(const std::string& className);

	private:
		/**
		 * to be called in Registry.generated.cpp
		 * O(1)
		 * 
		 * @param className		the name of the class you're looking for
		 * @returns				address of the ConstructorWrapper if it exists, nullptr otherwise
		 */
		static const Reflection::ConstructorWrapper* GetConstructor(const std::string& className) noexcept;
	};
}

#include "Reflection.inl"
