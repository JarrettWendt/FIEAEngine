#include "pch.h"
#include "Registry.h"

#include "E:/Dev/Students/CH16/Jarrett.Wendt/Prog3/FIEAEngine/Source/UnitTest.Library.Desktop/AttributedFoo.h"
#include "E:/Dev/Students/CH16/Jarrett.Wendt/Prog3/FIEAEngine/Source/Library.Shared/Entity.h"

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
					{ "Float", Reflection::GetConstructor("float"), 1, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::Float), Datum::TypeOf<float>,  },
					{ "floats", Reflection::GetConstructor("float"), 10, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::floats), Datum::TypeOf<float>,  },
					{ "string", Reflection::GetConstructor("string"), 1, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::string), Datum::TypeOf<std::string>,  },
					{ "strings", Reflection::GetConstructor("string"), 10, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::strings), Datum::TypeOf<std::string>,  },
					{ "foo", Reflection::GetConstructor("Foo>"), 1, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::foo), Datum::TypeOf<std::shared_ptr<UnitTests::Foo>>,  },
					{ "foos", Reflection::GetConstructor("Foo>"), 10, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::foos), Datum::TypeOf<std::shared_ptr<UnitTests::Foo>>,  },
					{ "intWithoutMember", Reflection::GetConstructor("int"), 0, 0, Datum::TypeOf<int>,  },
					{ "floatWithoutmember", Reflection::GetConstructor("float"), 0, 0, Datum::TypeOf<float>,  },
					{ "stringWithoutMember", Reflection::GetConstructor("string"), 0, 0, Datum::TypeOf<std::string>,  },
					{ "fooWithoutMember", Reflection::GetConstructor("Foo>"), 0, 0, Datum::TypeOf<std::shared_ptr<UnitTests::Foo>>,  },
					{ "recursive", Reflection::GetConstructor("AttributedFoo>"), 0, 0, Datum::TypeOf<std::shared_ptr<UnitTests::AttributedFoo>>,  },
				}
			}
		},
		{
			Library::Entity::typeID,
			{
				Attributed::typeID,
				{
					{ "name", Reflection::GetConstructor("string"), 1, offsetof(Library::Entity, Library::Entity::name), Datum::TypeOf<std::string>,  },
					{ "enabled", Reflection::GetConstructor("bool"), 1, offsetof(Library::Entity, Library::Entity::enabled), Datum::TypeOf<bool>,  },
				}
			}
		},
	};
}
