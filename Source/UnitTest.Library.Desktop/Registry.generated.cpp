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
					{ "integer", 1, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::integer), Datum::TypeOf<int>,  },
					{ "integers", 10, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::integers), Datum::TypeOf<int>,  },
					{ "Float", 1, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::Float), Datum::TypeOf<float>,  },
					{ "floats", 10, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::floats), Datum::TypeOf<float>,  },
					{ "string", 1, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::string), Datum::TypeOf<std::string>,  },
					{ "strings", 10, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::strings), Datum::TypeOf<std::string>,  },
					{ "foo", 1, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::foo), Datum::TypeOf<std::shared_ptr<UnitTests::Foo>>,  },
					{ "foos", 10, offsetof(UnitTests::AttributedFoo, UnitTests::AttributedFoo::foos), Datum::TypeOf<std::shared_ptr<UnitTests::Foo>>,  },
					{ "intWithoutMember", 0, 0, Datum::TypeOf<int>,  },
					{ "floatWithoutmember", 0, 0, Datum::TypeOf<float>,  },
					{ "stringWithoutMember", 0, 0, Datum::TypeOf<std::string>,  },
					{ "fooWithoutMember", 0, 0, Datum::TypeOf<std::shared_ptr<UnitTests::Foo>>,  },
					{ "recursive", 0, 0, Datum::TypeOf<std::shared_ptr<UnitTests::AttributedFoo>>,  },
				}
			}
		},
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
