#include "pch.h"

#include "Input.h"
#include "Util.h"

#pragma warning(push)
#pragma warning(disable: 4005)		// Macro redefinition.
#include <Windows.h>
#pragma warning(pop)

namespace Library
{
	using KeyCode = Input::KeyCode;
	using KeyState = Input::KeyState;

	// This map has three purposes:
	// - Stringifying Enums.
	// - It's inversion is used for enumerating strings.
	// - It's a special static mapping so users can do Input::GetKey("Space").
	static const HashMap<std::string, KeyCode> stringToKey
	{
		{ "NONE", KeyCode::None },
		{ "A", KeyCode::A },
		{ "B", KeyCode::B },
		{ "C", KeyCode::C },
		{ "D", KeyCode::D },
		{ "E", KeyCode::E },
		{ "F", KeyCode::F },
		{ "G", KeyCode::G },
		{ "H", KeyCode::H },
		{ "I", KeyCode::I },
		{ "J", KeyCode::J },
		{ "K", KeyCode::K },
		{ "L", KeyCode::L },
		{ "M", KeyCode::M },
		{ "N", KeyCode::N },
		{ "O", KeyCode::O },
		{ "P", KeyCode::P },
		{ "Q", KeyCode::Q },
		{ "R", KeyCode::R },
		{ "S", KeyCode::S },
		{ "T", KeyCode::T },
		{ "U", KeyCode::U },
		{ "V", KeyCode::V },
		{ "W", KeyCode::W },
		{ "X", KeyCode::X },
		{ "Y", KeyCode::Y },
		{ "Z", KeyCode::Z },
		{ "Space", KeyCode::Space }
	};

	// TODO: This can be an array. That would be much more efficient, but much less readable.
	static const HashMap<KeyCode, std::string> keyToString = stringToKey.Invert();

	void Input::WndProc(const unsigned int uMsg, const unsigned long long wParam)
	{
		switch (uMsg)
		{
		case WM_KEYDOWN:
			// Windows can send repeated keydown events for repeat-keys.
			if (keys[wParam].state != KeyState::Down)
			{
				keys[wParam].state = KeyState::Pressed;
				keys[wParam].seen = false;
				keysHeld.PushBack(static_cast<KeyCode>(wParam));
			}
			break;

		case WM_KEYUP:
			keys[wParam].state = KeyState::Released;
			keys[wParam].seen = false;
			keysHeld.Remove(static_cast<KeyCode>(wParam));
			break;

		case WM_KILLFOCUS:
			// When we loose focus on the window, go through all down keys and release them.
			for (size_t i = 0; i < NUM_KEYS; i++)
			{
				if (keys[i].state == KeyState::Down)
				{
					keys[i].state = KeyState::Released;
					keys[i].seen = false;
					keysHeld.Remove(static_cast<KeyCode>(i));
				}
			}
			break;

		default:
			break;
		}
	}

	void Input::Update()
	{
		// Use the seen flag to ensure that keys remain in the transition states for 1 and only 1 frame.
		for (size_t i = 0; i < NUM_KEYS; i++)
		{
			auto& key = keys[i];
			if (key.seen)
			{
				switch (key.state)
				{
				case KeyState::Pressed:
					key.state = KeyState::Down;
					break;
				case KeyState::Released:
					key.state = KeyState::Up;
					break;
				default:;
				}
			}
			else
			{
				key.seen = true;
			}

			EventManager::Event({ KeyCode(i), key.state }).Invoke();
		}
	}

#pragma region Key
	KeyCode Input::GetKey() noexcept
	{
		return keysHeld.IsEmpty() ? KeyCode::None : keysHeld.Back();
	}

	KeyState Input::StateOf(const KeyCode keyCode) noexcept
	{
		return keys[static_cast<size_t>(keyCode)].state;
	}

	bool Input::KeyDown(const KeyCode keyCode) noexcept
	{
		return StateOf(keyCode) == KeyState::Down;
	}

	bool Input::KeyPressed(const KeyCode keyCode) noexcept
	{
		return StateOf(keyCode) == KeyState::Pressed;
	}

	bool Input::KeyReleased(const KeyCode keyCode) noexcept
	{
		return StateOf(keyCode) == KeyState::Released;
	}

	bool Input::KeyUp(const KeyCode keyCode) noexcept
	{
		return StateOf(keyCode) == KeyState::Up;
	}
#pragma endregion

#pragma region String
	void Input::Map(const std::string& str, const KeyCode keyCode) noexcept
	{
		if (!stringToKey.Contains(str))
		{
			mappings.Insert({ str, keyCode });
		}
	}

	void Input::UnMap(const std::string& str) noexcept
	{
		mappings.Remove(str);
	}

	void Input::UnMapAll() noexcept
	{
		mappings.Clear();
		mappings.Resize(1);
	}

	KeyCode Input::KeyOf(const std::string& str) noexcept
	{
		const auto it = Find(str);
		return it ? it->value : KeyCode::None;
	}

	KeyState Input::StateOf(const std::string& str)
	{
		if (const auto it = Find(str))
		{
			return StateOf(it->value);
		}
		throw std::invalid_argument("no key mapping found to " + str);
	}

	bool Input::KeyDown(const std::string& str)
	{
		return StateOf(str) == KeyState::Down;
	}

	bool Input::KeyPressed(const std::string& str)
	{
		return StateOf(str) == KeyState::Pressed;
	}

	bool Input::KeyReleased(const std::string& str)
	{
		return StateOf(str) == KeyState::Released;
	}

	bool Input::KeyUp(const std::string& str)
	{
		return StateOf(str) == KeyState::Up;
	}
#pragma endregion
	
	HashMap<std::string, KeyCode>::const_iterator Input::Find(const std::string& str) noexcept
	{
		// TODO: Add a HashMap::Find(hash_t) method so we can avoid hashing twice.
		const auto it = stringToKey.Find(str);
		return it ? it : mappings.Find(str);
	}

	const std::string& Enum<Input::KeyCode>::ToString(const KeyCode t)
	{
		return keyToString.At(t);
	}

	KeyCode Enum<Input::KeyCode>::FromString(const std::string& str)
	{
		const auto it = stringToKey.Find(Util::RemoveWhitespace(str));
		return it ? it->value : KeyCode::None;
	}

	const std::string& Enum<KeyState>::ToString(const KeyState t)
	{
		const static std::string strings[]{ "Up", "Down", "Pressed", "Released" };
		return strings[static_cast<size_t>(t)];
	}

	KeyState Enum<KeyState>::FromString(const std::string& str)
	{
		const static HashMap<std::string, KeyState> states
		{
			{ "up", KeyState::Up },
			{ "down", KeyState::Down },
			{ "pressed", KeyState::Pressed },
			{ "released", KeyState::Released },
		};
		if (const auto it = states.Find(Util::ToLower(Util::RemoveWhitespace(str))))
		{
			return it->value;
		}
		throw std::invalid_argument("No KeyState by the name " + str);
	}
}
