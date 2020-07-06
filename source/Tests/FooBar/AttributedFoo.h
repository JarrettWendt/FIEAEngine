// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Attributed.h"
#include "Foo.h"

namespace UnitTests
{
	[[Reflectable]];
    class AttributedFoo final : public Library::Attributed
    {
        ATTRIBUTED_DECLARATIONS(Library::Attributed)

    public:
#pragma region primitives
		[[Attribute]]
    	bool boolean{};
    	[[Attribute]]
    	bool booleans[10]{};
    	
		[[Attribute]]
		int integer{};
		[[Attribute]]
		int integers[10]{ 0, 1, 2, 3, 4, 5, 5, 7, 8, 9 };

		[[Attribute]]
		float Float{ 0.f };
		[[Attribute]]
		float floats[10]{};
#pragma endregion

#pragma region math types
    	[[Attribute]]
        Library::Vector2 vector2{};
    	[[Attribute]]
    	Library::Vector2 vector2s[10]{};
    	
    	[[Attribute]]
        Library::Vector3 vector3{};
    	[[Attribute]]
    	Library::Vector3 vector3s[10]{};
    	
    	[[Attribute]]
        Library::Vector4 vector4{};
    	[[Attribute]]
    	Library::Vector4 vector4s[10]{};
    	
    	[[Attribute]]
        Library::Quaternion quaternion{};
    	[[Attribute]]
    	Library::Quaternion quaternions[10]{};

    	[[Attribute]]
    	Library::Matrix matrix{};
    	[[Attribute]]
    	Library::Matrix matrices[10]{};

    	[[Attribute]]
    	Library::Transform transform{};
    	[[Attribute]]
    	Library::Transform transforms[10]{};
#pragma endregion

#pragma region object types
		[[Attribute]]
		std::string string{};
		[[Attribute]]
		std::string strings[10]{};

		[[Attribute]]
		std::shared_ptr<UnitTests::Foo> foo{};
		[[Attribute]]
		std::shared_ptr<UnitTests::Foo> foos[10]{};
#pragma endregion
    	
		// Prescribed attributes without data members.
		[[Attribute(bool, "boolWithoutMember")]]
		[[Attribute(int, "intWithoutMember")]]
		[[Attribute(float, "floatWithoutmember")]]
		[[Attribute(Library::Vector2, "vector2Withoutmember")]]
		[[Attribute(Library::Vector3, "vector3Withoutmember")]]
		[[Attribute(Library::Vector4, "vector4Withoutmember")]]
		[[Attribute(Library::Quaternion, "quaternionWithoutmember")]]
		[[Attribute(Library::Matrix, "matrixWithoutmember")]]
		[[Attribute(Library::Transform, "transformWithoutmember")]]
		[[Attribute(std::string, "stringWithoutMember")]]
		[[Attribute(std::shared_ptr<UnitTests::Foo>, "fooWithoutMember")]]
		[[Attribute(std::shared_ptr<UnitTests::AttributedFoo>, "recursive")]];

		ATTRIBUTED_SPECIAL_MEMBERS(AttributedFoo, default)
    };
}
