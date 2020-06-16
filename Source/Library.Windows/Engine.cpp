// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Engine.h"

#include "Coroutine.h"
#include "Entity.h"
#include "EngineTime.h"
#include "Input.h"

#include <Python.h>
#include <Windows.h>
#include "../Library.Python/FIEAEngine_module.h"

namespace Library
{
	Entity& Engine::World() noexcept
	{
		return *world;
	}
	
	void Engine::Main(const Args& args)
	{
		Init(args);
		
		while (IsActive())
		{
			Update();
		}
		Terminate();
	}
	
	bool& Engine::IsActive() noexcept
	{
		static bool ret = true;
		return ret;
	}
	
	void Engine::Init(const Args& args)
	{
		world = std::make_shared<Entity>();
		world->Init();

		const auto wProgramName = Py_DecodeLocale(args[0], nullptr);
		programName = std::to_string(wProgramName);
		Py_SetProgramName(wProgramName);

		//PyImport_AppendInittab("FIEAEngine", &PyInit_FIEAEngine);
		
		Py_Initialize();
		
		FILE* f;
		fopen_s(&f, "Init.py", "r");
		PyRun_SimpleFile(f, "Init.py");
	}

	void Engine::Update()
	{
		Time::Update();
		Input::Update();
		Coroutines::Update();
		world->Update();
	}
	
	void Engine::Terminate()
	{
		world = nullptr;
		if (Py_FinalizeEx() < 0)
		{
			std::terminate();
		}
	}
	
	LRESULT CALLBACK Engine::WndProc([[maybe_unused]] void* hWnd, const UINT uMsg, const WPARAM wParam, [[maybe_unused]] const LPARAM lParam)
	{
		Input::WndProc(uMsg, wParam);
		return 0;
	}
}
