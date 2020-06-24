// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include <variant>

#include "Array.h"

namespace Library
{
	/**
	 * A homogenous container that can hold many different values of many different types, but only one type at a time.
	 * 
	 * @param <Ts>	The types for this container to store.
	 */
	template<typename... Ts>
	class VariantArray
	{
		std::variant<Array<Ts>...> variants;
	public:
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		/**
		 * wrapper for a reference to an index of this container
		 * similar to an iterator, but no functionality to iterate
		 * in exchange for no iteration, it provides implicit dereferencing
		 */
		struct value_type final
		{
			friend class VariantArray;
			
		private:
			size_type index;
			VariantArray& owner;

		private:
			value_type(size_type index, VariantArray& owner);
		public:
			SPECIAL_MEMBERS(value_type, default)

			/**
			 * @param <T>	The type to compare against the contained type.
			 * @returns		Whether or not this container contains the specified type.
			 */
			template<typename T>
			[[nodiscard]] constexpr bool IsType() const noexcept;

			/**
			 * recommended for the user to create a strong enumeration mapping this index
			 *
			 * @returns 0	if the container has never held a value
			 * @returns		the one-based index of the alternative that is currently held
			 */
			[[nodiscard]] constexpr size_t TypeID() const noexcept;

#pragma region Operators
			/**
			 * assigns to the value this wrapper references
			 *
			 * @param t		the value to assign
			 * @returns		this after assignment (the wrapper itself should be unchanged)
			 */
			template<typename T>
			value_type& operator=(const T& t);
			
			/**
			 * implicit conversion
			 * usage:
			 * static_cast<T>(my_value_type)
			 * for (T t : myDatum)
			 */
			template<typename T>
			explicit operator T () const;

			/**
			 * implicit conversion
			 * usage:
			 * static_cast<T&>(my_value_type)
			 * for (T& t : myDatum)
			 */
			template<typename T>
			operator T& ();

			/**
			 * implicit conversion
			 * usage:
			 * static_cast<const T&>(my_value_type)
			 * with a const Datum:
			 * for (const T& t : myConstDatum)
			 * cannot use this with non-const Datum
			 */
			template<typename T>
			operator const T& () const;

			/**
			 * "to_string" operator for value_type
			 *
			 * @param stream	the stream to append to
			 * @param val		the value to stream from
			 * @returns			the stream after the append
			 */
			friend std::ostream& operator<<(std::ostream& stream, const value_type val)
			{
				return val.StreamTo(stream);
			}

#pragma region Comparison
			COMPARISONS(value_type)
			
			/**
			 * comparison operator for element type
			 *
			 * @param <T>	type that this wrapper may reference
			 * @param t		value to compare this wrapper against
			 * @returns		whether or not the value this wrapper references is equivalent to t
			 */
			template<typename T>
			[[nodiscard]] friend bool operator==(const value_type& val, const T& t)
			{
				return val.IsType<T>() && val.owner.Get<T>(val.index) == t;
			}

			/**
			 * comparison operator for element type
			 *
			 * @param <T>	type that this wrapper may reference
			 * @param t		value to compare this wrapper against
			 * @returns		whether or not the value this wrapper references is inequivalent to t
			 */
			template<typename T>
			[[nodiscard]] friend bool operator!=(const value_type& val, const T& t)
			{
				return !(val == t);
			}

			/**
			 * comparison operator for element type
			 *
			 * @param <T>	type that this wrapper may reference
			 * @param t		value to compare this wrapper against
			 * @returns		whether or not the value this wrapper references is equivalent to t
			 */
			template<typename T>
			[[nodiscard]] friend bool operator==(const T& t, const value_type& val)
			{
				return val == t;
			}

			/**
			 * comparison operator for element type
			 *
			 * @param <T>	type that this wrapper may reference
			 * @param t		value to compare this wrapper against
			 * @returns		whether or not the value this wrapper references is inequivalent to t
			 */
			template<typename T>
			[[nodiscard]] friend bool operator!=(const T& t, const value_type& val)
			{
				return val != t;
			}
#pragma endregion
#pragma endregion

		private:
			/**
			 * helper for operator<< because class-friendship doesn't extend to friend functions
			 * 
			 * @param stream	the stream to stream to
			 * @returns			the same stream
			 */
			std::ostream& StreamTo(std::ostream& stream) const;
		};
		using reference = value_type;
		using cosnt_reference = const reference;
		using pointer = value_type*;
		using const_pointer = const pointer;

		/**
		 * exception that will be thrown when attempting to dereference the container as a type that it is not
		 */
		class InvalidTypeException final : public std::invalid_argument
		{
		public:
			explicit InvalidTypeException(const std::string& str) : std::invalid_argument(str) {};
			SPECIAL_MEMBERS(InvalidTypeException, default)
		};

#pragma region Special Members
		/**
		 * explicit ctor
		 *
		 * @param count			how many elements of space to reserve and construct
		 * @param prototype		prototypical value to be copy-constructed into each element
		 */
		template<typename T>
		VariantArray(size_type count, const T& prototype);

		/**
		 * explicit ctor
		 * Fills the container with values read through the iterators.
		 *
		 * @param <It>			the iterator type
		 * @param first			the beginning iterator, inclusive
		 * @param last			the last iterator, exclusive
		 */
		template<std::forward_iterator It>
		VariantArray(It first, It last);

		/**
		 * explicit ctor
		 * Fills the container with values read through the iterators.
		 *
		 * @param <It>			the iterator type
		 * @param first			the beginning iterator, inclusive
		 * @param last			the last iterator, exclusive
		 */
		template<std::random_access_iterator It>
		VariantArray(It first, It last);

		/**
		 * typecast ctor which accepts any range
		 * will at most resize once
		 *
		 * @param range			the range to construct from
		 */
		template<std::ranges::range Range>
		VariantArray(const Range& range);

		/**
		 * typecast ctor which accepts any range
		 * will at most resize once
		 *
		 * @param range			the range to construct from
		 */
		template<std::ranges::range Range>
		VariantArray& operator=(const Range& range)
		{
			if constexpr (Util::IsOneOf<Range, Ts...>())
			{
				Assign(range);
			}
			else
			{
				variants = Array<Util::BestMatch<typename Range::value_type, Ts...>>(range.begin(), range.end());
			}
			return *this;
		}

		/**
		 * initializer_list ctor
		 *
		 * @param list		the initializer_list to construct this container from
		 */
		template<typename T>
		VariantArray(std::initializer_list<T> list);

		/**
		 * initializer_list operator=
		 *
		 * @param list		the initializer_list to assign this container to
		 * @return			this container after assignment
		 */
		template<typename T>
		VariantArray& operator=(std::initializer_list<T> list)
		{
			variants.operator=(list);
			return *this;
		}

		/**
		 * Assignment ctor for a single element.
		 * After this, the container will hold a single element, a copy of t.
		 *
		 * @param t		the value to assign this container to
		 * @returns		this after assignment
		 */
		template<typename T>
		VariantArray& operator=(const T& t);

		SPECIAL_MEMBERS(VariantArray, default)
#pragma endregion

#pragma region iterator		
		class iterator final
		{
			friend class VariantArray;
			friend class const_iterator;

		private:
			VariantArray* owner{ nullptr };
			size_type index{ 0 };

		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = VariantArray::value_type;
			using difference_type = ptrdiff_t;
			using pointer = std::unique_ptr<value_type>;
			using reference = value_type;

			/**
			 * explicit constructor
			 *
			 * @param index		index for this iterator to start at
			 * @param owner		the container this iterator references
			 */
			iterator(const size_type index, VariantArray& owner) noexcept :
				owner(&owner),
				index(index) {}

			SPECIAL_MEMBERS(iterator, default)

			/**
			 * Difference of two iterators.
			 *
			 * @param left		lhs of the operator
			 * @param right		rhs of the operator
			 * @returns			The difference between the indices of the two iterators.
			 *
			 * @asserts			The iterators are initialized and they belong to the same container.
			 */
			[[nodiscard]] friend difference_type operator-(const iterator left, const iterator right) noexcept
			{
				// No need to AssertInitialize anything because it will be called by operator +=
				// AssertInitialize is therefore implicitly called on right since we're comparing it to left's owner
				assertm(left.owner == right.owner, "iterators must belong to the same container");
				return left.index - right.index;
			}

			/**
			 * Will not go beyond end().
			 *
			 * @param i		How much to increment the iterator by.
			 * @returns		A new iterator at that index.
			 *
			 * @asserts		this iterator is initialized
			 */
			[[nodiscard]] iterator operator+(difference_type i) const noexcept;

			/**
			 * @returns		The value of the index the iterator is at.
			 *
			 * @asserts		this iterator is initialized and not at end().
			 */
			[[nodiscard]] reference operator*() const;

			/**
			 * shortcut to *(it + i)
			 * 
			 * @param i		the offset
			 * @returns		the value at this iterator offset by i
			 */
			[[nodiscard]] reference operator[](difference_type i) const noexcept;

			/**
			 * @returns true if this iterator is both initialized and not at end().
			 */
			[[nodiscard]] explicit operator bool() const noexcept;

			/**
			 * Inverse of operator bool().
			 *
			 * @returns true if this iterator is uninitialized or at end(), false otherwise.
			 */
			[[nodiscard]] bool operator!() const noexcept;
			
			RANDOM_ITER_ARITHMETIC_OPS(iterator)
			COMPARISONS(iterator)

			/**
			 * O(1)
			 * @returns		Whether or not this iterator is at end().
			 *
			 * @asserts		the iterator is initialized.
			 */
			[[nodiscard]] bool IsAtEnd() const noexcept;

			/**
			 * O(1)
			 * @returns		Whether or not this iterator is at begin().
			 */
			[[nodiscard]] bool IsAtBegin() const noexcept;

		private:
			/**
			 * @asserts		this iterator is initialized.
			 */
			constexpr void AssertInitialized() const noexcept;
		};

		class const_iterator final
		{
			friend class VariantArray;
			CONST_RANDOM_ACCESS_ITERATOR(const_iterator, iterator)

			/**
			 * explicit ctor
			 *
			 * @param index		index for this iterator to start at
			 * @param owner		the container this iterator references
			 */
			const_iterator(const size_t index, const VariantArray& owner) noexcept :
			it{ index, const_cast<VariantArray&>(owner) } {};

			/**
			 * @returns true if this iterator is both initialized and not at end().
			 */
			[[nodiscard]] explicit operator bool() const noexcept;

			/**
			 * Inverse of operator bool().
			 *
			 * @returns true if this iterator is uninitialized or at end(), false otherwise.
			 */
			[[nodiscard]] bool operator!() const noexcept;

			/**
			 * O(1)
			 * @returns		Whether or not this iterator is at end().
			 *
			 * @throws std::runtime_error	if the iterator is uninitialized.
			 */
			[[nodiscard]] bool IsAtEnd() const noexcept;

			/**
			 * O(1)
			 * @returns		Whether or not this iterator is at begin().
			 */
			[[nodiscard]] bool IsAtBegin() const noexcept;
		};

		BEGIN_END(iterator, const_iterator, VariantArray)
		MAKE_REVERSE_BEGIN_END
#pragma endregion

#pragma region Properties
		/**
		 * @returns		the number of elements this container can store without resizing
		 */
		[[nodiscard]] constexpr size_type Capacity() const noexcept;

		/**
		 * @returns		how many elements are in the container
		 */
		[[nodiscard]] constexpr size_type Size() const noexcept;

		/**
		 * @returns true	if the container is empty
		 * @returns false	otherwise
		 */
		[[nodiscard]] constexpr bool IsEmpty() const noexcept;

		/**
		 * @returns true	if the container is full
		 * @returns false	otherwise
		 */
		[[nodiscard]] constexpr bool IsFull() const noexcept;

		/**
		 * @param <T>	The type to compare against the contained type.
		 * @returns		Whether or not this container contains the specified type.
		 */
		template<typename T>
		[[nodiscard]] constexpr bool IsType() const noexcept;

		/**
		 * recommended for the user to create a strong enumeration mapping this index
		 * 
		 * @returns 0	if the container has never held a value
		 * @returns		the one-based index of the alternative that is currently held
		 */
		[[nodiscard]] constexpr size_t TypeID() const noexcept;

		/**
		 * @returns 0	if T cannot be one of Ts
		 * @returns		the TypeID of T from Ts
		 */
		template<typename T>
		static constexpr size_t TypeID() noexcept;
#pragma endregion

#pragma region Element Access
		/**
		 * @param <T>		type of the element to return
		 * @param index		position of the element to return
		 * @returns			element of type T at index
		 *
		 * @throws std::out_of_range		if index >= Size()
		 * @throws InvalidTypeException		if this container does not store types of T
		 */
		template<typename T>
		T& Get(size_t index = 0);
		
		/**
		 * @param <T>		type of the element to return
		 * @param index		position of the element to return
		 * @returns			element of type T at index
		 *
		 * @throws std::out_of_range		if index >= Size()
		 * @throws InvalidTypeException		if this container does not store types of T
		 */
		template<typename T>
		const T& Get(size_t index = 0) const;

		/**
		 * @param <T>		type of the element to return
		 * @param index		position of the element to return
		 * @returns			element of type T at index
		 *
		 * @throws std::out_of_range		if index >= Size()
		 * @throws InvalidTypeException		if this container does not store types of T
		 */
		template<typename T>
		T& At(size_t index);

		/**
		 * @param <T>		type of the element to return
		 * @param index		position of the element to return
		 * @returns			element of type T at index
		 *
		 * @throws std::out_of_range		if index >= Size()
		 * @throws InvalidTypeException		if this container does not store types of T
		 */
		template<typename T>
		const T& At(size_t index) const;

		/**
		 * @param index		index to retrieve data from
		 * @returns			value wrapper at that index
		 */
		value_type operator[](size_t index);

		/**
		 * @param index		index to retrieve data from
		 * @returns			value wrapper at that index
		 */
		const value_type operator[](size_t index) const;

		/**
		 * @returns		reference to the first element.
		 *
		 * @throws std::out_of_range		if the container is empty
		 * @throws InvalidTypeException		if this container does not store types of T
		 */
		template<typename T>
		T& Front();

		/**
		 * @returns		reference to the first element.
		 *
		 * @throws std::out_of_range		if the container is empty
		 * @throws InvalidTypeException		if this container does not store types of T
		 */
		template<typename T>
		const T& Front() const;

		/**
		 * @returns		reference to the last element.
		 *
		 * @throws std::out_of_range		if the container is empty
		 * @throws InvalidTypeException		if this container does not store types of T
		 */
		template<typename T>
		T& Back();

		/**
		 * @returns		reference to the last element.
		 *
		 * @throws std::out_of_range		if the container is empty
		 * @throws InvalidTypeException		if this container does not store types of T
		 */
		template<typename T>
		const T& Back() const;
#pragma endregion

#pragma region Setters
		/**
		 * Sets the type for this container to hold.
		 * Will erase all data stored within.
		 * 
		 * @param <T>	The new type for this container.
		 */
		template<typename T>
		void SetType();

		/**
		 * @param <T>		the type to insert
		 * @param index		the index to set at
		 *
		 * @throws InvalidTypeException		if the container does not store types of T
		 * @throws std::out_of_range		if index >= Size()
		 */
		template<typename T>
		void Set(const T& t, const size_t index = 0);

		/**
		 * assigns this Datum to a scalar
		 * same as operator=<T>
		 *
		 * @param t		the scalar to set this container to
		 */
		template<typename T>
		void Assign(const T& t);
#pragma endregion

#pragma region Insert
		/**
		 * @param index		where the new element will be construted
		 * @param t			value to insert
		 */
		template<typename T>
		void Insert(size_t index, const T& t);

		/**
		 * @param index		where the new element will be construted
		 * @param count		how many copies of the value to insert
		 * @param t			value to insert
		 * @returns			iterator at the position of the last element inserted
		 */
		template<typename T>
		iterator Insert(size_t index, size_t count, const T& t);

		/**
		 * @param index		where the new element will be construted
		 * @param list		initializer_list of values to insert
		 * @returns			iterator at the position of the last element inserted
		 */
		template<typename T>
		iterator Insert(size_t index, std::initializer_list<T> list);

		/**
		 * Insert elements through the values read from the iterators in the range [first, last).
		 *
		 * @param index		where the first new element will be constructed
		 * @param first		beginning iterator, inclusive
		 * @param last		ending iterator, exclusive
		 * @returns			iterator at the position of the last element inserted
		 *
		 * @throws			std::invalid_argument if the iterators belong to this container
		 */
		template<std::forward_iterator It>
		iterator Insert(size_t index, It first, It last);

		/**
		 * @param it		position of where to insert
		 * @param t			prototypical value to insert
		 *
		 * @throws std::invalid_argument	if the iterator does not belong to this container
		 */
		template<typename T>
		void Insert(const_iterator it, const T& t);

		/**
		 * @param it		initial position of where to insert
		 * @param count		how many copies of the prototype to insert
		 * @param t			prototypical value to insert
		 * @returns			iterator at the last element inserted
		 *
		 * @throws std::invalid_argument	if the iterator does not belong to this container
		 */
		template<typename T>
		iterator Insert(const_iterator it, size_t count, const T& t);


		/**
		 * @param it		initial position of where to insert
		 * @param list		initializer_list of values to insert
		 * @returns			iterator at the last element inserted
		 *
		 * @throws std::invalid_argument	if the iterator does not belong to this container
		 */
		template<typename T>
		iterator Insert(const_iterator it, std::initializer_list<T> list);

		/**
		 * Insert elements through the values read from the iterators in the range [first, last).
		 *
		 * @param it		initial position of where to insert
		 * @param first		beginning iterator, inclusive
		 * @param last		ending iterator, exclusive
		 * @returns			iterator at the position of the last element inserted
		 *
		 * @throws			std::invalid_argument if the iterators do not belong to this container
		 */
		template<std::forward_iterator It>
		iterator Insert(const_iterator it, It first, It last);

		/**
		 * Inserts a new element into the container directly at index.
		 *
		 * @param index		Position in the container to put this element.
		 * @param args		Arguments to forward to the constructor of the element.
		 *
		 * @throws std::out_of_range	if index > Size().
		 */
		template<typename T, typename... Args>
		T& Emplace(size_t index, Args&&... args);

		/**
		 * Inserts a new element into the container directly at index.
		 * The passed iterator is guaranteed to be valid and pointing at the position of the newly emplaced element after the operation.
		 *
		 * @param it		Position in the container to put this element.
		 * @param args		Arguments to forward to the constructor of the element.
		 *
		 * @throws std::out_of_range	if index > Size().
		 */
		template<typename T, typename... Args>
		T& Emplace(const_iterator it, Args&&... args);

		/**
		 * Appends a new element to the front of the container.
		 * O(1) most cases.
		 * O(n) if the operation requires a Resize().
		 *
		 * @param args		Arguments to forward to the constructor of the element.
		 */
		template<typename T, typename... Args>
		T& EmplaceFront(Args&&... args);

		/**
		 * Appends a new element to the end of the container.
		 * O(1) most cases.
		 * O(n) if the operation requires a Resize().
		 *
		 * @param args		Arguments to forward to the constructor of the element.
		 */
		template<typename T, typename... Args>
		T& EmplaceBack(Args&&... args);
		
		/**
		 * Appends the element to the end of the container.
		 * O(1) most cases.
		 * O(n) if the operation requires a Resize().
		 *
		 * @param t		The value of the element to append.
		 */
		template<typename T>
		void PushBack(const T& t);

		/**
		 * Appends the element to the end of the container.
		 * O(1) most cases.
		 * O(n) if the operation requires a Resize().
		 *
		 * @param t		The value of the element to append.
		 */
		template<typename T>
		void PushBack(T&& t);

		/**
		 * Appends the element to the front of the container.
		 * O(n) where n is Size().
		 *
		 * @param t		The value of the element to prepend.
		 */
		template<typename T>
		void PushFront(const T& t);

		/**
		 * Appends the element to the front of the container.
		 * O(n) where n is Size().
		 *
		 * @param t		The value of the element to prepend.
		 */
		template<typename T>
		void PushFront(T&& t);

		/**
		 * Insert elements through the values read from the iterators in the range [first, last).
		 * Only resizes once (this is most efficient if It is a random access iterator).
		 *
		 * @param it		initial position of where to insert
		 * @param first		beginning iterator, inclusive
		 * @param last		ending iterator, exclusive
		 * @returns			iterator at the position of the last element inserted
		 */
		template<std::forward_iterator It>
		void PushBack(It first, It last);

		/**
		 * Insert elements through the values read from the iterators in the range [first, last).
		 * Only resizes once (this is most efficient if It is a random access iterator).
		 *
		 * @param it		initial position of where to insert
		 * @param first		beginning iterator, inclusive
		 * @param last		ending iterator, exclusive
		 * @returns			iterator at the position of the last element inserted
		 */
		template<std::forward_iterator It>
		void PushFront(It first, It last);

		/**
		 * Copies the passed prototypical value into the remaining empty slots.
		 * O(n) where n is Capacity() - Size().
		 *
		 * @param prototype		The prottype to fill the container with.
		 */
		template<typename T>
		void Fill(const T& prototype);
#pragma endregion

#pragma region Remove
		/**
		 * Removes the first instance of the passed value from the container.
		 * Will only return false if the value did not exist in the container.
		 * A return value of true does not imply that no duplicate values do not still exist in the container.
		 * All elements after the removed one get shifted to the left in memory.
		 * O(n)
		 *
		 * @param t		The value to remove.
		 * @returns		Whether or not a removal operation was performed.
		 */
		template<typename T>
		bool Remove(const T& t);

		/**
		 * Removes the first element matching the passed predicate.
		 * Will only return false if the value did not exist in the container.
		 * A return value of true does not imply that no duplicate values do not still exist in the container.
		 * All elements after the removed one get shifted to the left in memory.
		 * O(n)
		 *
		 * @param predicate		The predicate to query with.
		 * @returns				Whether or not a removal was performed.
		 */
		template<typename T, std::predicate<T> Predicate>
		bool Remove(Predicate predicate);

		/**
		 * Removes the index specifid and shift values to the left over.
		 * O(n) where n is Size() - index;
		 *
		 * @param index		The index to remove.
		 */
		void RemoveAt(size_type index);

		/**
		 * Removes all elements mathcing the specified value.
		 * O(n)
		 *
		 * @param t		The value to remove all instances of.
		 * @returns		How many removals were performed.
		 */
		template<typename T>
		size_type RemoveAll(const T& t);

		/**
		 * Removes all elements matching the passed predicate.
		 * O(n)
		 *
		 * @param predicate		The predicate to query with.
		 * @returns				How many elements were removed.
		 */
		template<typename T, std::predicate<T> Predicate>
		size_type RemoveAll(Predicate predicate);

		template<typename Predicate>
		friend size_type erase_if(VariantArray& arr, const Predicate predicate)
		{
			return arr.RemoveAll(predicate);
		}

		/**
		 * Removes the range [first, last) elements between the two passed indices.
		 * If last is beyond Size() then all elements from first to the end will be removed safely.
		 * Does nothing if the indices are both beyond Size().
		 *
		 * @param first		index of the first value to remove, inclusive.
		 * @param last		index of the last value to remove, exclusive.
		 * @returns			The number of elements removed, equal to first - last unless last > Size().
		 *
		 * @throws std::invalid_argument	if first > last.
		 */
		size_type Remove(size_type first, size_type last);

		/**
		 * Removes the range [first, last) elements between the two passed iterators.
		 * O(n) where n = Size() - first
		 *
		 * @param first		iterator at the first element to remove, inclusive
		 * @param last		iterator at the last element to remove, exclusive
		 * @returns			The number of elements removed.
		 */
		size_t Remove(const_iterator first, const_iterator last);
		
		/**
		 * O(1)
		 * Removes the last element of the container.
		 * Does nothing if the container is empty.
		 */
		void PopBack();

		/**
		 * O(n) where n = Size() - 1 before the operation.
		 * Removes the first element of the container.
		 * Does nothing if the container is empty.
		 */
		void PopFront();

		/**
		 * Erases all elements from the container by calling the destructors on each of them.
		 * Afterwards Size() == 0.
		 */
		void Clear() noexcept;

		/**
		 * Erases all memory held by this container.
		 */
		void Empty() noexcept;
#pragma endregion

#pragma region Query		
		/**
		 * does an O(n) search for the first occurrence of the passed element.
		 *
		 * @param t				the element to query for
		 * @returns				the index of the first element found, or Size() if it was not found
		 */
		template<typename T>
		size_type IndexOf(const T& t) const;

		/**
		 * does an O(n) search for the first element satisfying the given predicate
		 *
		 * @param <T>			the template parameter for the predicate
		 * @param <Predicate>	the predicate type
		 * @param pred			the predicate
		 * @returns				the index of the first element that satisfies pred, or Size() if none was found
		 */
		template<typename T, std::predicate<T> Predicate>
		size_type IndexOf(Predicate pred) const;
#pragma endregion
		
#pragma region Memory
		/**
		 * @param <T>	the type of array to get
		 * @returns		a reference to the underlying array
		 *
		 * @throws InvalidTypeException		if this container does not store types of T
		 */
		template<typename T>
		constexpr Array<T>& GetArray() noexcept;

		/**
		 * @param <T>	the type of array to get
		 * @returns		a reference to the underlying array
		 *
		 * @throws InvalidTypeException		if this container does not store types of T
		 */
		template<typename T>
		constexpr const Array<T>& GetArray() const noexcept;
		
		/**
		 * Calls SetType<T>() then Reserve(capacity)
		 * O(n) where n is the current size of the container.
		 * Does nothing if newCapacity <= Capacity().
		 *
		 * @param <T>			the type to reserve space for
		 * @param capacity		the new capacity for the container.
		 *
		 * @throws std::length_error	if newCapacity > MaxSize()
		 * @throws std::bad_alloc		if memory allocation fails.
		 */
		template<typename T>
		void Reserve(size_t capacity);

		/**
		 * O(n) where n is the current size of the container.
		 * Does nothing if newCapacity <= Capacity().
		 *
		 * @param capacity		the new capacity for the container.
		 *
		 * @throws std::length_error	if newCapacity > MaxSize()
		 * @throws std::bad_alloc		if memory allocation fails.
		 */
		void Reserve(size_t capacity);

		/**
		 * O(n) where n = newSize - Size(), or n = newSize if the operation requires a Reserve().
		 * If the current Size() is greater than newSize, the container is reduced to its first newSize elements.
		 *
		 * @param newSize		The new size for the container.
		 * @param prototype		The value to initialize each new element with.
		 */
		template<typename T>
		void Resize(size_t capacity, const T& prototype = T());

		/**
		 * Reduces memory usage such that Size() == Capacity().
		 * O(n) where n = Size().
		 * Does nothing if IsFull()
		 */
		void ShrinkToFit();

		/**
		 * Reduces memory usage such that Capacity() == std::max(count, Size()).
		 * O(n) where n = Size().
		 * Does nothing if Capacity() already == std::max(count, Size()).
		 */
		void ShrinkToFit(size_t count);

		/**
		 * Reverses the order of the elements in the container.
		 * O(n)
		 */
		void Reverse() noexcept;
#pragma endregion

#pragma region Operators
		/**
		 * "ToString" operator.
		 * @param stream	The stream to pass the stringified container to.
		 * @param d			The container to stringify.
		 * @returns			The same stream for chaining.
		 */
		friend std::ostream& operator<<(std::ostream& stream, const VariantArray& v) noexcept
		{
			Util::StreamTo(stream, v.begin(), v.end());
			return stream;
		}

		/**
		 * @param a			lhs
		 * @param b			rhs
		 * @return true		if a == b
		 * @return false	if a != b
		 */
		[[nodiscard]] friend bool operator==(const VariantArray& a, const VariantArray& b)
		{
			return a.variants == b.variants;
		}

		/**
		 * @param a			lhs
		 * @param b			rhs
		 * @return true		if a != b
		 * @return false	if a == b
		 */
		[[nodiscard]] friend bool operator!=(const VariantArray& a, const VariantArray& b)
		{
			return !operator==(a, b);
		}
#pragma endregion

#pragma region Helpers
	public:		
		/**
		 * Parses Ts for a type that T is convertible to.
		 * If T is in Ts, just use T.
		 *
		 * @returns		a valid in_place_type or otherwise static_asserts 
		 */
		template<typename T>
		static constexpr auto InPlaceType() noexcept;
		
		/**
		 * Ensures that this container has the specified type before continuing.
		 * Will silently change the type if it can do so with no loss of data.
		 * 
		 * @param <T>						The type to check the already contained type against.
		 * @throws InvalidTypeException		If the type does not match and the container has data.
		 */
		template<typename T>
		void AssertSetType();
		
		/**
		 * Ensures that this container has the specified type before continuing.
		 * 
		 * Will silently change the type if it can do so with no loss of data.
		 * @param <T>						The type to check the already contained type against.
		 * @throws InvalidTypeException		If the type does not match and the container has data.
		 */
		template<typename T>
		void AssertType() const;

		/**
		 * Shorthand for invoking std::visit on the varaints.
		 * @param <Callable>	A callable type.
		 * @param callable		A callable type.
		 */
		template<typename Callable>
		constexpr auto Visit(Callable&& callable);

		/**
		 * Shorthand for invoking std::visit on the varaints.
		 * @param <Callable>	A callable type.
		 * @param callable		A callable type.
		 */
		template<typename Callable>
		constexpr auto Visit(Callable&& callable) const;
#pragma endregion
	};
}

#include "VariantArray.inl"
