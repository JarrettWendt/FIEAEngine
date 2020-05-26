// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"

using namespace std::string_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(InputTests)
	{
		INITIALIZE_CLEANUP

		using KeyCode = Input::KeyCode;
		using KeyState = Input::KeyState;

		TEST_METHOD(GetKey)
		{
			Assert::AreEqual(KeyCode::None, Input::GetKey());
		}

		TEST_METHOD(KeyDown)
		{
			Assert::IsFalse(Input::KeyDown(KeyCode::A));
		}

		TEST_METHOD(KeyUp)
		{
			Assert::IsTrue(Input::KeyUp(KeyCode::A));
		}

		TEST_METHOD(KeyPressed)
		{
			Assert::IsFalse(Input::KeyPressed(KeyCode::A));
		}

		TEST_METHOD(KeyReleased)
		{
			Assert::IsFalse(Input::KeyReleased(KeyCode::A));
		}

		TEST_METHOD(StateOf)
		{
			Assert::AreEqual(KeyState::Up, Input::StateOf(KeyCode::A));
		}

		TEST_METHOD(MapString)
		{
			Input::Map("Jump", KeyCode::Space);
			Input::Map("Forward", KeyCode::W);
			Input::Map("Left", KeyCode::A);
			Input::Map("Down", KeyCode::S);
			Input::Map("Right", KeyCode::D);

			Assert::AreEqual(KeyCode::Space, Input::KeyOf("Jump"));
			Assert::AreEqual(KeyCode::None, Input::KeyOf("bleh"));
			Assert::IsTrue(Input::KeyUp("Jump"));
			Input::UnMap("Jump");
			Assert::ExpectException<std::exception>([] { Input::KeyDown("Jump"); });

			Assert::AreEqual(KeyState::Up, Input::StateOf("Left"));
			Assert::IsTrue(Input::KeyUp("Left"));
			Assert::IsFalse(Input::KeyDown("Left"));
			Assert::IsFalse(Input::KeyPressed("Left"));
			Assert::IsFalse(Input::KeyReleased("Left"));

			Input::UnMapAll();
		}

		TEST_METHOD(KeyStateFromString)
		{
			Assert::ExpectException<std::invalid_argument>([] { Enum<KeyState>::FromString(""); });
		}

		TEST_METHOD(DefaultMappings)
		{
			Assert::AreEqual(KeyState::Up, Input::StateOf("A"));
		}
	};
}
