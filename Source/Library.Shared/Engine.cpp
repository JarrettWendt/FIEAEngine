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

		PyRun_SimpleString(R"(
from random import randint
from asciimatics.screen import Screen

def demo(screen):
    while True:
        screen.print_at('Hello world!',
                        randint(0, screen.width), randint(0, screen.height),
                        colour=randint(0, screen.colours - 1),
                        bg=randint(0, screen.colours - 1))
        ev = screen.get_key()
        if ev in (ord('Q'), ord('q')):
            return
        screen.refresh()

Screen.wrapper(demo)
)");
		
		//FILE* f;
		//fopen_s(&f, "Init.py", "r");
		//PyRun_SimpleFile(f, "Init.py");
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
