// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include <experimental/coroutine>
#include <future>
#include <mutex>

#include "EngineTime.h"
#include "Exception.h"
#include "HashMap.h"

namespace Library
{
	/**
	 * the type which a Coroutine function should return
	 */
	class Coroutine final
	{
		friend class Coroutines;
		using SuspendAlways = std::experimental::suspend_always;
		using SuspendNever = std::experimental::suspend_never;

	public:
		struct promise_type
		{
			friend class Coroutine;
			using Handle = std::experimental::coroutine_handle<promise_type>;

			Time::Seconds yieldValue{};

		public:
			/**
			 * @returns a handle to this promise
			 */
			Handle get_return_object() noexcept;

			/**
			 * Behavior when the Coroutine is created.
			 * Does it immediately suspend or does it run until the first co_ statement?
			 * 
			 * @returns	always suspending
			 */
			SuspendAlways initial_suspend() noexcept;

			/**
			 * Invoked whenever cont rol leaves a Coroutine statement such as with:
			 * unhandled_exception()
			 * return_value()
			 * return_void()
			 *
			 * @returns	always suspending
			 */
			SuspendAlways final_suspend() noexcept;

			/**
			 * Invoked when the coroutine throws an exception.
			 * Currently does nothing so Coroutines::Update can aggregate it.
			 */
			void unhandled_exception();

			/**
			 * What co_return invokes when returning nothing.
			 * return_value is invoked when we are returning something.
			 * We can only have return_void or return_value, not both.
			 */
			void return_void() noexcept;

			/**
			 * what co_yield invokes
			 *
			 * @param seconds	how many seconds until the Coroutine should be resumed
			 * @returns			always suspending
			 */
			SuspendAlways yield_value(Time::Seconds seconds) noexcept;
		};

	private:
		/** a handle to our promise_type */
		promise_type::Handle handle;
		/** the next time Resume() should be called */
		Time::Point nextResume{ Time::CurrentTime() };

	public:
		/**
		 * implicit converting ctor
		 *
		 * @param handle	Handle to this Coroutine's promise_type
		 */
		Coroutine(promise_type::Handle handle) noexcept;
		COPY_SEMANTICS(Coroutine, delete)
		Coroutine(Coroutine&& other) noexcept;
		Coroutine& operator=(Coroutine&& other) noexcept = default;
		~Coroutine();

		/**
		 * invokes the Coroutine's function, if its time is up.
		 * 
		 * @returns		true if this Coroutine has more work to do 
		 */
		bool Resume();
	};

	/**
	 * static container of all Coroutines
	 */
	class Coroutines final
	{
		friend class Engine;
		friend Coroutine;
		
	public:
		STATIC_CLASS(Coroutines)

		/** the type which is used to look up specific Coroutines */
		using Key = std::string;
		using Functor = std::function<Coroutine()>;

	private:
		using Pair = std::pair<std::shared_ptr<Functor>, std::shared_ptr<Coroutine>>;
		
		struct PendingOp final
		{
			enum class Type : uint8_t
			{
				Add,
				Remove,
				RemoveAll
			};

			Pair pair;
			Key key;
			Type type;
			bool async = false;
		};

		static inline HashMap<Key, Pair> blockCoroutines{};
		static inline HashMap<Key, std::pair<std::shared_ptr<Functor>, std::shared_future<void>>> asyncCoroutines{};
		static inline SList<PendingOp> pendingOps{};
		static inline AggregateException aggregateException{};
		
		/**
		 * Lock for when accessing the pending list.
		 * This is to ensure safety when attempting to Start/Stop from within an async Coroutine.
		 * We don't need a lock for accessing the HashMaps because Update() should only be called from the main thread.
		 */
		static inline std::mutex mutex{};
		/**
		 * The net number of items pending to be added.
		 * Not necessarily same as pendingOps.Size().
		 */
		static inline size_t pendingAdditions{};

	public:
		/**
		 * O(1)
		 * 
		 * @returns		how many Coroutines are in the queue
		 */
		static constexpr size_t Count() noexcept;

		/**
		 * O(1)
		 * 
		 * @param coroutine		the Coroutine to enqueue
		 * @param async			whether or not to run the Coroutine asynchronously
		 */
		static void Start(const Functor& coroutine, bool async = false);

		/**
		 * O(1)
		 * 
		 * @param key			the Key for this Coroutine
		 * @param coroutine		the coroutine to enqueue
		 * @param async			whether or not to run the Coroutine asynchronously
		 */
		static void Start(const Key& key, const Functor& coroutine, bool async = false);

		/**
		 * O(1)
		 * 
		 * @param key	the Key of the Coroutine to be stopped
		 */
		static void Stop(const Key& key);

		/**
		 * Stops all Coroutines.
		 * Also frees any memory used to store all the Coroutines.
		 * O(n) where n is Count()
		 */
		static void StopAll();

	private:
		/**
		 * To be invoked by Engine::Update().
		 * Pumps all Coroutines.
		 *
		 * @throws AggregateException	If any of the Coroutines throw an exception. All Coroutines will still be ran.
		 */
		static void Update();

		/**
		 * Applies the pending list.
		 */
		static void ApplyPending();
	};
}

#include "Coroutine.inl"
