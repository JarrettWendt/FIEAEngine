#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "Entity::" #name, "[Entity]")

namespace UnitTests
{		
	TEST(Destruction)
	{
		std::weak_ptr<Entity> ptr;
		{
			const auto e = std::make_shared<Entity>();
			ptr = e;
		}
		REQUIRE(ptr.expired());
	}

#pragma region iterator
	TEST(ConstIterator)
	{
		const auto e = std::make_shared<Entity>();

		auto cit = e->cbegin();
		REQUIRE(cit == e->cend());
		
		REQUIRE(cit.IsAtBegin());
		REQUIRE(cit.IsAtEnd());

		REQUIRE(!cit);
		REQUIRE(!cit);

		const auto c = e->CreateChild();
		cit = e->begin();

		REQUIRE(c == *cit);
		REQUIRE(e == cit->Parent());

		cit++;
		REQUIRE(!cit);
	}
#pragma endregion

#pragma region Properties
	TEST(Child)
	{
		const auto p = std::make_shared<Entity>();
		REQUIRE(!p->Child("bleh"));
		
		const auto c = p->CreateChild();
		const std::shared_ptr<const Entity> cp = p;

		REQUIRE(c == cp->Child(c->GetName()));
	}

	TEST(Parent)
	{
		const auto p = std::make_shared<Entity>();
		const auto c = p->CreateChild();
		const std::shared_ptr<const Entity> cc = c;

		REQUIRE(p == cc->Parent());
	}
#pragma endregion

#pragma region Transform
	TEST(SetLocalTransformFromParent)
	{			
		const auto p = std::make_shared<Entity>();
		const auto c = p->CreateChild<>();

		REQUIRE(p->GetLocalTransform() == p->GetWorldTransform());
		REQUIRE(p->GetWorldTransform() == c->GetWorldTransform());

		auto t = p->GetLocalTransform();
		t.translation = { 0, 0, 1 };
		p->SetLocalTransform(t);

		REQUIRE(t == p->GetLocalTransform());
		REQUIRE(t == p->GetWorldTransform());
		REQUIRE(t == c->GetWorldTransform());
		REQUIRE(Transform{} == c->GetLocalTransform());
	}

	TEST(SetWorldTransformFromParent)
	{
		const auto p = std::make_shared<Entity>();
		const auto c = p->CreateChild<>();

		REQUIRE(p->GetLocalTransform() == p->GetWorldTransform());
		REQUIRE(p->GetWorldTransform() == c->GetWorldTransform());

		auto t = p->GetLocalTransform();
		t.translation = { 0, 0, 1 };
		p->SetWorldTransform(t);

		REQUIRE(t == p->GetLocalTransform());
		REQUIRE(t == p->GetWorldTransform());
		REQUIRE(t == c->GetWorldTransform());
		REQUIRE(Transform{} == c->GetLocalTransform());
	}
	
	TEST(SetLocalTransformFromChild)
	{
		const auto p = std::make_shared<Entity>();
		const auto c = p->CreateChild<>();

		REQUIRE(p->GetLocalTransform() == p->GetWorldTransform());
		REQUIRE(p->GetWorldTransform() == c->GetWorldTransform());

		auto t = c->GetLocalTransform();
		t.translation = { 0, 0, 1 };
		c->SetLocalTransform(t);

		REQUIRE(Transform{} == p->GetLocalTransform());
		REQUIRE(Transform{} == p->GetWorldTransform());
		REQUIRE(t == c->GetWorldTransform());
		REQUIRE(t == c->GetLocalTransform());
	}

	TEST(SetWorldTransformFromChild)
	{
		const auto p = std::make_shared<Entity>();
		const auto c = p->CreateChild<>();

		REQUIRE(p->GetLocalTransform() == p->GetWorldTransform());
		REQUIRE(p->GetWorldTransform() == c->GetWorldTransform());

		auto t = c->GetLocalTransform();
		t.translation = { 0, 0, 1 };
		c->SetWorldTransform(t);

		REQUIRE(Transform{} == p->GetLocalTransform());
		REQUIRE(Transform{} == p->GetWorldTransform());
		REQUIRE(t == c->GetWorldTransform());
		REQUIRE(t == c->GetLocalTransform());
	}
#pragma endregion

	TEST(SetName)
	{
		const auto p = std::make_shared<Entity>();
		const auto c = p->CreateChild<>("child"s);
		c->SetName("Seven");
		REQUIRE("Seven"s == c->GetName());
		REQUIRE(p->Child("Seven"));
	}

	TEST(GetName)
	{
		const auto e = std::make_shared<Entity>();
		REQUIRE("Entity"s == e->GetName());
	}
		
#pragma region Insert
	TEST(Insert)
	{
		REQUIRE_THROWS_AS(Entity().CreateChild(""s), Entity::InvalidNameException);
	}

	TEST(InsertScope)
	{
		auto parent = std::make_shared<Entity>();
		const auto child = parent->CreateChild("child"s);

		REQUIRE(parent == child->Parent());
		REQUIRE("child" == child->GetName());
	}

	TEST(Adopt)
	{
		auto parent = std::make_shared<Entity>();
		auto child = std::make_shared<Entity>();

		REQUIRE(!parent->Parent());
		REQUIRE(!child->Parent());

		child = parent->Adopt("child", child);

		REQUIRE(!parent->Parent());
		REQUIRE(child->Parent());
		REQUIRE(parent == child->Parent());
		REQUIRE("child" == child->GetName());

		const auto child2 = parent->Adopt("child", child);

		REQUIRE(child2 == child);
		REQUIRE("child" == child2->GetName());

		auto newParent = std::make_shared<Entity>();
		const auto child3 = newParent->Adopt("child", child2);

		REQUIRE(newParent == child3->Parent());
		REQUIRE("child" == child3->GetName());

		REQUIRE_THROWS_AS(newParent->CreateChild<>("child"s), Entity::InvalidNameException);
	}

	TEST(Grandchildren)
	{
		auto parent = std::make_shared<Entity>();
		parent->CreateChild<>("child"s)->CreateChild<>("grandChild"s)->CreateChild<>("greatGrandChild"s);
	}
#pragma endregion
		
#pragma region Remove
	TEST(Orphan)
	{
		auto p = Engine::World().CreateChild<>();			
		auto c = p->CreateChild<>();
		REQUIRE(c->Parent());
		c->Orphan();
		Engine::Update();
		REQUIRE(!c->Parent());
		p->Orphan();
	}

	TEST(Remove)
	{
		auto s = std::make_shared<Entity>();
		const auto child = s->CreateChild<>("child"s);
		
		s->RemoveChild("child");
		REQUIRE(!child->Parent());
		REQUIRE(!s->HasChildren());
	}
#pragma endregion

	TEST(Init)
	{
		//Engine::IsActive() = false;
		//Engine::Main({});
	}

	TEST(Update)
	{
		//Coroutines::Start([]()->Coroutine { Engine::IsActive() = false; co_return; });
		//Engine::Main({});
	}

#pragma region RTTI
	TEST(StaticTypeID)
	{
		REQUIRE(Entity::typeID != RTTI::typeID);
	}

	TEST(VirtualTypeID)
	{
		REQUIRE(Entity().TypeID() == Entity().TypeID());
		REQUIRE(Entity().TypeID() != Foo().TypeID());
	}

	TEST(Is)
	{
		REQUIRE(Entity().Is<Attributed>());
		REQUIRE(!Entity().Is<Foo>());
	}

	TEST(As)
	{
		REQUIRE(!Entity().As<Foo>());
		REQUIRE(Entity().As<Attributed>());
	}
#pragma endregion
}
