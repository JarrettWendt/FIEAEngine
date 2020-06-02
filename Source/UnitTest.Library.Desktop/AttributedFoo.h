// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Attributed.h"

namespace UnitTests
{
	[[Reflectable]];
    class AttributedFoo final : public Library::Attributed
    {
        ATTRIBUTED_DECLARATIONS(Library::Attributed)

    public:
		[[Attribute]]
		int integer{ 0 };
		[[Attribute]]
		int integers[10]{ 0, 1, 2, 3, 4, 5, 5, 7, 8, 9 };

		[[Attribute]]
		float Float{ 0.f };
		[[Attribute]]
		float floats[10]{};

		[[Attribute]]
		std::string string{};
		[[Attribute]]
		std::string strings[10]{};

		[[Attribute]]
		std::shared_ptr<UnitTests::Foo> foo{};
		[[Attribute]]
		std::shared_ptr<UnitTests::Foo> foos[10]{};

		[[Attribute]]
		std::shared_ptr<Scope> scope{ std::make_shared<Scope>() };

		// Prescribed attributes without data members.
		[[Attribute(int, "intWithoutMember")]]
		[[Attribute(float, "floatWithoutmember")]]
		[[Attribute(std::string, "stringWithoutMember")]];
		[[Attribute(std::shared_ptr<UnitTests::Foo>, "fooWithoutMember")]]
		[[Attribute(std::shared_ptr<Scope>, "scopeWithoutMember")]]
		[[Attribute(std::shared_ptr<UnitTests::AttributedFoo>, "recursive")]];

		ATTRIBUTED_SPECIAL_MEMBERS(AttributedFoo, default)
    };
}
