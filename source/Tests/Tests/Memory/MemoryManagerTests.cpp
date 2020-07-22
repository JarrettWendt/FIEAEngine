// forcibly allow us to see Entity::Init and Entity::Update
#include "Attributed.h"
#define protected public
#include "Entity.h"
#include "MemLeakUtils.h"
#undef protected

#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;

#define TEST(name) TEST_CASE("Memory::Manager::" #name, "[Memory::Manager]")

namespace UnitTests
{
	using SharedString = SharedPtr<std::string>;
	using SharedEntity = SharedPtr<Entity>;
	
//	TEST(std::string)
//	{
//		std::string* addr;
//		{
//			auto a = SharedString::Make("hello");
//			REQUIRE(*a == "hello");
//			addr = &*a;
//		}
//		Memory::Manager::Defrag();
//
//#ifdef _DEBUG
//		REQUIRE(*reinterpret_cast<std::uint16_t*>(addr) == std::uint16_t(0xF1EA));
//#endif
//		
//		{
//			auto a = SharedString::Make("hello");
//			REQUIRE(*a == "hello");
//			REQUIRE(&*a == addr);
//		}
//		Memory::Manager::Defrag();
//	}
//
//	TEST(virtual)
//	{
//		Entity* addr;
//		{
//			auto a = SharedEntity::Make();
//			a->Init();
//			addr = &*a;
//		}
//		Memory::Manager::Defrag();
//
//#ifdef _DEBUG
//		REQUIRE(*reinterpret_cast<std::uint16_t*>(addr) == std::uint16_t(0xF1EA));
//#endif
//
//		{
//			auto a = SharedEntity::Make();
//			a->Init();
//			REQUIRE(&*a == addr);
//		}
//		Memory::Manager::Defrag();
//	}
//
//	TEST(World)
//	{
//		auto world = SharedEntity::Make();
//		world->SetName("World");
//		world->Init();
//
//		world = SharedEntity::Make();
//		world->SetName("World");
//		world->Init();
//	}
//
//	TEST(Init)
//	{
//		Engine::Init();
//		Coroutines::StopAll();
//		Engine::Update();
//	
//		Coroutines::StopAll();
//		Engine::Update();
//		
//		Engine::Init();
//		Coroutines::StopAll();
//		Engine::Update();
//	}

	TEST(MemLeak)
	{
		MemLeak();
		MemLeak();
		MemLeak();
		MemLeak();
		// the 5th one has a strange bug
		MemLeak();
	}
}
