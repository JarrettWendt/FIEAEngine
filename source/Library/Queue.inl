// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Queue.h"

#define TEMPLATE template<typename T, typename Container>
#define QUEUE Queue<T, Container>

namespace Library
{
#pragma region Properties
	TEMPLATE
	inline constexpr bool QUEUE::IsEmpty() const noexcept
	{
		return Size() <= 0;
	}

	TEMPLATE
	inline constexpr typename QUEUE::size_type QUEUE::Size() const noexcept
	{
		return c.Size();
	}
#pragma endregion

#pragma region Element Access
	TEMPLATE
	inline typename QUEUE::reference QUEUE::Front()
	{
		return c.Front();
	}

	TEMPLATE
	inline typename QUEUE::const_reference QUEUE::Front() const
	{
		return const_cast<QUEUE*>(this)->Front();
	}

	TEMPLATE
	inline typename QUEUE::reference QUEUE::Back()
	{
		return c.Back();
	}

	TEMPLATE
	inline typename QUEUE::const_reference QUEUE::Back() const
	{
		return const_cast<QUEUE*>(this)->Back();
	}
#pragma endregion

#pragma region Insert
	TEMPLATE
	inline void QUEUE::Enqueue(const value_type& t)
	{
		c.PushBack(t);
	}

	TEMPLATE
	inline void QUEUE::Enqueue(value_type&& t)
	{
		c.PushBack(std::move(t));
	}

	TEMPLATE
	template<typename... Args>
	inline auto QUEUE::Emplace(Args&& ...args)
	{
		return c.Emplace(std::forward<Args>(args)...);
	}
#pragma endregion

#pragma region Remove
	TEMPLATE
	inline void QUEUE::Dequeue()
	{
		c.PopFront();
	}

	TEMPLATE
	inline void QUEUE::Clear()
	{
		c.Clear();
	}
#pragma endregion

#pragma region Operators
	TEMPLATE
	inline bool QUEUE::operator==(const QUEUE& other) const
	{
		return c == other.c;
	}

	TEMPLATE
	inline bool QUEUE::operator!=(const QUEUE& other) const
	{
		return c != other.c;
	}
#pragma endregion
}

#undef TEMPLATE
#undef QUEUE
