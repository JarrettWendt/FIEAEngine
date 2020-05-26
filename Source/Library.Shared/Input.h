// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "Enum.h"
#include "Event.h"
#include "EventManager.h"
#include "HashMap.h"

namespace Library
{
	class Input final
	{
		friend class Engine;
	public:
		STATIC_CLASS(Input)
		
		/**
		 * Keys on the keyboard.
		 * These magic numbers come from WinUser.h
		 */
		enum class KeyCode : uint8_t
		{
			None,

			A = 'A',
			B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

			Alpha0 = '0',
			Alpha1, Alpha2, Alpha3, Alpha4, Alpha5, Alpha6, Alpha7, Alpha8, Alpha9,

			Keypad0 = 0x60,
			Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9,

			KeypadMultiply, KeypadPlus,
			KeypadMinus = 0x6D,
			KeypadPeriod, KeypadDivide,

			F1 = 0x70,
			F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,

			NumLock = 0x90,
			ScrollLock,

			LeftShift = 0xA0,
			RightShift,
			LeftControl,
			RightControl,
			LeftMenu,
			RightMenu,

			Escape = 0x1B,

			Space = 0x20,

			End = 0x23,
			Home,

			LeftArrow = 0x26,
			UpArrow, RightArrow, DownArrow,

			Print = 0x2A,
			Insert = 0x2D,
			Delete,

			LeftMeta = 0x5B,
			LeftCommand = LeftMeta,
			LeftWindows = LeftMeta,
			RightMeta = 0x5C,
			RightCommand = RightMeta,
			RightWindows = RightMeta
		};

		/**
		 * In a given frame, a key can be in one of four states:
		 * up, down, and the transition between those two.
		 */
		enum class KeyState : uint8_t
		{
			// Passively up.
			Up,
			// Held down.
			Down,
			// Just pressed.
			Pressed,
			// Just released.
			Released
		};

		/** it is suggested to use this EventManager for storing Input Events */
		using EventManager = Library::EventManager<std::pair<KeyCode, KeyState>>;

	private:
		/** how many keys Windows supports */
		static constexpr size_t NUM_KEYS = 256;

		/** array of keys to keep track of their states */
		static inline struct
		{
			// The key's current state.
			KeyState state = KeyState::Up;
			// Whether or not Update() has already seen this key in its current state.
			bool seen = false;
		} keys[NUM_KEYS]{};

		/** the "stack" of keys currently held */
		static inline Array<KeyCode> keysHeld{};
		/** user-friendly mappings of strings to keys */
		static inline HashMap<std::string, KeyCode> mappings{};

		/**
		 * to be invoked by Engine::WinProc()
		 * 
		 * @param uMsg		either a WM_KEYDOWN or WM_KEYUP event
		 * @param wParam	what key was pressesd
		 */
		static void WndProc(unsigned int uMsg, unsigned long long wParam);

		/**
		 * to be invoked by Engine::Update()
		 */
		static void Update();

	public:
#pragma region Key
		/**
		 * O(1)
		 * 
		 * @returns		the most recently pressed key, or Key::None if no keys are down.
		 */
		static KeyCode GetKey() noexcept;

		/**
		 * O(1)
		 * 
		 * @param keyCode	the key to look up
		 * @returns			the current state of the key
		 */
		static KeyState StateOf(KeyCode keyCode) noexcept;

		/**
		 * O(1)
		 * 
		 * @param keyCode	the key to look up
		 * @returns			if the key is held down during this frame
		 */
		static bool KeyDown(KeyCode keyCode) noexcept;

		/**
		 * O(1)
		 *
		 * @param keyCode	the key to look up
		 * @returns			if the key was pressed this frame
		 */
		static bool KeyPressed(KeyCode keyCode) noexcept;

		/**
		 * O(1)
		 * 
		 * @param keyCode	the key to look up
		 * @returns			if the key was released this frame 
		 */
		static bool KeyReleased(KeyCode keyCode) noexcept;

		/**
		 * O(1)
		 * 
		 * @param keyCode	the key to look up
		 * @returns			if the key is up during this frame
		 */
		static bool KeyUp(KeyCode keyCode) noexcept;
#pragma endregion

#pragma region String
		/**
		 * Maps a string to a key.
		 * If the stringis already mapped, it will be remapped to the new key.
		 * 
		 * @param str		the string to map
		 * @param keyCode	key this string mapps to 
		 */
		static void Map(const std::string& str, KeyCode keyCode) noexcept;

		/**
		 * Removes the mapping of the given string to whatever key it may be mapped to.
		 * Does nothing if the string is not mapped to anything.
		 * 
		 * @param str	key mapping to remove
		 */
		static void UnMap(const std::string& str) noexcept;

		/**
		 * Removes all string mappings. 
		 */
		static void UnMapAll() noexcept;

		/**
		 * O(1)
		 * 
		 * @param str	key mapping to look up
		 * @returns		the key this string is mapped to, or KeyCode::None if not mapped at all
		 */
		static KeyCode KeyOf(const std::string& str) noexcept;

		/**
		 * O(1)
		 *
		 * @param str	key mapping to look up
		 * @returns		the current state of the key
		 *
		 * @throws std::invalid_argument	if no mapping exists for the passed string
		 */
		static KeyState StateOf(const std::string& str);

		/**
		 * O(1)
		 * 
		 * @param str	key mapping to look up
		 * @returns		if the key is down during this frame
		 *
		 * @throws std::invalid_argument	if no mapping exists for the passed string
		 */
		static bool KeyDown(const std::string& str);

		/**
		 * O(1)
		 * 
		 * @param str	key mapping to look up
		 * @returns		if the key was pressed during this frame
		 *
		 * @throws std::invalid_argument	if no mapping exists for the passed string
		 */
		static bool KeyPressed(const std::string& str);

		/**
		 * O(1)
		 * 
		 * @param str	key mapping to look up
		 * @returns		if the key was released during this frame
		 *
		 * @throws std::invalid_argument	if no mapping exists for the passed string
		 */
		static bool KeyReleased(const std::string& str);

		/**
		 * O(1)
		 * 
		 * @param str	key mapping to look up
		 * @returns		if the key is up during this frame
		 *
		 * @throws std::invalid_argument	if no mapping exists for the passed string
		 */
		static bool KeyUp(const std::string& str);
#pragma endregion

	private:
		/**
		 * Looks through several HashMaps and returns an iterator to one of them, or end
		 *
		 * @param str	key mapping to look up
		 * @return		iterator at that key, or end
		 */
		static HashMap<std::string, KeyCode>::const_iterator Find(const std::string& str) noexcept;
	};

	/**
	 * O(1)
	 *
	 * @param t		the KeyCode to stringify
	 * @returns		the string representation of this KeyCode
	 */
	template<>
	const std::string& Enum<Input::KeyCode>::ToString(Input::KeyCode t);

	/**
	 * O(1)
	 *
	 * @param str	the string to turn into a KeyCode
	 * @returns		the KeyCode that string represents, or KeyCode::None if the string could not be parsed
	 */
	template<>
	Input::KeyCode Enum<Input::KeyCode>::FromString(const std::string& str);

	/**
	 * O(1)
	 *
	 * @param t		the KeyState to stringify
	 * @returns		the string representation of this KeyState
	 */
	template<>
	const std::string& Enum<Input::KeyState>::ToString(Input::KeyState t);

	/**
	 * O(1)
	 *
	 * @param str	the string to turn into a KeyState
	 * @returns		the KeyState this string represents
	 *
	 * @throws std::invalid_argument	if the string could not be parsed
	 */
	template<>
	Input::KeyState Enum<Input::KeyState>::FromString(const std::string& str);
}
