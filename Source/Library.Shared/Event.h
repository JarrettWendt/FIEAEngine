// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include <functional>

#include "Exception.h"
#include "HashMap.h"
#include "RTTI.h"
#include "SList.h"

namespace Library
{
	/**
	 * @param <Args>	the types for the arguments that a Listener may take
	 */
	template<typename... Args>
	class Event : public RTTI
	{
		RTTI_DECLARATIONS(RTTI)

	public:
		/** the type that listens for this Event */
		using Listener = std::function<void(Args...)>;
		/** the type that will be used as keys for looking up or removing listeners */
		using Key = std::string;

	private:
		/**
		 * A container to hold all the data for a pending operation.
		 * We store pending ops instead of just doing them immediately because manipulating the container of Listeners while invoking a Listener would disrupt the iterator.
		 * We store pending ops in one container instead of separate lists for adds/removes so that order is preserved.
		 */
		struct PendingOp final
		{
			enum class Type : uint8_t
			{
				Add,
				Remove,
				RemoveAll
			};

			Listener listener;
			Key key;
			Type type;
		};

		/**
		 * Where all Listeners are stored.
		 * We use a HashMap because it gives us...
		 * - Faster removals.
		 * - Guaranteed no duplicate listeners.
		 */
		HashMap<Key, Listener> listeners{};
		/** the list of pending operations */
		SList<PendingOp> pendingOps{};
		/**
		 * The net number of items pending to be added.
		 * Not necesarilly same as pendingOps.Size().
		 * Saves on superflous HashMap resizes.
		 */
		size_t pendingAdditions{};
		/**
		 * Counter for generating pseudo-unique keys.
		 * Only non-unique if:
		 * - This number overflows (unlikely).
		 * - The user uses numeric keys themselves (not out of the realm of possibility).
		 * If this doesn't work out, we should switch to GUIDs.
		 */
		size_t uniqueCounter{};

	public:
		SPECIAL_MEMBERS_V(Event, default)

#pragma region Properties
		/**
		 * @returns		how many Listeners this Event has
		 */
		[[nodiscard]] constexpr size_t ListenerCount() const noexcept;

		/**
		 * @returns		if this Event has any Listeners
		 */
		[[nodiscard]] constexpr bool HasListeners() const noexcept;
#pragma endregion

#pragma region Insert
		/**
		 * Will take effect on the next Invoke() before calling any Listener.
		 * O(1)
		 * 
		 * @param key			the Key to add this Listener as
		 * @param listener		the Listener to add to this Event
		 */
		void AddListener(const Key& key, const Listener& listener);

		/**
		 * Will take effect on the next Invoke() before calling any Listener.
		 * O(1)
		 *
		 * @param listener		the Listener to add to this Event
		 * @returns				the Key this Listener was added with
		 */
		Key AddListener(const Listener& listener);
#pragma endregion

#pragma region Remove
		/**
		 * Unsubscribe a Listener from this Event.
		 * If a Listener with this Key does not exist, does nothing.
		 * Will take effect on the next Invoke() before calling any Listener.
		 * 
		 * @param key			the Key of the Listener to remove 
		 */
		void RemoveListener(const Key& key);

		/**
		 * Unsubscribes all Listeners from this Event.
		 * Will take place on the next Invoke() before calling any Listener.
		 * O(n) where n is ListenerCount()
		 */
		void RemoveAllListeners();
#pragma endregion

		/**
		 * Invokes all of this Event's Listeners.
		 * 
		 * @param ...args		the params to pass along to the Listeners
		 *
		 * @throws AggregateException if any of the listeners throw an exception. All Listeners will still be executed.
		 */
		void Invoke(Args&&... args);
		
	private:
		/**
		 * Applies the pending list. 
		 */
		void ApplyPending();
	};
}

#include "Event.inl"
