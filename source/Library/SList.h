// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "Macros.h"
#include "Util.h"
#include "Hash.h"

#include <algorithm>			// std::min/max
#include <initializer_list>		// std::initializer_list
#include <iterator>				// std::_Is_random_iter

// TODO: This container could be vastly optimized if Node were always guaranteed to be the size of cache lane.
// That would mean SList is made up of linked sub-arrays.
// This would all around decrease memory usage (since we have less next pointers) and reduce cache misses.
// That would require adding an index data member to the iterator to know what position in the sub-array it is at.
// That would also mean removals would be slower since objects have to be shifted.
// We could use dirty bits to improve the speed of removals, but then the empy positions need to be culled every now and then.
// It might be better to just avoid this problem by not allowing removals in the middle of the list.
// That would imply a different class though.

// SList, SList::Node, SList::iterator, and SList::const_iterator all need to be friends of HashMap for it to be able to emplace with no moves or copies.
#define FRIEND_HASHMAP template<typename TKey, typename TValue, Concept::Hasher<TKey> Hash, std::predicate<TKey, TKey> KeyEqual, Concept::ReserveStrategy ReserveStrategy> friend class HashMap;

namespace Library
{	
	template<typename T>
	class SList final
	{
		FRIEND_HASHMAP
		
	public:
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		
	private:
		struct Node final
		{
			FRIEND_HASHMAP
			
			// before_begin relies on this being the first data member.
			Node* next{ nullptr };
			value_type data;

			template<typename... Args>
			explicit Node(Node* next, Args&&... args);
			
			template<typename... Args>
			explicit Node(Args&&... args);
			
			Node() = default;
			~Node() = default;
			MOVE_COPY(Node, delete)
		};

		// before_begin relies on head being the first data member.
		Node* head{ nullptr };
		Node* tail{ nullptr };
		size_type size{ 0 };

	public:
#pragma region Special Members
		SList(std::initializer_list<T> list);
		SList& operator=(std::initializer_list<T> list);

		/**
		 * constructs this container from a forward iterator
		 * slightly less efficient than constructing from a random access iterator
		 *
		 * @param <It>		iterator type
		 * @param first		beginning iterator to read from (inclusive)
		 * @param last		ending iterator to read from (exclusive)
		 */
		template<std::forward_iterator It>
		SList(It first, It last);

		/**
		 * constructs this container from a random access iterator
		 * slightly more efficient than constructing from a forward iterator
		 *
		 * @param <It>		iterator type
		 * @param first		beginning iterator to read from (inclusive)
		 * @param last		ending iterator to read from (exclusive)
		 */
		template<std::random_access_iterator It>
		SList(It first, It last);

		/**
		 * typecast ctor which accepts any range
		 * will at most resize once
		 *
		 * @param range			the range to construct from
		 */
		template<Concept::RangeOf<T> Range>
		SList(const Range& range);

		/**
		 * typecast ctor which accepts any range
		 * will at most resize once
		 *
		 * @param range			the range to construct from
		 */
		template<Concept::RangeOf<T> Range>
		SList& operator=(const Range& range);

	private:
		/**
		 * private ctor to be invoked by other ctors
		 * for when the size can be determined from the iterators at compile time
		 * all this saves us is a size++ on every insert
		 *
		 * @param size		should be std::distance(first, last)
		 * @param first		beginning iterator to read from (inclusive)
		 * @param last		ending iterator to read from (exclusive)
		 */
		template<std::forward_iterator It>
		SList(size_type size, It first, It last);
		
	public:
		SList() = default;
		SList(const SList& other);
		SList(SList&& other) noexcept;
		SList& operator=(const SList& other);
		SList& operator=(SList&& other) noexcept;
		~SList();
#pragma endregion

#pragma region iterator
		class const_iterator;

		class iterator final
		{
			FRIEND_HASHMAP
			
			friend class SList;
			friend class const_iterator;

		private:
			// The compiler will not optimize away unused data members.
			// We're only using the owner pointer to assert when debugging.
			// We don't want this data member in release.
			// Trusting that the compiler will remove all other references to owner, such as in the ctor.
#ifdef _DEBUG
			const SList* owner{ nullptr };
#endif
			Node* node{ nullptr };

		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			using difference_type = ptrdiff_t;
			using pointer = T*;
			using reference = T&;

		private:
			iterator(Node* node, const SList* owner) noexcept;
		public:
			SPECIAL_MEMBERS(iterator, default)

			iterator operator++(int) noexcept;
			iterator& operator++() noexcept;
			reference operator*() const;
			pointer operator->() const;
			bool operator==(iterator other) const noexcept;
			bool operator!=(iterator other) const noexcept;
			operator bool() const noexcept;
			bool operator!() const noexcept;
			bool IsAtEnd() const noexcept;
		private:
			void AssertInitialized() const noexcept;
		};

		class const_iterator final
		{
			FRIEND_HASHMAP
			
			friend class SList;
			CONST_FORWARD_ITERATOR(const_iterator, iterator)
			operator bool() const noexcept;
			bool operator!() const noexcept;
			bool IsAtEnd() const noexcept;
		};

		BEGIN_END(iterator, const_iterator, SList)

		/**
		 * Gets the iterator preceding begin()
		 * Dereferencing the before_begin iterator is undefined.
		 * Pairs well with InsertAfter an RemoveAfter
		 * 
		 * @returns		the iterator preceding begin()
		 */
		iterator before_begin() noexcept;

		/**
		 * Gets the iterator preceding begin()
		 * Dereferencing the before_begin iterator is undefined.
		 * Pairs well with InsertAfter an RemoveAfter
		 *
		 * @returns		the iterator preceding begin()
		 */
		const_iterator before_begin() const noexcept;

		/**
		 * Gets the iterator preceding begin()
		 * Dereferencing the before_begin iterator is undefined.
		 * Pairs well with InsertAfter an RemoveAfter
		 *
		 * @returns		the iterator preceding begin()
		 */
		const_iterator cbefore_begin() const noexcept;

		/**
		 * Gets the iterator preceding end()
		 * Pairs well with InsertAfter an RemoveAfter
		 *
		 * @returns		the iterator preceding end()
		 */
		iterator before_end() noexcept;

		/**
		 * Gets the iterator preceding end()
		 * Pairs well with InsertAfter an RemoveAfter
		 *
		 * @returns		the iterator preceding end()
		 */
		const_iterator before_end() const noexcept;

		/**
		 * Gets the iterator preceding end()
		 * Pairs well with InsertAfter an RemoveAfter
		 *
		 * @returns		the iterator preceding end()
		 */
		const_iterator cbefore_end() const noexcept;
#pragma endregion

#pragma region Properties
		/**
		 * @returns		true if the container is empty, false otherwise
		 */
		[[nodiscard]] constexpr bool IsEmpty() const noexcept;

		/**
		 * @returns		How many elements are in the container.
		 */
		[[nodiscard]] constexpr size_t Size() const noexcept;
#pragma endregion

#pragma region Element Access
		/**
		 * @returns		reference to the first element.
		 *
		 * @throws std::out_of_range	if the container is empty
		 */
		[[nodiscard]] reference Front();
		
		/**
		 * @returns		reference to the first element.
		 *
		 * @throws std::out_of_range	if the container is empty
		 */
		[[nodiscard]] const_reference Front() const;

		/**
		 * @returns		reference to the last element.
		 *
		 * @throws std::out_of_range	if the container is empty
		 */
		[[nodiscard]] reference Back();
		
		/**
		 * @returns		reference to the last element.
		 *
		 * @throws std::out_of_range	if the container is empty
		 */
		[[nodiscard]] const_reference Back() const;

		/**
		 * Iterates through the container to find the value at the specified index.
		 * O(n)
		 * 
		 * @param pos	the position to get
		 * @returns		the value at that position
		 *
		 * @throws std::out_of_range	if pos >= Size()
		 */
		[[nodiscard]] reference At(size_type pos);

		/**
		 * Iterates through the container to find the value at the specified index.
		 * O(n)
		 *
		 * @param pos	the position to get
		 * @returns		the value at that position
		 *
		 * @throws std::out_of_range	if pos >= Size()
		 */
		[[nodiscard]] const_reference At(size_type pos) const;
#pragma endregion
		
#pragma region Insert
		/**
		 * O(n) where n is count
		 * If end() is passed this performs the same operation as PushBack().
		 * does not invalidate any iterators or references
		 *
		 * @param pos			where to insert
		 * @param t				value to insert
		 * @returns				iterator at the newly inserted element
		 *
		 * @asserts				the iterator belongs to this container
		 */
		iterator InsertAfter(const_iterator pos, const T& t);

		/**
		 * O(n) where n is count
		 * If end() is passed this performs the same operation as PushBack().
		 * does not invalidate any iterators or references
		 *
		 * @param pos			where to insert
		 * @param t				value to insert
		 * @returns				iterator at the newly inserted element
		 *
		 * @asserts				the iterator belongs to this container
		 */
		iterator InsertAfter(const_iterator pos, T&& t);

		/**
		 * O(n) where n is count
		 * If end() is passed this appends to the container.
		 * does not invalidate any iterators or references
		 * 
		 * @param pos			where to insert
		 * @param count			how many values to insert
		 * @param prototype		prototypical value to insert
		 * @returns				iterator at the first newly inserted element
		 *
		 * @asserts				the iterator belongs to this container
		 */
		iterator InsertAfter(const_iterator pos, size_type count, const T& prototype);

		/**
		 * O(n) where in is the size of list
		 * If end() is passed this appends to the container.
		 * does not invalidate any iterators or references
		 * 
		 * @param pos		where to insert
		 * @param list		list to read values from
		 *
		 * @asserts			the iterator belongs to this container
		 */
		iterator InsertAfter(const_iterator pos, std::initializer_list<T> list);

		/**
		 * Insert elements after the position specified by the passed iterator.
		 * Reads values in the range of [first, last).
		 * If end() is passed this appends to the container.
		 * does not invalidate any iterators or references
		 *
		 * @param <It>		the type of iterator to read from
		 * @param pos		iterator at which content will be inserted
		 * @param first		beginning iterator to read values from
		 * @param last		ending iterator to read values from
		 * @returns			iterator to the last element inserted
		 *
		 * @asserts			the iterator belongs to this container
		 */
		template<std::forward_iterator It>
		iterator InsertAfter(const_iterator pos, It first, It last);
		
		/**
		 * Inserts a value in the list after the position referenced by the passed iterator.
		 * If end() is passed this performs the same operation as PushBack().
		 * does not invalidate any iterators or references
		 *
		 * @param pos	where to emplace
		 * @param args	the arguments to be forwarded to the constructor
		 * @returns		iterator at the new element
		 *
		 * @asserts		the iterator belongs to this container
		 */
		template<typename... Args>
		iterator EmplaceAfter(const_iterator pos, Args&&... args);

		/**
		 * Appends a new element to the end of the container.
		 * O(1)
		 *
		 * @param args		Arguments to forward to the constructor of the element.
		 */
		template<typename... Args>
		reference EmplaceBack(Args&&... args);

		/**
		 * Appends a new element to the front of the container.
		 * O(1)
		 *
		 * @param args		Arguments to forward to the constructor of the element.
		 */
		template<typename... Args>
		reference EmplaceFront(Args&&... args);

		/**
		 * Appends the element to the end of the container.
		 * O(1)
		 *
		 * @param t		The value of the element to append.
		 */
		void PushBack(const T& t);

		/**
		 * Appends the element to the end of the container.
		 * O(1)
		 *
		 * @param t		The value of the element to append.
		 */
		void PushBack(T&& t);

		/**
		 * Appends the element to the front of the container.
		 * O(1)
		 *
		 * @param t		The value of the element to prepend.
		 */
		void PushFront(const T& t);

		/**
		 * Appends the element to the front of the container.
		 * O(1)
		 *
		 * @param t		The value of the element to prepend.
		 */
		void PushFront(T&& t);

		/**
		 * Appends a prototypical value to the end of the container
		 * O(n) where n = count
		 * 
		 * @param count			how many values to append
		 * @param prototype		what to append
		 */
		void Append(size_type count, const T& prototype = T());

		/**
		 * appends an initializer_list to the end of the container
		 * O(n) where n = list.size()
		 * 
		 * @param list		the list of values to append
		 */
		void Append(std::initializer_list<T> list);

		/**
		 * appends the values from another list to the end of the container
		 * O(n) where n = list.size()
		 *
		 * @param list		the list of values to append
		 */
		void Append(const SList<T>& list);

		/**
		 * Appends a range of data to the end of the list: [first, last)
		 * O(n) where n = std::distance(first, last)
		 *
		 * @param <It>		the iterator type
		 * @param first		the beginning iterator to read from (inclusive)
		 * @param last		the ending iterator to read from (exclusive)
		 */
		template<std::forward_iterator It>
		void Append(It first, It last);
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
		 * @returns		iterator at the position after where t was, or end() if nothing was removed
		 */
		iterator Remove(const T& t);
		
		/**
		 * Removes the first instance of the passed value from the container.
		 * Will only return false if the value did not exist in the container.
		 * A return value of true does not imply that no duplicate values do not still exist in the container.
		 * All elements after the removed one get shifted to the left in memory.
		 * O(n)
		 *
		 * @param t		The value to remove.
		 * @returns		iterator at the position after where t was, or end() if nothing was removed
		 */
		iterator Remove(T&& t);

		/**
		 * Removes the first element matching the passed predicate.
		 * Will only return false if the value did not exist in the container.
		 * A return value of true does not imply that no duplicate values do not still exist in the container.
		 * All elements after the removed one get shifted to the left in memory.
		 * O(n)
		 *
		 * @param predicate		The predicate to query with.
		 * @returns				iterator at the element proceeding the one removed, or end if nothing was removed
		 */
		template<std::predicate<T> Predicate>
		iterator Remove(Predicate predicate);

		/**
		 * Removes the range [first, last) elements between the two passed indices.
		 * If last is beyond Size() then all elements from first to the end will be removed safely.
		 * Does nothing if the indices are both beyond Size().
		 * Will absolutely invalidate iterators and references.
		 * O(n) where n = std::min(last, Size())
		 *
		 * @param first		index of the first value to remove, inclusive.
		 * @param last		index of the last value to remove, exclusive.
		 * @returns			The number of elements removed, equal to first - last unless last > Size().
		 *
		 * @throws std::invalid_argument	if first > last.
		 */
		iterator Remove(size_type first, size_type last);

		/**
		 * Removes the range [first, last) elements between the two passed iterators.
		 * Will absolutely invalidate iterators and references.
		 * O(n) where n = last - first
		 *
		 * @param first		iterator at the first element to remove, inclusive
		 * @param last		iterator at the last element to remove, exclusive
		 * @returns			last
		 *
		 * @asserts			both iterators belong to this container
		 */
		iterator Remove(const_iterator first, const_iterator last);

		/**
		 * Removes the value at the specified position.
		 * The passed iterator will reference the following value after this operation.
		 * Will absolutely invalidate iterators and references.
		 * O(1)
		 *
		 * @param pos	the position of the value to remove
		 * @returns		an iterator at the next position
		 *
		 * @asserts		the iterator belongs to this container
		 */
		iterator RemoveAt(const_iterator pos);

		/**
		 * Removes the value following the specified position.
		 * Not iterators or references are invalidated except for ones at the next element.
		 * If Size() <= 1 this operation does nothing.
		 * O(1)
		 *
		 * @param pos	the position of the value to remove
		 * @returns		an iterator following the erased one, or end() if no such element exists.
		 *
		 * @asserts		the iterator belongs to this container
		 */
		iterator RemoveAfter(const_iterator pos);

		/**
		 * Removes the range (first, last) elements between the two passed iterators.
		 * Will only invalidate iterators in the range (first, last).
		 * first and last itself will not be changed.
		 * O(n) where n = last - first
		 *
		 * @param first		iterator at the first element to remove, exclusive
		 * @param last		iterator at the last element to remove, exclusive
		 * @returns			last
		 *
		 * @asserts			both iterators belong to this container
		 */
		iterator RemoveAfter(const_iterator first, const_iterator last);

		/**
		 * Removes all values following the specified position.
		 * Will only invalidate iterators and references following pos.
		 * The passed iterator will reference the last element after this operation.
		 * If Size() <= 1 this operation does nothing.
		 * O(n) where n = Size() - pos
		 *
		 * @param pos	the position of the value to remove
		 *
		 * @asserts		the iterator belongs to this container
		 */
		void RemoveAllAfter(const_iterator pos);

		/**
		 * Removes all elements mathcing the specified value.
		 * O(n)
		 *
		 * @param t		The value to remove all instances of.
		 * @returns		How many removals were performed.
		 */
		size_type RemoveAll(const T& t);

		/**
		 * Removes all elements mathcing the specified value.
		 * O(n)
		 *
		 * @param t		The value to remove all instances of.
		 * @returns		How many removals were performed.
		 */
		size_type RemoveAll(T&& t);

		/**
		 * Removes all elements matching the passed predicate.
		 * O(n)
		 *
		 * @param predicate		The predicate to query with.
		 * @returns				How many elements were removed.
		 */
		template<std::predicate<T> Predicate>
		size_type RemoveAll(Predicate predicate);
		
		friend size_t erase(SList& list, const T& t)
		{
			return list.RemoveAll(t);
		}

		template<std::predicate<T> Predicate>
		friend size_t erase_if(SList& list, const Predicate predicate)
		{
			return list.RemoveAll(predicate);
		}

		/**
		 * O(1)
		 * Removes the first element of the container.
		 * Does nothing if the container is empty.
		 */
		void PopFront();
		
		/**
		 * O(1)
		 * Removes the last element of the container.
		 * Does nothing if the container is empty.
		 */
		void PopBack();

		/**
		 * Erases all elements from the container by calling the destructors on each of them.
		 * Afterwards Size() == 0.
		 */
		void Clear();
#pragma endregion

#pragma region Query
		/**
		 * Does an O(n) search for the element preceeding the first occurrence of the passed element.
		 * Pairs well with InsertAfter and RemoveAfter.
		 *
		 * @param t				the element to query for.
		 * @returns				an iterator at the position of the first element, or before_begin() if it was not found.
		 */
		[[nodiscard]] iterator FindPrev(const T& t);

		/**
		 * Does an O(n) search for the element preceeding the first occurrence of the passed element.
		 * Pairs well with InsertAfter and RemoveAfter.
		 *
		 * @param t				the element to query for.
		 * @returns				an iterator at the position of the first element, or before_begin() if it was not found.
		 */
		[[nodiscard]] const_iterator FindPrev(const T& t) const;

		/**
		 * Does an O(n) search for the element preceeding the first occurrence of the passed element.
		 * Pairs well with InsertAfter and RemoveAfter.
		 *
		 * @param t				the element to query for.
		 * @returns				an iterator at the position of the first element, or before_begin() if it was not found.
		 */
		[[nodiscard]] iterator FindPrev(T&& t);

		/**
		 * Does an O(n) search for the element preceeding the first occurrence of the passed element.
		 * Pairs well with InsertAfter and RemoveAfter.
		 *
		 * @param t				the element to query for.
		 * @returns				an iterator at the position of the first element, or before_begin() if it was not found.
		 */
		[[nodiscard]] const_iterator FindPrev(T&& t) const;

		/**
		 * Does an O(n) search for the element preceeding the first element matching the passed predicate.
		 * Pairs well with InsertAfter and RemoveAfter.
		 *
		 * @param predicate		the predicate to query with
		 * @returns				an iterator at the position of the first match, or before_begin() if it was not found.
		 */
		template<std::predicate<T> Predicate>
		[[nodiscard]] iterator FindPrev(Predicate predicate);

		/**
		 * Does an O(n) search for the element preceeding the first element matching the passed predicate.
		 * Pairs well with InsertAfter and RemoveAfter.
		 *
		 * @param predicate		the predicate to query with
		 * @returns				an iterator at the position of the first match, or before_begin() if it was not found.
		 */
		template<std::predicate<T> Predicate>
		[[nodiscard]] const_iterator FindPrev(Predicate predicate) const;
#pragma endregion

#pragma region Memory
		/**
		 * Resizes the container to contain count elements.
		 * 
		 * @param count						how many elements the container should hold
		 *
		 * @throws std::invalid_argument	if count > Size()
		 */
		void ShrinkTo(size_type count);

		/**
		 * Resizes the container to contain count elements.
		 * Will shrink the container if count < Size().
		 * 
		 * @param count			the new size for the container
		 * @param prototype		a prototypical value to initialize new elements with
		 */
		void Resize(size_type count, const T& prototype = T());

		/**
		 * Swaps the contents of this list with the other list.
		 * TODO: It might be that the STL expects swap to be lowercase and a non-member function (so a friend function will do).
		 * O(1)
		 * 
		 * @param other		the list to swap contents with 
		 */
		void Swap(SList& other) noexcept;

		/**
		 * Reverses the order of the elements in the container.
		 * No references or iterators become invalidated.
		 * O(n)
		 */
		void Reverse() noexcept;
		
		/**
		 * Does not copy any elements.
		 * Implements the Merge-Sort algorithm.
		 * O(nlogn)
		 *
		 * @param less		the comparator to use
		 */
		template<std::predicate<T, T> LessThan>
		void Sort(LessThan less);

		/**
		 * Does not copy any elements.
		 * Implements the Merge-Sort algorithm.
		 * Uses std::less<T> to compare elements.
		 * O(nlogn)
		 */
		void Sort();

		/**
		 * Merges another sorted list into this one.
		 * Assumes both containers are already sorted.
		 * The other list will be empty after this operation.
		 * No elements are copied.
		 * O(n) where n is the total number of elements.
		 *
		 * @param other			the list to merge into this one.
		 * @param less			the comparator to use for this operation.
		 */
		template<std::predicate<T, T> LessThan>
		void Merge(SList<T>& other, LessThan less);

		/**
		 * Merges another sorted list into this one.
		 * Assumes both containers are already sorted.
		 * The other list will be empty after this operation.
		 * No elements are copied.
		 * Uses std::less<T> to compare elements.
		 * O(n) where n is the total number of elements.
		 *
		 * @param other			the list to merge into this one.
		 */
		void Merge(SList<T>& other);

		/**
		 * Merges another sorted list into this one.
		 * Assumes both containers are already sorted.
		 * No elements are copied.
		 * O(n) where n is the total number of elements.
		 *
		 * @param other			the list to merge into this one.
		 * @param less			the comparator to use for this operation.
		 */
		template<std::predicate<T, T> LessThan>
		void Merge(SList<T>&& other, LessThan less);

		/**
		 * Merges another sorted list into this one.
		 * Assumes both containers are already sorted.
		 * No elements are copied.
		 * Uses std::less<T> to compare elements.
		 * O(n) where n is the total number of elements.
		 *
		 * @param other			the list to merge into this one.
		 */
		void Merge(SList<T>&& other);
#pragma endregion
		
#pragma region Operators
		/**
		 * O(n)
		 * 
		 * @param other		the container to compare this one against
		 * @return true		if all elements are the same in both containers
		 * @return false	otherwise
		 */
		[[nodiscard]] bool operator==(const SList<T>& other) const;

		/**
		 * O(n)
		 * 
		 * @param other		the container to compare this one against
		 * @return true		if at least 1 element is different in these two containers
		 * @return false	otherwise
		 */
		[[nodiscard]] bool operator!=(const SList<T>& other) const;

		/**
		 * "ToString" operator
		 * 
		 * @param stream 
		 * @param list 
		 * @return 
		 */
		friend std::ostream& operator<<(std::ostream& stream, const SList& list) noexcept
		{
			Util::StreamTo(stream, list.begin(), list.end());
			return stream;
		}
#pragma endregion

#pragma region Helpers
	private:
		/**
		 * assumes node is valid
		 * 
		 * @param pos		where to insert
		 * @param node		node to insert
		 * @returns			iterator to newly inserted element
		 */
		iterator InsertAfter(const_iterator pos, Node* node);
		
		/**
		 * @param node		node to insert at the front of the list
		 */
		void PushFront(Node* node) noexcept;
		
		/**
		 * @returns		iterator at the tail
		 */
		[[nodiscard]] iterator TailIt() noexcept;
		
		/**
		 * Traverses the list to find the node right before the tail.
		 * O(n)
		 * 
		 * @returns		the second to last node, or nullptr if there is only one item in the list
		 */
		[[nodiscard]] Node* GetPenultimate() const noexcept;
		
		/**
		 * Traverses the list to find the node right before the passed node.
		 * O(n)
		 *
		 * @returns		the node just before the passed one, or nullptr of no such node exists
		 */
		[[nodiscard]] Node* GetPrev(const Node* node) const noexcept;

		/**
		 * O(n / 2)
		 * @returns		iterator at the middle node (or the first of the two if Size() is even)
		 */
		iterator GetMiddle() const noexcept;

		/**
		 * Merges the nodes of two lists.
		 * Assumes both pointers are valid.
		 * Assumes a <= b
		 * Assumes h1->data < h2->data.
		 * O(nlogn)
		 * 
		 * @param a			head of the first list
		 * @param b			head of the second list
		 * @param less		comparator to use
		 * @returns			the head node of the merged list
		 */
		template<std::predicate<T, T> LessThan>
		static Node* Merge(Node* a, Node* b, LessThan less);

		/**
		 * Deletes this node and all of its children.
		 * 
		 * @param node		the head node to delete
		 * @return			h
		 * 
		 * ow many nodes were deleted
		 */
		static size_type Delete(Node* node);
		
		/**
		 * Deletes this node and all of its children.
		 *
		 * @param node		the head node to delete
		 * @return			h
		 *
		 * ow many nodes were deleted
		 */
		static size_type DeleteUntil(Node* node, const Node* until);
		
		/**
		 * Helper for methods that accept iterators.
		 *
		 * @param it		The iterator who's owner to compare against this.
		 *
		 * @asserts			if owner == this
		 */
		void AssertOwner(iterator it) const;

		/**
		 * Helper for methods that accept iterators.
		 *
		 * @param it		The iterator who's owner to compare against this.
		 *
		 * @asserts			if owner == this
		 */
		void AssertOwner(const_iterator it) const;

		/**
		 * Helper for Front() and Back().
		 *
		 * @throws std::out_of_range	if IsEmpty()
		 */
		void ThrowEmpty() const;

		/**
		 * @param index		the index to compare against the size
		 *
		 * @throws std::invalid_argument	if index >= Size()
		 */
		void ThrowIndex(size_type index) const;
		
		/**
		 * Copies nodes from the other list.
		 * Does not free any memory.
		 * Does not change size.
		 * 
		 * @param other		the list to copy
		 */
		void Copy(const SList& other);

		/**
		 * Helper called from constructors.
		 * Sets this Array's state after std::move.
		 */
		void SetPostMoveState() noexcept;
#pragma endregion
	};
}

#undef FRIEND_HASHMAP
#include "SList.inl"
