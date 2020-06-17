// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "PythonPCH.h"
#include "Engine.h"

#include "Coroutine.h"
#include "Entity.h"
#include "EngineTime.h"
#include "Input.h"

#include <Windows.h>
#include "FIEAEngine_module.h"

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
		world->SetName("World");
		world->Init();

		if (!args.empty())
		{
			const auto wProgramName = Py_DecodeLocale(args[0], nullptr);
			programName = std::to_string(wProgramName);
			Py_SetProgramName(wProgramName);
		}

		PyImport_AppendInittab("FIEAEngine", &PyInit_FIEAEngine);
		
		Py_Initialize();

#ifdef _DEBUG
		const std::ifstream file{ "Init.py" };
		const std::string str = (std::stringstream{} << file.rdbuf()).str();
		PyRun_SimpleString(str.c_str());
#else
		// TODO: This is the way running a python file is _supposed_ to work.
		FILE* f;
		fopen_s(&f, "Init.py", "r");
		PyRun_SimpleFile(f, "Init.py");
#endif
	}

	void Engine::Update()
	{
		for (const auto& o : pendingOrphans)
		{
			if (const auto shared = o.lock())
			{
				shared->OrphanNow();
			}
		}
		pendingOrphans.Clear();
		
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
