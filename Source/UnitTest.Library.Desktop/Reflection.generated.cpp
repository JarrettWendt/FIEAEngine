#include "pch.h"
#include "Reflection.h"

#include "E:/Dev/Students/CH16/Jarrett.Wendt/Prog3/FIEAEngine/Source/UnitTest.Library.Desktop/AttributedFoo.h"
#include "E:/Dev/Students/CH16/Jarrett.Wendt/Prog3/FIEAEngine/Source/Library.Shared/Entity.h"
#include "E:/Dev/Students/CH16/Jarrett.Wendt/Prog3/FIEAEngine/Source/Library.Shared/Scope.h"

namespace Library
{
	const HashMap<std::string, std::function<std::shared_ptr<void>()>> Reflection::constructors =
	{
		{ "AttributedFoo", [] { return std::make_shared<UnitTests::AttributedFoo>(); } },
		{ "Entity", [] { return std::make_shared<Library::Entity>(); } },
		{ "Scope", [] { return std::make_shared<Library::Scope>(); } },
	};
}
