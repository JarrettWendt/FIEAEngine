// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Stack.h"

#define TEMPLATE template<typename T, typename Container>
#define STACK Stack<T, Container>

namespace Library
{
#pragma region Properties
	TEMPLATE
		inline constexpr bool STACK::IsEmpty() const noexcept
	{
		return Size() <= 0;
	}

	TEMPLATE
		inline constexpr typename STACK::size_type STACK::Size() const noexcept
	{
		return c.Size();
	}
#pragma endregion

#pragma region Element Access
	TEMPLATE
		inline typename STACK::reference STACK::Top()
	{
		return c.Back();
	}

	TEMPLATE
		inline typename STACK::const_reference STACK::Top() const
	{
		return const_cast<STACK*>(this)->Top();
	}

	TEMPLATE
		inline typename STACK::reference STACK::Peek()
	{
		return Top();
	}

	TEMPLATE
		inline typename STACK::const_reference STACK::Peek() const
	{
		return const_cast<STACK*>(this)->Peek();
	}
#pragma endregion

#pragma region Insert
	TEMPLATE
		inline void STACK::Push(const value_type& t)
	{
		c.PushBack(t);
	}

	TEMPLATE
		inline void STACK::Push(value_type&& t)
	{
		c.PushBack(std::move(t));
	}

	TEMPLATE
	template<typename... Args>
	inline auto STACK::Emplace(Args&& ...args)
	{
		return c.Emplace(std::forward<Args>(args)...);
	}
#pragma endregion

#pragma region Remove
	TEMPLATE
		inline void STACK::Pop()
	{
		c.PopBack();
	}

	TEMPLATE
		inline void STACK::Clear()
	{
		c.Clear();
	}
#pragma endregion

#pragma region Operators
	TEMPLATE
		inline bool STACK::operator==(const STACK& other) const
	{
		return c == other.c;
	}

	TEMPLATE
		inline bool STACK::operator!=(const STACK& other) const
	{
		return c != other.c;
	}
#pragma endregion
}

#undef TEMPLATE
#undef STACK
