// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "Event.h"
#include "LibMath.h"

namespace Library
{
#pragma region Properties
	template<typename ...Args>
	inline constexpr size_t Event<Args...>::ListenerCount() const noexcept
	{
		return listeners.Size();
	}
	
	template<typename ...Args>
	inline constexpr bool Event<Args...>::HasListeners() const noexcept
	{
		return ListenerCount() > 0;
	}
#pragma endregion

#pragma region Insert	
	template<typename ...Args>
	inline void Event<Args...>::AddListener(const Key& key, const Listener& listener)
	{
		pendingOps.PushBack({ listener, key, PendingOp::Type::Add });
		pendingAdditions++;
	}
	
	template<typename ...Args>
	inline typename Event<Args...>::Key Event<Args...>::AddListener(const Listener& listener)
	{
		const std::string key = std::to_string(uniqueCounter++);
		AddListener(key, listener);
		return key;
	}
#pragma endregion

#pragma region Remove
	template<typename ...Args>
	inline void Event<Args...>::RemoveListener(const Key& key)
	{
		pendingOps.PushBack({ Listener(), key, PendingOp::Type::Remove });
		Math::Decrement(pendingAdditions);
	}
	
	template<typename ...Args>
	inline void Event<Args...>::RemoveAllListeners()
	{
		pendingOps.Clear();
		pendingOps.PushBack({ Listener(), Key(), PendingOp::Type::RemoveAll });
		pendingAdditions = 0;
	}
#pragma endregion
	
	template<typename ...Args>
	inline void Event<Args...>::Invoke(Args&& ...args)
	{
		ApplyPending();

		AggregateException aggregateException;

		for (auto& [_, listener] : listeners)
		{
			try
			{
				listener(std::forward<Args>(args)...);
			}
			catch (...)
			{
				aggregateException.exceptions.PushBack(std::current_exception());
			}
		}

		if (!aggregateException.exceptions.IsEmpty())
		{
			throw std::move(aggregateException);
		}
	}

	template<typename ...Args>
	inline void Event<Args...>::ApplyPending()
	{
		// Resize once rather than potentially doing it several times.
		if (listeners.BucketCount() < listeners.Size() + pendingAdditions)
		{
			listeners.Resize(Math::NextPrime(listeners.Size() + pendingAdditions));
		}
		pendingAdditions = 0;

		// Perform all the pending ops.
		for (const auto& [listener, key, type] : pendingOps)
		{
			switch (type)
			{
			case PendingOp::Type::Add:
				listeners.Insert(key, listener);
				break;

			case PendingOp::Type::Remove:
				listeners.Remove(key);
				break;

			case PendingOp::Type::RemoveAll:
				listeners.Clear();
				break;

			default:;
			}
		}
		// TODO: Come up with some sort of a dtor iterator that destructs the list as we iterate.
		pendingOps.Clear();
	}
}
