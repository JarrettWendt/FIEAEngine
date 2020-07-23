#pragma once
#include "Entity.h"

namespace UnitTests
{
	[[Reflectable]];
	class TestEntity : public Library::Entity
	{
		ATTRIBUTED_DECLARATIONS(Entity)
		ENTITY_SPECIAL_MEMBERS(TestEntity)
	};
}
