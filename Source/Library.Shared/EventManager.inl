#pragma once
#include "EventManager.h"

namespace Library
{
	template<typename TKey, typename ...Args>
	inline Event<Args...>& EventManager<TKey, Args...>::Event(const TKey& key)
	{
		return events[key];
	}
	
	template<typename EventKey, typename ...Args>
	inline void EventManager<EventKey, Args...>::RemoveAllListeners()
	{
		for (auto& [key, event] : events)
		{
			event.RemoveAllListeners();
		}
	}
	
	template<typename TKey, typename ...Args>
	inline void EventManager<TKey, Args...>::RemoveAllEvents()
	{
		events.Clear();
		events.Resize(1);
	}
}
