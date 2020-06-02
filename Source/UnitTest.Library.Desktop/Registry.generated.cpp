#include "pch.h"
#include "Registry.h"

#include "E:/Dev/Students/CH16/Jarrett.Wendt/Prog3/FIEAEngine/Source/UnitTest.Library.Desktop/AttributedFoo.h"

namespace Library
{
	const HashMap<RTTI::IDType, Registry::Attributes> Registry::registry =
	{
		{
			UnitTests::AttributedFoo::typeID,
			{
				Attributed::typeID,
				{
					{ "integer", Reflection::GetConstructor("int"), 1, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::integer), Datum::TypeOf<int>,  },
					{ "integers", Reflection::GetConstructor("int"), 10, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::integers), Datum::TypeOf<int>,  },
				}
			}
		},
	};
}
