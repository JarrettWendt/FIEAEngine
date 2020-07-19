// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include <gsl/gsl>

#include "Macros.h"
#include "SList.h"
#include "SharedPtr.h"
#include "WeakPtr.h"

namespace Library
{
	class Entity;
	
	class Engine final
	{
		friend Entity;
		
		/** the parentmost Entity */
		static inline SharedPtr<Entity> world{ nullptr };
		
		static inline std::string programName{};
		static inline std::string pythonSourceDirectory{};
		static inline const std::string initFileName{ "init.py" };
		static inline const std::string shutdownFileName{ "del.py" };

		static inline FILE* initFilePtr{ nullptr };
		
	public:
		STATIC_CLASS(Engine)
		using Args = gsl::span<const char*>;
		
		/**
		 * O(1)
		 * 
		 * @returns		the parentmost Entity
		 */
		static Entity& World() noexcept;
		
		/**
		 * to be called by WinMain()
		 */
		static void Main(const Args& args);

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
		 * wraps all of the logic for parsing the command line args.
		 */
		static void ParseArgs(const Args& args);
		
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
