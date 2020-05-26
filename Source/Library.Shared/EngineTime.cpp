#include "pch.h"
#include "EngineTime.h"

using namespace std::chrono;

namespace Library
{
	void Time::Reset() noexcept
	{
		startTime = lastTime = currentTime = high_resolution_clock::now();
		totalTime = deltaTime = Millis{ 0 };
	}
	
	void Time::Update() noexcept
	{
		lastTime = currentTime;
		currentTime = high_resolution_clock::now();
		totalTime = duration_cast<Millis>(currentTime - startTime);
		deltaTime = duration_cast<Millis>(currentTime - lastTime);
	}
}
