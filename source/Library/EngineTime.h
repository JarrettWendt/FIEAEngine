// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include <chrono>

#include "Macros.h"

namespace Library
{	
	class Time final
	{
		friend class Engine;
		
	public:
		STATIC_CLASS(Time)
		
		// not std::chrono::high_resolution_clock because `Point = Point + Seconds` wouldn't work.
		using Point = std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::duration<float, std::nano>>;
		/**
		 * See namespace Library::Literals to be able to type 500ms to create 500 Millis. 
		 */
		using Millis = std::chrono::milliseconds;
		/**
		 * Not std::chrono::seconds because that uses integers.
		 * Use .count() to get a float out of one of these.
		 * See namespace Library::Literals to be able to type 0.5s to create half a Second.
		 */
		using Seconds = std::chrono::duration<float>;

	private:
		/** the time the simulation began or was last reset */
		static inline Point startTime{ std::chrono::high_resolution_clock::now() };
		/** the time of this frame */
		static inline Point currentTime{ startTime };
		/** the time of the last frame */
		static inline Point lastTime{ startTime };

		/** milliseconds since startTime */
		static inline Millis totalTime{ 0 };
		/** milliseconds since lastTime */
		static inline Millis deltaTime{ 0 };
		
	public:
		/**
		 * O(1)
		 * 
		 * @returns		time in seconds since the last frame
		 */
		static constexpr Seconds DeltaTime() noexcept;

		/**
		 * O(1)
		 * 
		 * @returns		time in seconds since the simulation began
		 */
		static constexpr Seconds TotalTime() noexcept;

		/**
		 * O(1)
		 * 
		 * @returns		the current point in time
		 */
		static constexpr Point CurrentTime() noexcept;

		/**
		 * resets the simulation epoch
		 */
		static void Reset() noexcept;
		
	private:
		/**
		 * To be called by Engine::Update().
		 * Ticks the internal clock.
		 */
		static void Update() noexcept;
	};
}

#include "EngineTime.inl"
