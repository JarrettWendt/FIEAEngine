// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "Event.h"

namespace Library
{
	/**
	 * A place for global events.
	 *
	 * @param <EventKey>	the type for keys to look up Events
	 * @param <Args>		the type for the arguments passed along to Listeners
	 */
	template<typename TKey = Event<>::Key, typename... Args>
	class EventManager final
	{
		static inline HashMap<TKey, Event<Args...>> events{};
	public:
		STATIC_CLASS(EventManager)

		/**
		 * Will default-construct the Event if it does not exist.
		 *
		 * @param key	the key to look up the Event with
		 * @returns		the Event at that key
		 */
		static Event<Args...>& Event(const TKey& key);

		/**
		 * Destroys all Listeners without destroying their Events.
		 */
		static void RemoveAllListeners();

		/**
		 * Destroys all Events and their Listeners.
		 */
		static void RemoveAllEvents();
	};
}

#include "EventManager.inl"
