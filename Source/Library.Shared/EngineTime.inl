// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "EngineTime.h"

namespace Library
{
	inline constexpr Time::Seconds Time::DeltaTime() noexcept
	{
		using namespace std::chrono;
		return duration_cast<duration<float>>(deltaTime);
	}

	inline constexpr Time::Seconds Time::TotalTime() noexcept
	{
		using namespace std::chrono;
		return duration_cast<duration<float>>(totalTime);
	}

	inline constexpr Time::Point Time::CurrentTime() noexcept
	{
		return currentTime;
	}
}
