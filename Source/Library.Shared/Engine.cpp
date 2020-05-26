// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Engine.h"

#include "Coroutine.h"
#include "EngineTime.h"
#include "Input.h"

#include <Windows.h>

namespace Library
{
	void Engine::Main()
	{
		Start();
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
	
	void Engine::Start()
	{
		// TODO
	}

	void Engine::Update()
	{
		Time::Update();
		Input::Update();
		Coroutines::Update();
	}
	
	void Engine::Terminate()
	{
		// TODO
	}
	
	LRESULT CALLBACK Engine::WndProc([[maybe_unused]] void* hWnd, const UINT uMsg, const WPARAM wParam, [[maybe_unused]] const LPARAM lParam)
	{
		Input::WndProc(uMsg, wParam);
		return 0;
	}
}
