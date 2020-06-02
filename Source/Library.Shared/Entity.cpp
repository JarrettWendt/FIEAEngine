#include "pch.h"
#include "Entity.h"

namespace Library
{
	constexpr bool& Entity::Enabled() noexcept
	{
		return enabled;
	}
	
	constexpr bool Entity::Enabled() const noexcept
	{
		return const_cast<Entity*>(this)->Enabled();
	}
	
	void Entity::Init()
	{
		// TODO: invoke all children Init
	}

	void Entity::Update()
	{
		// TODO: invoke all children Update
	}
}
