// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "SList.h"

namespace Library
{
	/**
	 * LIFO wrapper with no fixed size limit.
	 *
	 * @param T				the type to store
	 * @param Container		the container to use internally
	 */
	template<typename T, typename Container = SList<T>>
	class Stack
	{
	public:
		using container_type = Container;
		using value_type = typename container_type::value_type;
		using size_type = typename container_type::size_type;
		using reference = typename container_type::reference;
		using const_reference = typename container_type::const_reference;
		
	protected:
		container_type c{};

	public:
		SPECIAL_MEMBERS(Stack, default)

#pragma region Properties
		/**
		 * O(1)
		 *
		 * @returns		whether or not the container is empty
		 */
		[[nodicard]] constexpr bool IsEmpty() const noexcept;

		/**
		 * O(1)
		 *
		 * @returns		how many elements are in the container
		 */
		[[nodicard]] constexpr size_type Size() const noexcept;
#pragma endregion

#pragma region Element Access
		/**
		 * O(1)
		 *
		 * @returns		the item at the Top of the Queue
		 *
		 * @throws std::out_of_range if the container is empty
		 */
		reference Top();

		/**
		 * O(1)
		 *
		 * @returns		the item at the Top of the Queue
		 *
		 * @throws std::out_of_range if the container is empty
		 */
		const_reference Top() const;

		/**
		 * O(1)
		 *
		 * @returns		the item at the Top of the Queue
		 *
		 * @throws std::out_of_range if the container is empty
		 */
		reference Peek();

		/**
		 * O(1)
		 *
		 * @returns		the item at the Top of the Queue
		 *
		 * @throws std::out_of_range if the container is empty
		 */
		const_reference Peek() const;
#pragma endregion

#pragma region Insert
		/**
		 * O(1)
		 *
		 * @param t		the element to be put into the Back of the Queue
		 */
		void Push(const value_type& t);

		/**
		 * O(1)
		 *
		 * @param t		the element to be put into the Back of the Queue
		 */
		void Push(value_type&& t);

		/**
		 * O(1)
		 *
		 * @param <Args>	the type for the arguments to construct a value_type
		 * @param args		the arguments to construct a value_type in-place
		 */
		template<typename... Args>
		auto Emplace(Args&&... args);
#pragma endregion

#pragma region Remove
		/**
		 * removes the element at the Front of the Queue
		 * O(1)
		 */
		void Pop();

		/**
		 * destructs all elements from the container
		 */
		void Clear();
#pragma endregion

#pragma region Operators
		/**
		 * O(n)
		 *
		 * @param other		the container to compare this one against
		 * @return true		if all elements are the same in both containers
		 * @return false	otherwise
		 */
		[[nodiscard]] bool operator==(const Stack& other) const;

		/**
		 * O(n)
		 *
		 * @param other		the container to compare this one against
		 * @return true		if at least 1 element is different in these two containers
		 * @return false	otherwise
		 */
		[[nodiscard]] bool operator!=(const Stack& other) const;
#pragma endregion
	};
}

#include "Stack.inl"
