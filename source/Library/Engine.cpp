#include "Engine.h"
// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "python/pch.h"
#include "Engine.h"

#include "Coroutine.h"
#include "Entity.h"
#include "EngineTime.h"
#include "Input.h"

#ifdef _WIN32
#include <Windows.h>
#endif
#include "python/modules/FIEAEngine.h"
#include <filesystem>

namespace Library
{
	Entity& Engine::World() noexcept
	{
		return *world;
	}
	
	void Engine::Main(const Args& args)
	{
		ParseArgs(args);
		Init();
		
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
	
	void Engine::Init()
	{
		world = std::make_shared<Entity>();
		world->SetName("World");
		world->Init();

		PyImport_AppendInittab("FIEAEngine", &PyInit_FIEAEngine);
		
		Py_Initialize();

#ifdef _DEBUG
		const std::string bleh = std::filesystem::current_path().string();
		const std::ifstream file{ pythonSourceDirectory + initFileName };
		const std::string str = (std::stringstream{} << file.rdbuf()).str();
		PyRun_SimpleString(str.c_str());
#else
		// TODO: This is the way running a python file is _supposed_ to work.
		FILE* f;
		fopen_s(&f, (pythonSourceDirectory + initFileName).c_str(), "r");
		PyRun_SimpleFile(f, initFileName.c_str());
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

	void Engine::ParseArgs(const Args& args)
	{
		if (!args.empty())
		{
			const auto wProgramName = Py_DecodeLocale(args[0], nullptr);
			programName = std::to_string(wProgramName);
			Py_SetProgramName(wProgramName);
		}
		pythonSourceDirectory = args.size() > 1 ? args[1] : "";
	}

#ifdef _WIN32
	LRESULT CALLBACK Engine::WndProc([[maybe_unused]] void* hWnd, const UINT uMsg, const WPARAM wParam, [[maybe_unused]] const LPARAM lParam)
	{
		Input::WndProc(uMsg, wParam);
		return 0;
	}
#else
#endif
}
