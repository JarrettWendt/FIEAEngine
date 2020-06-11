#include "pch.h"
#include "Reflection.h"

#include "E:/Dev/Students/CH16/Jarrett.Wendt/Prog3/FIEAEngine/Source/Library.Shared/Entity.h"

namespace Library
{
	const HashMap<std::string, std::function<std::shared_ptr<void>()>> Reflection::constructors =
	{
		{ "Entity", [] { return std::make_shared<Library::Entity>(); } },
	};
}
