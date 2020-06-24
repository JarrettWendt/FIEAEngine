// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "Macros.h"
#include "Memory.h"
#include "Util.h"

#include <algorithm>			// std::min/max
#include <compare>				// std::strong_ordering
#include <initializer_list>		// std::initializer/list
#include <iterator>				// std::random_access_iterator

#include <gsl/gsl>				// gsl::owner

// Some macros to save on typing.
// These get #undef-ed at the end of the .inl file.
// Also pretty convenient in case I ever decide I want to change/rename the template args.
// The only downside is that intellisense thinks my methods are unimplemented.
#define TEMPLATE template<typename T, Concept::ReserveStrategy ReserveStrategy>
#define ARRAY Array<T, ReserveStrategy>

namespace Library
{
	// TODO: Array<bool> can be specialized as a bit-array to save memory.
	
	/**
	 * A contiguous-memory dynamically resizing array.
	 * Internally uses std::vector and provides more convenience methods to it.
	 * Name chosen to disambiguate between vector the data container and vector the mathematical concept.
	 *
	 * @param <T>					The type for this container to store.
	 * @param <ReserveStrategy>		A callable type for determining a new capacity given the current memory usage.
	 */
	template<typename T, Concept::ReserveStrategy ReserveStrategy = Util::DefaultReserveStrategy>
	class Array final
	{
	public:
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reserve_strategy = ReserveStrategy;
		
	private:
		using internal_size_type = uint32_t;
		
		T* array{ nullptr };
		internal_size_type size{ 0 };
		internal_size_type capacity{ 0 };

	public:
#pragma region Special Members
		/**
		 * default ctor
		 * creates an empty container owning no memory
		 */
		Array() = default;

		/**
		 * explicit ctor
		 * 
		 * @param capacity		how many elements worth of space to reserve
		 */
		explicit Array(size_type capacity);

		/**
		 * explicit ctor
		 * 
		 * @param count			how many elements of space to reserve and construct
		 * @param prototype		prototypical value to be copy-constructed into each element
		 */
		Array(size_type count, const T& prototype);

		/**
		 * explicit ctor
		 * Fills the container with values read through the iterators.
		 *
		 * @param <It>			the iterator type
		 * @param first			the beginning iterator, inclusive
		 * @param last			the last iterator, exclusive
		 */
		template<std::forward_iterator It>
		Array(It first, It last);

		/**
		 * explicit ctor
		 * Fills the container with values read through the iterators.
		 *
		 * @param <It>			the iterator type
		 * @param first			the beginning iterator, inclusive
		 * @param last			the last iterator, exclusive
		 */
		template<std::random_access_iterator It>
		Array(It first, It last);

		/**
		 * typecast ctor which accepts any range
		 * will at most resize once
		 *
		 * @param range			the range to construct from
		 */
		template<Concept::RangeOf<T> Range>
		Array(const Range& range);

		/**
		 * typecast ctor which accepts any range
		 * will at most resize once
		 *
		 * @param range			the range to construct from
		 */
		template<Concept::RangeOf<T> Range>
		Array& operator=(const Range& range);

		/**
		 * initializer_list ctor
		 * 
		 * @param list		the initializer_list to construct this container from
		 */
		Array(std::initializer_list<T> list);

		/**
		 * initializer_list operator=
		 * 
		 * @param list		the initializer_list to assign this container to
		 * @return			this container after assignment
		 */
		Array& operator=(std::initializer_list<T> list);

		/**
		 * copy ctor
		 * 
		 * @param other		the container to copy
		 */
		Array(const Array& other);

		/**
		 * move ctor
		 *
		 * @param other		the container to move, will be made empty after this operation
		 */
		Array(Array&& other) noexcept;

		/**
		 * copy operator=
		 *
		 * @param other		the container to move, will be made empty after this operation
		 * @returns			this container after assignment
		 */
		Array& operator=(const Array& other);

		/**
		 * move operator=
		 *
		 * @param other		the container to move, will be made empty after this operation
		 * @returns			this container after assignment
		 */
		Array& operator=(Array&& other) noexcept;

		/**
		 * templated copy ctor accepting containers with other ReserveStrategies
		 * this container will keep its original reserve strategy
		 *
		 * @param other		the container to move, will be made empty after this operation
		 * @returns			this container after assignment
		 */
		template<Concept::ReserveStrategy OtherReserveStrategy>
		Array(const Array<T, OtherReserveStrategy>& other);

		/**
		 * templated move ctor accepting containers with other ReserveStrategies
		 * this container will keep its original reserve strategy
		 *
		 * @param other		the container to move, will be made empty after this operation
		 * @returns			this container after assignment
		 */
		template<Concept::ReserveStrategy OtherReserveStrategy>
		Array(Array<T, OtherReserveStrategy>&& other) noexcept;

		/**
		 * templated copy operator= accepting containers with other ReserveStrategies
		 * this container will keep its original reserve strategy
		 *
		 * @param other		the container to move, will be made empty after this operation
		 * @returns			this container after assignment
		 */
		template<Concept::ReserveStrategy OtherReserveStrategy>
		Array& operator=(const Array<T, OtherReserveStrategy>& other);

		/**
		 * templated move operator= accepting containers with other ReserveStrategies
		 * this container will keep its original reserve strategy
		 *
		 * @param other		the container to move, will be made empty after this operation
		 * @returns			this container after assignment
		 */
		template<Concept::ReserveStrategy OtherReserveStrategy>
		Array& operator=(Array<T, OtherReserveStrategy>&& other) noexcept;

		/**
		 * dtor
		 * frees all associated memory
		 */
		~Array();
#pragma endregion

#pragma region iterator		
		class iterator final
		{
			friend class Array;
			friend class const_iterator;

		private:
			T* data{ nullptr };
#ifdef _DEBUG
			Array* owner{ nullptr };
#endif
			
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = T;
			using difference_type = ptrdiff_t;
			using pointer = T*;
			using reference = T&;

		private:
			iterator(T* data, [[maybe_unused]] Array* owner) noexcept :
				data(data)
#ifdef _DEBUG
				,owner(owner)
#endif
			{}
			
		public:
			/**
			 * explicit constructor
			 *
			 * @param index		index for this iterator to start at
			 * @param owner		the container this iterator references
			 */
			iterator(const size_type index, Array& owner) noexcept :
				iterator(owner.array + index, &owner) {}

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
				return left.data - right.data;
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

			RANDOM_ITER_OPS(iterator)

		private:
			/**
			 * @asserts		this iterator is initialized.
			 */
			constexpr void AssertInitialized() const noexcept;
		};

		class const_iterator final
		{
			friend class Array;
			CONST_RANDOM_ACCESS_ITERATOR(const_iterator, iterator)

			/**
			 * explicit constructor
			 *
			 * @param index		index for this iterator to start at
			 * @param owner		the container this iterator references
			 */
			const_iterator(const size_t index, const Array& owner) noexcept :
				it{ index, const_cast<Array&>(owner) } {};
		};

		BEGIN_END(iterator, const_iterator, Array)
		MAKE_REVERSE_BEGIN_END
#pragma endregion

#pragma region Properties
		/**
		 * @returns		true if the container is empty, false otherwise
		 */
		[[nodiscard]] constexpr bool IsEmpty() const noexcept;

		/**
		 * @returns		true if the container is full, false otherwise
		 */
		[[nodiscard]] constexpr bool IsFull() const noexcept;

		/**
		 * @returns		How many elements are in the container.
		 */
		[[nodiscard]] constexpr size_type Size() const noexcept;

		/**
		 * @returns		The number of elements this container can hold without resizing.
		 */
		[[nodiscard]] constexpr size_type Capacity() const noexcept;

		/**
		 * @param it	an iterator
		 * @returns		the index represented by the iterator
		 */
		[[nodiscard]] constexpr size_type IndexOf(const_iterator it) const noexcept;
#pragma endregion

#pragma region Element Access
		/**
		 * Returns a reference to the given index with bounds checking.
		 * 
		 * @param index		position of the element to return
		 * @returns			reference to the requested element
		 *
		 * @throws std::out_of_range	if index >= Size()
		 */
		[[nodiscard]] T& At(size_type index);
		
		/**
		 * Returns a reference to the given index with bounds checking.
		 *
		 * @param index		position of the element to return
		 * @returns			reference to the requested element
		 *
		 * @throws std::out_of_range	if index >= Size()
		 */
		[[nodiscard]] const T& At(size_type index) const;

		/**
		 * Returns a reference to the given index with bounds checking.
		 *
		 * @param index		position of the element to return
		 * @returns			reference to the requested element
		 *
		 * @throws std::out_of_range	if index >= Size()
		 */
		[[nodiscard]] T& operator[](size_type index);

		/**
		 * Returns a reference to the given index with bounds checking.
		 *
		 * @param index		position of the element to return
		 * @returns			reference to the requested element
		 *
		 * @throws std::out_of_range	if index >= Size()
		 */
		[[nodiscard]] const T& operator[](size_type index) const;

		/**
		 * @returns		reference to the first element.
		 *
		 * @throws std::out_of_range	if the container is empty
		 */
		[[nodiscard]] T& Front();

		/**
		 * @returns		reference to the first element.
		 *
		 * @throws std::out_of_range	if the container is empty
		 */
		[[nodiscard]] const T& Front() const;

		/**
		 * @returns		reference to the last element.
		 *
		 * @throws std::out_of_range	if the container is empty
		 */
		[[nodiscard]] T& Back();

		/**
		 * @returns		reference to the last element.
		 *
		 * @throws std::out_of_range	if the container is empty
		 */
		[[nodiscard]] const T& Back() const;
#pragma endregion
		
#pragma region Insert
		/**
		 * @param index		where the new element will be construted
		 * @param t			value to insert
		 */
		void Insert(size_t index, const T& t);
		
		/**
		 * @param index		where the new element will be construted
		 * @param t			value to insert
		 */
		void Insert(size_t index, T&& t);

		/**
		 * @param index		where the new element will be construted
		 * @param count		how many copies of the value to insert
		 * @param t			value to insert
		 * @returns			iterator at the position of the last element inserted
		 */
		iterator Insert(size_t index, size_t count, const T& t);

		/**
		 * @param index		where the new element will be construted
		 * @param list		initializer_list of values to insert
		 * @returns			iterator at the position of the last element inserted
		 */
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
		void Insert(const_iterator it, const T& t);
		
		/**
		 * @param it		position of where to insert
		 * @param t			prototypical value to insert
		 *
		 * @throws std::invalid_argument	if the iterator does not belong to this container
		 */
		void Insert(const_iterator it, T&& t);

		/**
		 * @param it		initial position of where to insert
		 * @param count		how many copies of the prototype to insert
		 * @param t			prototypical value to insert
		 * @returns			iterator at the last element inserted
		 *
		 * @throws std::invalid_argument	if the iterator does not belong to this container
		 */
		iterator Insert(const_iterator it, size_t count, const T& t);


		/**
		 * @param it		initial position of where to insert
		 * @param list		initializer_list of values to insert
		 * @returns			iterator at the last element inserted
		 *
		 * @throws std::invalid_argument	if the iterator does not belong to this container
		 */
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
		template<typename... Args>
		reference Emplace(size_t index, Args&&... args);

		/**
		 * Inserts a new element into the container directly at index.
		 * The passed iterator is guaranteed to be valid and pointing at the position of the newly emplaced element after the operation.
		 *
		 * @param it		Position in the container to put this element.
		 * @param args		Arguments to forward to the constructor of the element.
		 *
		 * @throws std::out_of_range	if index > Size().
		 */
		template<typename... Args>
		reference Emplace(const_iterator it, Args&&... args);

		/**
		 * Appends a new element to the end of the container.
		 * O(1) most cases.
		 * O(n) if the operation requires a Resize().
		 * 
		 * @param args		Arguments to forward to the constructor of the element.
		 */
		template<typename... Args>
		reference EmplaceBack(Args&&... args);

		/**
		 * Appends a new element to the front of the container.
		 * O(1) most cases.
		 * O(n) if the operation requires a Resize().
		 *
		 * @param args		Arguments to forward to the constructor of the element.
		 */
		template<typename... Args>
		reference EmplaceFront(Args&&... args);
		
		/**
		 * Appends the element to the end of the container.
		 * O(1) most cases.
		 * O(n) if the operation requires a Resize().
		 * 
		 * @param t		The value of the element to append.
		 */
		void PushBack(const T& t);
		
		/**
		 * Appends the element to the end of the container.
		 * O(n) where n is Size().
		 * 
		 * @param t		The value of the element to append.
		 */
		void PushBack(T&& t);

		/**
		 * Appends the element to the front of the container.
		 * O(n) where n is Size().
		 * 
		 * @param t		The value of the element to prepend.
		 */
		void PushFront(const T& t);

		/**
		 * Appends the element to the front of the container.
		 * O(n) where n is Size().
		 * 
		 * @param t		The value of the element to prepend.
		 */
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
		template<std::predicate<T> Predicate>
		bool Remove(Predicate predicate);

		/**
		 * Removes the index specifid and shift values to the left over.
		 * O(n) where n is Size() - index;
		 * 
		 * @param index		The index to remove.
		 */
		void RemoveAt(size_t index);

		/**
		 * Removes all elements mathcing the specified value.
		 * O(n)
		 * 
		 * @param t		The value to remove all instances of.
		 * @returns		How many removals were performed.
		 */
		size_t RemoveAll(const T& t);

		/**
		 * Removes all elements matching the passed predicate.
		 * O(n)
		 * 
		 * @param predicate		The predicate to query with.
		 * @returns				How many elements were removed.
		 */
		template<std::predicate<T> Predicate>
		size_t RemoveAll(Predicate predicate);

		template<std::predicate<T> Predicate>
		friend size_t erase_if(Array& arr, const Predicate predicate)
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
		size_t Remove(size_t first, size_t last);

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
		 * Does an O(n) search for the first occurrence of the passed element.
		 * 
		 * @param t				the element to query for.
		 * @returns				an iterator at the position of the first element, or end() if it was not found.
		 */
		//iterator Find(const T& t);

		/**
		 * Does an O(n) search for the first occurrence of the passed element.
		 *
		 * @param t				the element to query for.
		 * @returns				an iterator at the position of the first element, or end() if it was not found.
		 */
		//const_iterator Find(const T& t) const;

		/**
		 * Does an O(n) search for the first occurrence of the passed element.
		 * 
		 * @param t				the element to query for.
		 * @returns				the index of the first element found, or Size() if it was not found.
		 */
		size_type IndexOf(const T& t) const;

		/**
		 * Does an O(n) search for the first element matching the passed predicate.
		 *
		 * @param predicate		the predicate to query with.
		 * @returns				the index of the first element found, or Size() if it was not found.
		 */
		template<std::predicate<T> Predicate>
		size_type IndexOf(Predicate predicate) const;

		/**
		 * Does an O(n) search for the first occurrence of the passed element.
		 * 
		 * @param t				The element to query for.
		 * @returns				Whether or not the element exists in the container. 
		 */
		//bool Contains(const T& t) const;
#pragma endregion
		
#pragma region Memory		
		/**
		 * Warning! Use at your own risk! Manipulating this memory can invalidate the state of the Arrray.
		 *
		 * @returns		pointer to the underlying data. nullptr if the container is empty.
		 */
		[[nodiscard]] T* Data() noexcept;

		/**
		 * Warning! Use at your own risk! Manipulating this memory can invalidate the state of the Arrray.
		 *
		 * @returns		pointer to the underlying data. nullptr if the container is empty.
		 */
		[[nodiscard]] const T* Data() const noexcept;

		/**
		 * Gets the underlying data and empties this container without deallocating any memory.
		 * Guaranteed to cause a memory leak if the caller discards this return value.
		 * Caller assumes all responsibility for managing this data.
		 * 
		 * @returns		pointer, size, capacity
		 */
		[[nodiscard]]
		std::tuple<gsl::owner<T*>, size_t, size_t> TakeData() noexcept;

		/**
		 * Force this container to take on the given memory.
		 * Will free all currently associated memory and destruct all objects.
		 * The container will manage the given data as if it had allocated it itself,
		 * possibly reallocating it in the event of an operation that would do such a thing.
		 * 
		 * @param data	pointer, size, capacity
		 */
		void SetData(std::tuple<T*, size_t, size_t> data) noexcept;

		/**
		 * Force this container to take on the given memory.
		 * Will free all currently associated memory and destruct all objects.
		 * The container will manage the given data as if it had allocated it itself,
		 * possibly reallocating it in the event of an operation that would do such a thing.
		 * 
		 * @param array		pointer to array of T
		 * @param size		how many elements array contains
		 * @param capacity	how many elements array can store 
		 */
		void SetData(T* array, size_t size, size_t capacity) noexcept;
		
		/**
		 * O(n) where n is the current size of the container.
		 * Does nothing if newCapacity <= Capacity().
		 * 
		 * @param newCapacity	The new capacity for the container.
		 *
		 * @throws std::length_error	if newCapacity > MaxSize()
		 * @throws std::bad_alloc		if memory allocation fails.	
		 */
		void Reserve(size_t newCapacity);

		/**
		 * O(n) where n = newSize - Size(), or n = newSize if the operation requires a Reserve().
		 * If the current Size() is greater than newSize, the container is reduced to its first newSize elements.
		 *
		 * @param newSize		The new size for the container.
		 * @param prototype		The value to initialize each new element with.
		 */
		void Resize(size_t newSize, const T& prototype = T());

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
		 * TODO: It might be that the STL expects swap to be lowercase and a non-member function (so a friend function will do).
		 * O(1)
		 * 
		 * @param <OtherReserveStrategy>	template parameter allowing swapping Arrays with different ReserveStrategies.
		 * @param other						container to exchange contents with.
		 */
		template<Concept::ReserveStrategy OtherReserveStrategy>
		void Swap(Array<T, OtherReserveStrategy>& other) noexcept;

		/**
		 * Reverses the order of the elements in the container.
		 * O(n)
		 */
		void Reverse() noexcept;
#pragma endregion

#pragma region Operators
		/**
		 * templated operator== to allow for comparing containers with different reserve strategies
		 *
		 * @param <OtherReserveStrategy>	reserve strategy for the other container
		 * @param left						lhs container
		 * @param right						rhs container
		 * @returns							whether or not the two containers are equal.
		 */
		template<Concept::ReserveStrategy OtherReserveStrategy>
		[[nodiscard]] friend bool operator==(const Array& left, const Array<T, OtherReserveStrategy>& right)
		{
			return &left == reinterpret_cast<const Array*>(&right) || left.Size() == right.Size() && std::equal(left.begin(), left.end(), right.begin(), right.end());
		}

		/**
		 * templated operator== to allow for comparing containers with different reserve strategies
		 *
		 * @param <OtherReserveStrategy>	reserve strategy for the other container
		 * @param left						lhs container
		 * @param right						rhs container
		 * @returns							whether or not the two containers are equal.
		 */
		template<Concept::ReserveStrategy OtherReserveStrategy>
		[[nodiscard]] friend bool operator!=(const Array& left, const Array<T, OtherReserveStrategy>& right)
		{
			return !operator==(left, right);
		}

		friend std::ostream& operator<<(std::ostream& stream, const ARRAY& array) noexcept
		{
			Util::StreamTo(stream, array.begin(), array.end());
			return stream;
		}
#pragma endregion

#pragma region Helpers
	private:
		/**
		 * assumes Capacity() is sufficient
		 *
		 * @param first		the beginning iterator to read from (inclusive)
		 * @param last		the ending iterator to read from (exclusive)
		 */
		template<std::forward_iterator It>
		void PushBackNoResize(It first, It last);
		
		/**
		 * Helper for methods that accept iterators.
		 *
		 * @param it		The iterator who's owner to compare against this.
		 *
		 * @asserts			owner == this
		 */
		void AssertOwner(iterator it) const;
		
		/**
		 * @param it		The iterator who's owner to compare against this.
		 *
		 * @asserts			owner == this
		 */
		void AssertOwner(const_iterator it) const;
		
		/**
		 * Calls the ReserveStrategy template argument and sanity checks its return value.
		 * @returns		A safe new Capacity() for this container.
		 */
		[[nodiscard]] size_type InvokeReserveStrategy() const noexcept;
		
		/**
		 * Calls destructor on all elements.
		 */
		void DestructAll() noexcept;

		/**
		 * Calls the destructor on all elements in range [first, last).
		 * 
		 * @param first		Index of first element to be deconstructed, inclusive.
		 * @param last		Index of last element to be decondstructed, exclusive.
		 */
		void DestructAll(const size_type first, const size_type last) noexcept;

		/**
		 * Shifts the internal array to the right starting at the passed index.
		 * Moves memory as if calling the destructor on elements before they're overwritten and after they're moved.
		 * 
		 * @param startIndex	The index to start the shifts at.
		 * @param shiftAmount	How much to shift by.
		 */
		void ShiftRight(size_type startIndex = 0, size_type shiftAmount = 1) noexcept;

		/**
		 * Shifts the internal array to the left starting at the passed index.
		 * Moves memory as if calling the destructor on elements before they're overwritten and after they're moved.
		 * 
		 * @param startIndex	The index to start the shifts at.
		 * @param shiftAmount	How much to shift by.
		 */
		void ShiftLeft(size_type startIndex = 0, size_type shiftAmount = 1) noexcept;
		
		/**
		 * Helper called from constructors.
		 * Assumes size is 0 and capacity is sufficient.
		 * 
		 * @param list		list to copy elements from.
		 */
		void Copy(std::initializer_list<T> list);

		/**
		 * Helper called from constructors.
		 * Assumes size is 0 and capacity is sufficient.
		 *
		 * @param list		list to copy elements from.
		 */
		template<typename OtherReserveStrategy>
		void Copy(const Array<T, OtherReserveStrategy>& other);

		/**
		 * Helper called from copy assignment operators.
		 *
		 * @param other		The array being copied.
		 * @returns			this after the copy.
		 */
		template<typename OtherReserveStrategy>
		[[nodiscard]] Array& CopyAssign(const Array<T, OtherReserveStrategy>& other);

		/**
		 * Helper called from move assignment operators.
		 *
		 * @param other		The array being moved.
		 * @returns			this after the move.
		 */
		template<typename OtherReserveStrategy>
		[[nodiscard]] Array& MoveAssign(Array<T, OtherReserveStrategy>&& other);

		/**
		 * Helper called from constructors.
		 * Sets this Array's state after std::move.
		 */
		void SetPostMoveState();
#pragma endregion
	};
}

#include "Array.inl"
