#include "pch.h"
#include "Coroutine.h"

#include "LibMath.h"

namespace Library
{
#pragma region Coroutine
#pragma region promise_type
	Coroutine::promise_type::Handle Coroutine::promise_type::get_return_object() noexcept
	{
		return Handle::from_promise(*this);
	}

	Coroutine::SuspendAlways Coroutine::promise_type::initial_suspend() noexcept
	{
		return {};
	}

	Coroutine::SuspendAlways Coroutine::promise_type::final_suspend() noexcept
	{
		return {};
	}
	
	void Coroutine::promise_type::unhandled_exception()
	{
		std::scoped_lock lock(Coroutines::mutex);
		Coroutines::aggregateException.exceptions.PushBack(std::current_exception());
	}
	
	void Coroutine::promise_type::return_void() noexcept {}
	
	Coroutine::SuspendAlways Coroutine::promise_type::yield_value(const Time::Seconds seconds) noexcept
	{
		yieldValue = seconds;
		return {};
	}
#pragma endregion

	Coroutine::Coroutine(const promise_type::Handle handle) noexcept :
		handle(handle)
	{
		assert(handle);
	}

	Coroutine::Coroutine(Coroutine&& other) noexcept :
		handle(other.handle)
	{
		other.handle = nullptr;
	}
	
	Coroutine::~Coroutine()
	{
		if (handle)
		{
			handle.destroy();
		}
	}
	
	bool Coroutine::Resume()
	{
		if (!handle.done() && nextResume <= Time::CurrentTime())
		{
			handle.resume();
			nextResume = Time::CurrentTime() + handle.promise().yieldValue;
		}
		return !handle.done();
	}
#pragma endregion

#pragma region Coroutines	
	void Coroutines::Start(const Functor& coroutine, const bool async)
	{
		// Counter from which pseudo-unique IDs are generated when none is provided.
		// If this doesn't work out, we could switch to GUIDs.
		static size_t uniqueCounter{};
		Start(std::to_string(uniqueCounter++), coroutine, async);
	}

	void Coroutines::Start(const Key& key, const Functor& coroutine, const bool async)
	{
		std::scoped_lock lock(mutex);
		const auto func = std::make_shared<Functor>(coroutine);
		const auto coro = std::make_shared<Coroutine>(func->operator()());
		pendingOps.PushBack({ { func, coro }, key, PendingOp::Type::Add, async });
		pendingAdditions++;
	}
	
	void Coroutines::Stop(const Key& key)
	{
		std::scoped_lock lock(mutex);
		pendingOps.PushBack({ { nullptr, nullptr }, key, PendingOp::Type::Remove });
		Math::Decrement(pendingAdditions);
	}
	
	void Coroutines::StopAll()
	{
		std::scoped_lock lock(mutex);
		pendingOps.Clear();
		pendingOps.PushBack({ { nullptr, nullptr } , "", PendingOp::Type::RemoveAll });
		pendingAdditions = 0;
	}
	
	void Coroutines::Update()
	{
		ApplyPending();

		// pump all the blocking coroutines
		for (auto& [key, pair] : blockCoroutines)
		{
			auto& [func, coro] = pair;
			if (!coro->Resume())
			{
				Stop(key);
			}
		}

		// join any async Coroutines
		for (const auto& [key, pair] : asyncCoroutines)
		{
			const auto& [func, future] = pair;
			// semantically same as .wait_for(1ns) == std::future_status::ready
			if (future._Is_ready())
			{
				Stop(key);
			}
		}

		if (!aggregateException.exceptions.IsEmpty())
		{
			throw std::move(aggregateException);
		}
	}
	
	void Coroutines::ApplyPending()
	{
		std::scoped_lock lock(mutex);
		
		if (blockCoroutines.BucketCount() < blockCoroutines.Size() + pendingAdditions)
		{
			blockCoroutines.Resize(Math::NextPrime(blockCoroutines.Size() + pendingAdditions));
		}
		if (asyncCoroutines.BucketCount() < asyncCoroutines.Size() + pendingAdditions)
		{
			asyncCoroutines.Resize(Math::NextPrime(asyncCoroutines.Size() + pendingAdditions));
		}
		pendingAdditions = 0;

		for (auto& [pair, key, type, async] : pendingOps)
		{
			switch (type)
			{
			case PendingOp::Type::Add:
				if (async)
				{
					auto& [func, coro] = pair;
					asyncCoroutines.Insert({ key, { func, std::async(std::launch::async, [coro] { while (coro->Resume()); }) } });
				}
				else
				{
					blockCoroutines.Insert(key, pair);
				}
				break;
				
			case PendingOp::Type::Remove:
				blockCoroutines.Remove(key);
				asyncCoroutines.Remove(key);
				break;
				
			case PendingOp::Type::RemoveAll:
				blockCoroutines.Clear();
				blockCoroutines.Resize(1);
				asyncCoroutines.Clear();
				asyncCoroutines.Resize(1);
				break;
				
			default:;
			}
		}
		pendingOps.Clear();
		// TODO: Have a way to iterate of pendingOps while also destroying it so we don't need the extra traversal caused by calling Clear().
	}
#pragma endregion
}
