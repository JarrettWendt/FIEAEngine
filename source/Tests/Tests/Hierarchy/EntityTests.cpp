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

#pragma region wrappers
#pragma region FloatWrapper
	TEST(FloatWrapper)
	{
		const auto e = std::make_shared<Entity>();
		{
			auto w = e->Transform<CoordinateSpace::Local>().Translation();
			const auto t = e->GetTransform<CoordinateSpace::Local>().translation;
			REQUIRE(w.X() == t.x);
			REQUIRE(w.Y() == t.y);
			REQUIRE(w.Z() == t.z);
		}
		{
			auto w = e->Transform<CoordinateSpace::Local>().Rotation();
			const auto r = e->GetTransform<CoordinateSpace::Local>().rotation;
			REQUIRE(w.X() == r.x);
			REQUIRE(w.Y() == r.y);
			REQUIRE(w.Z() == r.z);
			REQUIRE(w.W() == r.w);
		}
	}

	TEST(FloatWrapper::operator=)
	{
		const auto e = std::make_shared<Entity>();
		auto f = e->Transform<CoordinateSpace::World>().Translation().X();
		f = 1.f;
		REQUIRE(1.f == e->GetTransform<CoordinateSpace::Local>().translation.x);
	}

	TEST(FloatWrapper::operator+)
	{
		const auto e = std::make_shared<Entity>();
		auto f = e->Transform<CoordinateSpace::Local>().Translation().X();
		f = f + 1.f;
		REQUIRE(1.f == e->GetTransform<CoordinateSpace::Local>().translation.x);
	}

	TEST(FloatWrapper::operator-)
	{
		const auto e = std::make_shared<Entity>();
		auto f = e->Transform<CoordinateSpace::Local>().Translation().X() = 2.f;
		f = 1.f - f;
		REQUIRE(-1.f == e->GetTransform<CoordinateSpace::Local>().translation.x);
	}

	TEST(FloatWrapper::operator+=)
	{
		const auto e = std::make_shared<Entity>();
		auto f = e->Transform<CoordinateSpace::Local>().Translation().X();
		f += 1.f;
		REQUIRE(1.f == e->GetTransform<CoordinateSpace::Local>().translation.x);
	}
#pragma endregion	

	TEST(Vector3Wrapper::operator==)
	{
		const auto e = std::make_shared<Entity>();
		auto t = e->Transform<CoordinateSpace::Local>();
		REQUIRE(t == e->GetTransform<CoordinateSpace::Local>());
	}
	
	TEST(Vector3Wrapper::operator[])
	{
		const auto e = std::make_shared<Entity>();
		auto f = e->Transform<CoordinateSpace::Local>().Translation()[0_zc];
		f = 1.f;
		REQUIRE(1.f == e->GetTransform<CoordinateSpace::Local>().translation.x);
	}
	
	TEST(Vector3Wrapper::operator=)
	{
		const auto e = std::make_shared<Entity>();
		auto t = e->Transform<CoordinateSpace::Local>().Translation();
		const auto r = Random::Next<Vector3>();
		t = r;
		REQUIRE(r == e->GetTransform<CoordinateSpace::Local>().translation);
	}
	
	TEST(QuaternionWrapper::operator=)
	{
		const auto e = std::make_shared<Entity>();
		auto t = e->Transform<CoordinateSpace::Local>().Rotation();
		const auto r = Random::Next<Quaternion>();
		t = r;
		REQUIRE(r == e->GetTransform<CoordinateSpace::Local>().rotation);
	}
	
	TEST(ScaleWrapper)
	{
		const auto e = std::make_shared<Entity>();
		const Vector3 v = e->Transform<CoordinateSpace::Local>().Scale();
		REQUIRE(v == e->GetTransform<CoordinateSpace::Local>().scale);
	}
	
	TEST(TranslationWrapper)
	{
		const auto e = std::make_shared<Entity>();
		const Vector3 v = e->Transform<CoordinateSpace::Local>().Translation();
		REQUIRE(v == e->GetTransform<CoordinateSpace::Local>().translation);
	}
	
	TEST(QuaternionWrapper)
	{
		const auto e = std::make_shared<Entity>();
		const Quaternion q = e->Transform<CoordinateSpace::Local>().Rotation();
		REQUIRE(q == e->GetTransform<CoordinateSpace::Local>().rotation);
	}

	TEST(TransformWrapper)
	{
		const auto e = std::make_shared<Entity>();
		const Transform t = e->Transform<CoordinateSpace::Local>();
		REQUIRE(t == e->GetTransform<CoordinateSpace::Local>());
	}
#pragma endregion
	
#pragma region iterator
	TEST(ConstIterator)
	{
		const auto e = std::make_shared<Entity>();

		auto cit = e->cbegin();
		REQUIRE(cit == e->cend());
		
		REQUIRE(cit.IsAtBegin());
		REQUIRE(cit.IsAtEnd());

		REQUIRE_FALSE(cit);
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

		REQUIRE(p->GetTransform<CoordinateSpace::Local>() == p->GetTransform<CoordinateSpace::World>());
		REQUIRE(p->GetTransform<CoordinateSpace::World>() == c->GetTransform<CoordinateSpace::World>());

		auto t = p->GetTransform<CoordinateSpace::Local>();
		t.translation = { 0, 0, 1 };
		p->SetTransform<CoordinateSpace::Local>(t);

		REQUIRE(t == p->GetTransform<CoordinateSpace::Local>());
		REQUIRE(t == p->GetTransform<CoordinateSpace::World>());
		REQUIRE(t == c->GetTransform<CoordinateSpace::World>());
		REQUIRE(Transform{} == c->GetTransform<CoordinateSpace::Local>());
	}

	TEST(SetWorldTransformFromParent)
	{
		const auto p = std::make_shared<Entity>();
		const auto c = p->CreateChild<>();

		REQUIRE(p->GetTransform<CoordinateSpace::Local>() == p->GetTransform<CoordinateSpace::World>());
		REQUIRE(p->GetTransform<CoordinateSpace::World>() == c->GetTransform<CoordinateSpace::World>());

		auto t = p->GetTransform<CoordinateSpace::Local>();
		t.translation = { 0, 0, 1 };
		p->SetTransform<CoordinateSpace::World>(t);

		REQUIRE(t == p->GetTransform<CoordinateSpace::Local>());
		REQUIRE(t == p->GetTransform<CoordinateSpace::World>());
		REQUIRE(t == c->GetTransform<CoordinateSpace::World>());
		REQUIRE(Transform{} == c->GetTransform<CoordinateSpace::Local>());
	}
	
	TEST(SetLocalTransformFromChild)
	{
		const auto p = std::make_shared<Entity>();
		const auto c = p->CreateChild<>();

		REQUIRE(p->GetTransform<CoordinateSpace::Local>() == p->GetTransform<CoordinateSpace::World>());
		REQUIRE(p->GetTransform<CoordinateSpace::World>() == c->GetTransform<CoordinateSpace::World>());

		auto t = c->GetTransform<CoordinateSpace::Local>();
		t.translation = { 0, 0, 1 };
		c->SetTransform<CoordinateSpace::Local>(t);

		REQUIRE(Transform{} == p->GetTransform<CoordinateSpace::Local>());
		REQUIRE(Transform{} == p->GetTransform<CoordinateSpace::World>());
		REQUIRE(t == c->GetTransform<CoordinateSpace::World>());
		REQUIRE(t == c->GetTransform<CoordinateSpace::Local>());
	}

	TEST(SetWorldTransformFromChild)
	{
		const auto p = std::make_shared<Entity>();
		const auto c = p->CreateChild<>();

		REQUIRE(p->GetTransform<CoordinateSpace::Local>() == p->GetTransform<CoordinateSpace::World>());
		REQUIRE(p->GetTransform<CoordinateSpace::World>() == c->GetTransform<CoordinateSpace::World>());

		auto t = c->GetTransform<CoordinateSpace::Local>();
		t.translation = { 0, 0, 1 };
		c->SetTransform<CoordinateSpace::World>(t);

		REQUIRE(Transform{} == p->GetTransform<CoordinateSpace::Local>());
		REQUIRE(Transform{} == p->GetTransform<CoordinateSpace::World>());
		REQUIRE(t == c->GetTransform<CoordinateSpace::World>());
		REQUIRE(t == c->GetTransform<CoordinateSpace::Local>());
	}
#pragma endregion

	TEST(SetName)
	{
		const auto p = std::make_shared<Entity>();
		const auto c = p->CreateChild<>("child"_s);
		c->SetName("Seven"_s);
		REQUIRE("Seven"_s == c->GetName());
		REQUIRE(p->Child("Seven"_s));
	}

	TEST(GetName)
	{
		const auto e = std::make_shared<Entity>();
		REQUIRE("Entity"_s == e->GetName());
	}
		
#pragma region Insert
	TEST(Insert)
	{
		REQUIRE_THROWS_AS(Entity().CreateChild(""_s), Entity::InvalidNameException);
	}

	TEST(InsertScope)
	{
		auto parent = std::make_shared<Entity>();
		const auto child = parent->CreateChild("child"_s);

		REQUIRE(parent == child->Parent());
		REQUIRE("child"_s == child->GetName());
	}

	TEST(Adopt)
	{
		auto parent = std::make_shared<Entity>();
		auto child = std::make_shared<Entity>();

		REQUIRE(!parent->Parent());
		REQUIRE(!child->Parent());

		child->SetName("child"_s);
		child = parent->Adopt(child);

		REQUIRE(!parent->Parent());
		REQUIRE(child->Parent());
		REQUIRE(parent == child->Parent());
		REQUIRE("child"_s == child->GetName());

		const auto child2 = parent->Adopt("child"_s, child);

		REQUIRE(child2 == child);
		REQUIRE("child"_s == child2->GetName());

		auto newParent = std::make_shared<Entity>();
		const auto child3 = newParent->Adopt("child"_s, child2);

		REQUIRE(newParent == child3->Parent());
		REQUIRE("child"_s == child3->GetName());

		REQUIRE_THROWS_AS(newParent->CreateChild<>("child"_s), Entity::InvalidNameException);
	}

	TEST(Grandchildren)
	{
		auto parent = std::make_shared<Entity>();
		parent->CreateChild<>("child"_s)->CreateChild<>("grandChild"_s)->CreateChild<>("greatGrandChild"_s);
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
		const auto child = s->CreateChild<>("child"_s);
		
		s->RemoveChild("child"_s);
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

#pragma region operators
	TEST(operator==)
	{
		const auto a = std::make_shared<Entity>();
		const auto b = std::make_shared<Entity>();
		REQUIRE(*a == *b);
	}
	
	TEST(operator!=)
	{
		const auto a = std::make_shared<Entity>();
		const auto b = std::make_shared<Entity>();
		REQUIRE(!(*a != *b));
	}
#pragma endregion

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
