// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "Macros.h"

namespace Library
{
	class Entity;
	
	class Engine final
	{
		/** the parentmost Entity */
		static inline std::shared_ptr<Entity> world{ nullptr };

	public:
		STATIC_CLASS(Engine)

		/**
		 * O(1)
		 * 
		 * @returns		the parentmost Entity
		 */
		static Entity& World() noexcept;
		
		/**
		 * to be called by WinMain()
		 */
		static void Main();

		/**
		 * setting this to false will terminate the simulation
		 *
		 * @returns		whether or not the simulation is active
		 */
		static bool& IsActive() noexcept;

		/**
		 * runs before the first Update()
		 */
		static void Init();

		/**
		 * the main engine loop
		 */
		static void Update();

		/**
		 * runs after the last Update()
		 */
		static void Terminate();

	private:
		/**
		 * Invoked by WinMain's WndProc.
		 *
		 * @param hWnd		HWND
		 * @param uMsg		UINT
		 * @param wParam	WPARAM
		 * @param lParam	LPARAM
		 */
#ifdef _WIN64
		static long long __stdcall WndProc(void* hWnd, unsigned int uMsg, unsigned long long wParam, long long lParam);
#else
		static long __stdcall WndProc(void* hWnd, unsigned int uMsg, unsigned wParam, long lParam);
#endif
	};
}
