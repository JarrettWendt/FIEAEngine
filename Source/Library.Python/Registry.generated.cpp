#include "pch.h"
#include "Registry.h"

#include "E:/Dev/Students/CH16/Jarrett.Wendt/Prog3/FIEAEngine/Source/Library.Shared/Entity.h"

namespace Library
{
	const HashMap<RTTI::IDType, Registry::Attributes> Registry::registry =
	{
		{
			Library::Entity::typeID,
			{
				Attributed::typeID,
				{
					{ "name", 1, offsetof(Library::Entity, Library::Entity::name), Datum::TypeOf<std::string>,  },
					{ "enabled", 1, offsetof(Library::Entity, Library::Entity::enabled), Datum::TypeOf<bool>,  },
				}
			}
		},
	};
}
