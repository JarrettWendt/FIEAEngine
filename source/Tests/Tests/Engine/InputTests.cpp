#include "../../pch.h"

using namespace std::string_literals;
using namespace Library;

#define TEST(name) TEST_CASE_METHOD(MemLeak, #name, "[Input]")

namespace UnitTests
{
	using KeyCode = Input::KeyCode;
	using KeyState = Input::KeyState;
	
	TEST(GetKey)
	{
		REQUIRE(KeyCode::None == Input::GetKey());
	}

	TEST(KeyDown)
	{
		REQUIRE(!Input::KeyDown(KeyCode::A));
	}

	TEST(KeyUp)
	{
		REQUIRE(Input::KeyUp(KeyCode::A));
	}

	TEST(KeyPressed)
	{
		REQUIRE(!Input::KeyPressed(KeyCode::A));
	}

	TEST(KeyReleased)
	{
		REQUIRE(!Input::KeyReleased(KeyCode::A));
	}

	TEST(StateOf)
	{
		REQUIRE(KeyState::Up == Input::StateOf(KeyCode::A));
	}

	TEST(MapString)
	{
		Input::Map("Jump", KeyCode::Space);
		Input::Map("Forward", KeyCode::W);
		Input::Map("Left", KeyCode::A);
		Input::Map("Down", KeyCode::S);
		Input::Map("Right", KeyCode::D);

		REQUIRE(KeyCode::Space == Input::KeyOf("Jump"));
		REQUIRE(KeyCode::None == Input::KeyOf("bleh"));
		REQUIRE(Input::KeyUp("Jump"));
		Input::UnMap("Jump");
		REQUIRE_THROWS(Input::KeyDown("Jump"));

		REQUIRE(KeyState::Up == Input::StateOf("Left"));
		REQUIRE(Input::KeyUp("Left"));
		REQUIRE(!Input::KeyDown("Left"));
		REQUIRE(!Input::KeyPressed("Left"));
		REQUIRE(!Input::KeyReleased("Left"));

		Input::UnMapAll();
	}

	TEST(KeyStateFromString)
	{
		REQUIRE_THROWS_AS(Enum<KeyState>::FromString(""), std::invalid_argument);
	}

	TEST(DefaultMappings)
	{
		REQUIRE(KeyState::Up == Input::StateOf("A"));
	}
}
