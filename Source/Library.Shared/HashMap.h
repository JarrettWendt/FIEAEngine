#pragma once

#include "Array.h"
#include "Concept.h"
#include "Hash.h"
#include "Macros.h"
#include "SList.h"
#include "Util.h"

#include <functional>			// std::equal_to
#include <iterator>				// std::_Is_random_iter

#include <sstream>

// Some macros to save on typing.
// These get #undef-ed at the end of the .inl file.
// Also pretty convenient in case I ever decide I want to change/rename the template args.
// The only downside is that intellisense thinks my methods are unimplemented.
#define TEMPLATE template<typename TKey, typename TValue, Concept::Hasher<TKey> Hash, std::predicate<TKey, TKey> KeyEqual, Concept::ReserveStrategy ReserveStrategy>
#define HASHMAP HashMap<TKey, TValue, Hash, KeyEqual, ReserveStrategy>
#define OTHER_TEMPLATE template<Concept::Hasher<TKey> OtherHash, std::predicate<TKey, TKey> OtherKeyEqual, Concept::ReserveStrategy OtherReserveStrategy>
#define OTHER_HASHMAP HashMap<TKey, TValue, OtherHash, OtherKeyEqual, OtherReserveStrategy>

namespace Library
{
	/**
	 * A type representing a pair of key and value.
	 * Each element in this container is stored as one of these.
	 * The key is never modified, or else it would be impossible to find after modification.
	 *
	 * The reason this is a custom struct rather than just an alias to std::pair is for 2 reasons:
	 * - the vocabulary of saying .key and .value is nicer than .first and .last
	 * - std::pair::operator== uses operator== for both elements. HashMap should only require operator== for TKey, not TValue.
	 */
	template<typename TKey, typename TValue, std::predicate<TKey, TKey> KeyEqual = std::equal_to<TKey>>
	struct KeyValuePair final
	{
		using key_type = TKey;
		using value_type = TValue;

		const key_type key;
		value_type value;

		/**
		 * Only compares the key, not the value.
		 *
		 * @param other		the KeyValuePair to compare this against.
		 * @return true		if the keys are the same
		 * @return false	if the keys are not the same
		 */
		bool operator==(const KeyValuePair& other) const;

		/**
		 * Only compares the key, not the value.
		 *
		 * @param other		the KeyValuePair to compare this against.
		 * @return true		if the keys are not the same
		 * @return false	if the keys are the same
		 */
		bool operator!=(const KeyValuePair& other) const;

		/**
		 * Converts a KeyValuePair to a string.
		 * Requires both key and value have a to_string
		 *
		 * @param kvp	the KeyValuPair to stringify
		 * @returns		the string representation of this KeyValuePair
		 */
		[[nodiscard]] friend std::string to_string(const KeyValuePair& kvp)
		{
			std::stringstream stream;
			stream << "{ " << std::to_string(kvp.key) << ", ";
			stream << std::to_string(kvp.value) << " }";
			return stream.str();
		}

		/**
		 * "to_string" operator for KeyValuePair
		 * calls to_string
		 *
		 * @param stream	the stream to stream to
		 * @param kvp		a KeyValuePair
		 * @returns			the same stream
		 */
		friend std::ostream& operator<<(std::ostream& stream, const KeyValuePair& kvp) noexcept
		{
			return stream << to_string(kvp);
		}
	};
	
	template<typename TKey, typename TValue, Concept::Hasher<TKey> Hash = Hash<TKey>, std::predicate<TKey, TKey> KeyEqual = std::equal_to<TKey>, Concept::ReserveStrategy ReserveStrategy = Util::PrimeReserveStrategy>
	class HashMap final
	{
	public:
		using key_type = TKey;
		using mapped_type = TValue;
		using value_type = KeyValuePair<TKey, TValue, KeyEqual>;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using hasher = Hash;
		using key_equal = KeyEqual;
		using reserve_strategy = ReserveStrategy;
		using reference = value_type&;
		using cosnt_reference = const reference;
		using pointer = value_type*;
		using const_pointer = const pointer;

	private:
		using ChainType = SList<value_type>;
		using BucketType = Array<ChainType>;
		using ChainIt = typename ChainType::iterator;
		using BucketIt = typename BucketType::iterator;

		BucketType buckets{ 1, ChainType() };
		size_type size{ 0 };

	public:
#pragma region Special Members
		/**
		 * typecast ctor which takes a range of KeyValuePairs
		 * will cause at most 1 resize
		 * BucketCount() will be std::distance(range.begin(), range.end())
		 *
		 * @param <Range>	the range type
		 * @param range		the range
		 */
		template<Concept::RangeOf<value_type> Range>
		HashMap(const Range& range) :
			HashMap(Util::GetSize(range), range) {}

		/**
		 * ctor which takes a range of KeyValuePairs
		 * will cause at most 1 resize
		 *
		 * @param <Range>	the range type
		 * @param range		the range
		 * @param count		the desired BucketCount()
		 */
		template<Concept::RangeOf<value_type> Range>
		HashMap(const size_type count, const Range& range) :
			HashMap(count, range.begin(), range.end()) {}

		/**
		 * assignment operator which takes a range of KeyValuePairs
		 * will cause at most 1 resize
		 *
		 * @param <Range>	the range type
		 * @param range		the range
		 */
		template<Concept::RangeOf<value_type> Range>
		HashMap& operator=(const Range& range)
		{
			Clear();
			if constexpr (Concept::HasSize<Range>)
			{
				Resize(Util::GetSize(range));
				InsertNoResize(range.begin(), range.end());
			}
			else
			{
				Insert(range.begin(), range.end());
			}
			return *this;
		}

		/**
		 * Constructs a HashMap from the range defined by [first, last)
		 * BucketCount() will be defined by std::distance(first, last)
		 *
		 * @param <It>		the iterator type
		 * @param first		the beginning iterator (inclusive)
		 * @param last		the ending iterator (exclusive)
		 */
		template<std::forward_iterator It>
		HashMap(It first, It last);

		/**
		 * Constructs a HashMap from the range defined by [first, last)
		 *
		 * @param <It>		the iterator type
		 * @param first		the beginning iterator (inclusive)
		 * @param last		the ending iterator (exclusive)
		 * @param count		the desired BucketCount()
		 */
		template<std::forward_iterator It>
		HashMap(size_type count, It first, It last);

		/**
		 * typecast ctor which takes an initializer_list of KeyValuePairs
		 * will cause at most 1 resize
		 * 
		 * @param list		the list
		 */
		HashMap(std::initializer_list<value_type> list);

		/**
		 * assignment operator which takes an initializer_list of KeyValuePairs
		 * will cause at most 1 resize
		 *
		 * @param list		the list
		 */
		HashMap& operator=(std::initializer_list<value_type> list);

		/**
		 * typecast ctor which takes an initializer_list of KeyValuePairs
		 * will cause at most 1 resize
		 * 
		 * @param count 
		 * @param list 
		 */
		HashMap(size_type count, std::initializer_list<value_type> list);

		OTHER_TEMPLATE
		HashMap(const OTHER_HASHMAP& other);

		OTHER_TEMPLATE
		HashMap(OTHER_HASHMAP&& other);

		OTHER_TEMPLATE
		HashMap& operator=(const OTHER_HASHMAP& other);

		OTHER_TEMPLATE
		HashMap& operator=(OTHER_HASHMAP&& other);
		
		/**
		 * @param count		initial BucketCount() to give this container
		 *
		 * @asserts			count > 0
		 */
		explicit HashMap(size_type count);

		HashMap() = default;
		~HashMap() = default;
		HashMap(HashMap&& other) noexcept;
		HashMap& operator=(HashMap&& other) noexcept;

		COPY_SEMANTICS(HashMap, default)
#pragma endregion

#pragma region iterator
		class const_iterator;

		class iterator final
		{
			friend class HashMap;
			friend class const_iterator;

		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = KeyValuePair<TKey, TValue>;
			using difference_type = ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;

		private:
			const HashMap* owner{ nullptr };
			BucketIt bucketIt{};
			ChainIt chainIt{};

			iterator(BucketIt bucketIt, const HashMap& owner) noexcept;
			iterator(BucketIt bucketIt, ChainIt chainIt, const HashMap& owner) noexcept;

		public:
			SPECIAL_MEMBERS(iterator, default)

#pragma region Operators
			/**
			 * Post-increment
			 * Does nothing if alrady at end().
			 * O(1)
			 *
			 * @returns			the iterator before the increment
			 *
			 * @asserts			this iterator is initialized
			 */
			iterator operator++(int) noexcept;

			/**
			 * Pre-increment
			 * Does nothing if alrady at end().
			 * O(1)
			 *
			 * @returns			the iterator after the increment
			 *
			 * @asserts			this iterator is initialized
			 */
			iterator& operator++() noexcept;
			
			/**
			 * @returns		The value of the index the iterator is at.
			 *
			 * @asserts		this iterator is initialized and not at end().
			 */
			[[nodiscard]] reference operator*() const;

			/**
			 * @returns		The value of the index the iterator is at.
			 *
			 * @asserts		this iterator is initialized and not at end().
			 */
			[[nodiscard]] pointer operator->() const;
			
			/**
			 * @param other		the iterator to compare against
			 * @returns			whether or not they are equivalent
			 */
			[[nodiscard]] bool operator==(const iterator& other) const noexcept;

			/**
			 * @param other		the iterator to compare against
			 * @returns			whether or not they are inequivalent
			 */
			[[nodiscard]] bool operator!=(const iterator& other) const noexcept;

			/**
			 * @returns true if this iterator is both initialized and not at end().
			 */
			[[nodiscard]] operator bool() const noexcept;

			/**
			 * Inverse of operator bool().
			 *
			 * @returns true if this iterator is uninitialized or at end(), false otherwise.
			 */
			[[nodiscard]] bool operator!() const noexcept;
#pragma endregion

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
		};

		class const_iterator final
		{
			friend class HashMap;

			CONST_FORWARD_ITERATOR(const_iterator, iterator)

			/**
			 * @returns true if this iterator is both initialized and not at end().
			 */
			[[nodiscard]] operator bool() const noexcept;

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

		BEGIN_END(iterator, const_iterator, HashMap)
#pragma endregion

#pragma region Properties
		/**
		 * @returns		true if the container is empty, false otherwise
		 */
		[[nodiscard]] constexpr bool IsEmpty() const noexcept;

		/**
		 * @returns		how many elements are in the container
		 */
		[[nodiscard]] constexpr size_type Size() const noexcept;

		/**
		 * @returns		how many buckets are in the container
		 */
		[[nodiscard]] constexpr size_type BucketCount() const noexcept;
#pragma endregion

#pragma region Element Access
		/**
		 * @param key	the key to query for
		 * @returns		the value at that key
		 *
		 * @throws std::out_of_range	if the key does not exist
		 */
		TValue& At(const TKey& key);

		/**
		 * @param key	the key to query for
		 * @returns		the value at that key
		 *
		 * @throws std::out_of_range	if the key does not exist
		 */
		const TValue& At(const TKey& key) const;

		/**
		 * @param key	the key to query for
		 * @returns		the value at that key
		 *
		 * @asserts		the key exists
		 */
		TValue& operator[](const TKey& key);

		/**
		 * @param key	the key to query for
		 * @returns		the value at that key
		 *
		 * @asserts		the key exists
		 */
		const TValue& operator[](const TKey& key) const;
#pragma endregion

#pragma region Insert
		/**
		 * Inserts the passed entry into the map if the key does not exist
		 * If the entry's key does exist, then do nothing and return an iterator at that position.
		 * O(1)
		 *
		 * @param key		the key to insert with
		 * @param value		the value to insert
		 * @returns			an iterator at the key matching the passed entry, and a bool of whether or not an insertion was performed
		 */
		std::pair<iterator, bool> Insert(const TKey& key, const TValue& value);
		
		/**
		 * Inserts the passed entry into the map if the key does not exist
		 * If the entry's key does exist, then do nothing and return an iterator at that position.
		 * O(1)
		 *
		 * @param key		the key to insert with
		 * @param value		the value to insert
		 * @returns			an iterator at the key matching the passed entry, and a bool of whether or not an insertion was performed
		 */
		std::pair<iterator, bool> Insert(TKey&& key, TValue&& value);
		
		/**
		 * Inserts the passed entry into the map if the key does not exist
		 * If the entry's key does exist, then do nothing and return an iterator at that position.
		 * O(1)
		 * 
		 * @param entry		the KeyValuePair to insert
		 * @returns			an iterator at the key matching the passed entry, and a bool of whether or not an insertion was performed
		 */
		std::pair<iterator, bool> Insert(const value_type& entry);

		/**
		 * Inserts the passed entry into the map if the key does not exist
		 * If the entry's key does exist, then do nothing and return an iterator at that position.
		 * O(1)
		 *
		 * @param entry		the KeyValuePair to insert
		 * @returns			an iterator at the key matching the passed entry, and a bool of whether or not an insertion was performed
		 */
		std::pair<iterator, bool> Insert(value_type&& entry);

		/**
		 * Insert from a range defined by iterators to KeyValuePairs: [first, last)
		 *
		 * @param <It>		the iterator type
		 * @param first		beginning iterator to read from (inclusive)
		 * @param last		ending iterator to read from (exclusive)
		 */
		template<std::forward_iterator It>
		void Insert(It first, const It last)
		{
			Resize(Size() + std::distance(first, last));
			InsertNoResize(first, last);
		}

		/**
		 * Inserts the passed entry into the map if the key does not exist
		 * If an entry with this key already exists it will be overwritten.
		 * O(1)
		 *
		 * @param <Args>	the type for the arguments to be passed along to KeyValuePair's ctor
		 * @param args		the arguments to be passed along to KeyValuePair's ctor
		 * @returns			an iterator at the key matching the passed entry, and a bool of whether or not an overwrite happened
		 */
		template<typename... Args>
		std::pair<iterator, bool> Emplace(Args&&... args);

		/**
		 * Inserts the passed entry into the map if the key does not exist
		 * If the entry's key does exist, then do nothing and return an iterator at that position.
		 * O(1)
		 *
		 * @param <Args>	the type for the arguments to be passed along to KeyValuePair's ctor
		 * @param args		the arguments to be passed along to KeyValuePair's ctor
		 * @returns			an iterator at the key matching the passed entry, and a bool of whether or not an insertion was performed
		 */
		template<typename... Args>
		std::pair<iterator, bool> TryEmplace(Args&&... args);
#pragma endregion

#pragma region Remove
		/**
		 * removes the specified key from the container
		 * O(1)
		 * 
		 * @param key		key to remove
		 * @return true		if a removal was performed
		 * @return false	if a removeal was not performed
		 */
		bool Remove(const TKey& key);

		/**
		 * Does nothing if pos is at end()
		 * O(1)
		 *
		 * @param pos	where to remove
		 * @returns		iterator following the removed element
		 */
		iterator Remove(iterator pos);

		/**
		 * Does nothing if pos is at end()
		 * O(1)
		 * 
		 * @param pos	where to remove
		 * @returns		iterator following the removed element
		 */
		iterator Remove(const_iterator pos);

		/**
		 * Removes elements in the range [first, last)
		 * O(n) where n = std::distance(first, last)
		 *
		 * @param first		beginning element to remove (inclusive)
		 * @param first		ending element to remove (exclusive)
		 * @returns			iterator following the last removed element
		 */
		iterator Remove(const_iterator first, const_iterator last);
#pragma endregion

#pragma region Query
		/**
		 * O(1)
		 * 
		 * @param key	the key to query for
		 * @returns		iterator at that key, or end() if the key does not exist
		 */
		[[nodiscard]] iterator Find(const TKey& key);

		/**
		 * O(1)
		 *
		 * @param key	the key to query for
		 * @returns		iterator at that key, or end() if the key does not exist
		 */
		[[nodiscard]] const_iterator Find(const TKey& key) const;

		/**
		 * O(1)
		 *
		 * @param key	the key to query for
		 * @returns		whether or not that key is in this container
		 */
		[[nodiscard]] bool Contains(const TKey& key) const;
#pragma endregion

#pragma region Memory
		/**
		 * Empties the cotnainer and frees all associated memory.
		 * Afterwards Bucket() = 1 and Size() = 0
		 */
		void Clear();

		/**
		 * changes the BucketCount()
		 * re-hashes all elements
		 * 
		 * @param count		the new BucketCount()
		 *
		 * @throws std::invalid_argument	if count < 1
		 */
		void Resize(size_type count);

		/**
		 * Swaps this Container's contents with another.
		 * No references or iterators are invalidated, though they all now point to the other container.
		 * O(1)
		 * 
		 * @param other		the container to swap contents with
		 */
		OTHER_TEMPLATE
		void Swap(OTHER_HASHMAP& other) noexcept;

		/**
		 * Merges the other container's contents into this one, removing those elements from the other container.
		 * Duplicates are not merged and remain in the other container.
		 * No moves or copies are performed.
		 * No references are invalidated but all iterators are.
		 * O(n) where n = Size() + other.Size()
		 *
		 * @param other		the container to merge contents with
		 */
		OTHER_TEMPLATE
		void Merge(OTHER_HASHMAP& other) noexcept;

		/**
		 * @returns		a duplicate of this container with the keys and values switched 
		 */
		HashMap<TValue, TKey> Invert() const;
#pragma endregion

#pragma region Operators
		/**
		 * Compares all elements in both containers for equivalence.
		 * The number of buckets does not matter, only the elements.
		 * O(n)
		 * 
		 * @param other		the container to compare this against
		 * @return true		if both containers contain all the same elements
		 * @return false	otherwise
		 */
		OTHER_TEMPLATE
		[[nodiscard]] friend bool operator==(const HashMap& left, const OTHER_HASHMAP& right)
		{
			if (reinterpret_cast<const void*>(&left) == reinterpret_cast<const void*>(&right))
			{
				return true;
			}
			if (left.Size() != right.Size())
			{
				return false;
			}
			// Can't just std::equal because that would enforce order.
			for (const auto& [key, value] : right)
			{
				if (!left.Contains(key) || left.At(key) != value)
				{
					return false;
				}
			}
			return true;
		}

		/**
		 * Compares all elements in both containers for equivalence.
		 * The number of buckets does not matter, only the elements.
		 * O(n)
		 *
		 * @param other		the container to compare this against
		 * @return true		if the containers do not contain all the same elements
		 * @return false	otherwise
		 */
		OTHER_TEMPLATE
			[[nodiscard]] friend bool operator!=(const HashMap& left, const OTHER_HASHMAP& right)
		{
			return !operator==(left, right);
		}

		/**
		 * "ToString" operator
		 * 
		 * @param stream	the stream to append to
		 * @param map		a HashMap
		 * @returns			the same stream
		 */
		friend std::ostream& operator<<(std::ostream& stream, const HashMap& map) noexcept
		{
			Util::StreamTo(stream, map.begin(), map.end());
			return stream;
		}
#pragma endregion

#pragma region Helpers
	private:
		/**
		 * Conditionally calls the ReserveStrategy if Size() >= BucketCount().
		 * Sanity checks the ReserveStrategy's return value before calling Resize().
		 */
		void TryResize();

		/**
		 * Conditionally calls the ReserveStrategy if Size() >= BucketCount().
		 * Sanity checks the ReserveStrategy's return value before calling Resize().
		 *
		 * @param newSize	expected new Size() after the calling operation
		 */
		void TryResize(size_type newSize);
		
		/**
		 * Insert from a range defined by iterators to KeyValuePairs: [first, last)
		 *
		 * @param <It>		the iterator type
		 * @param first		beginning iterator to read from (inclusive)
		 * @param last		ending iterator to read from (exclusive)
		 */
		template<std::forward_iterator It>
		void InsertNoResize(It first, const It last)
		{
			while (first != last)
			{
				Insert(*first++);
			}
		}
		
		/**
		 * @param key	key to query with
		 * @returns		an iterator with the chainIt at the node just before key.
		 */
		[[nodiscard]] iterator FindPrev(const TKey& key);
		
		/**
		 * Called by all Removes
		 * Unconditionally removes
		 * 
		 * @param pos	the position to remove
		 * @returns		iterator following the removed position
		 */
		iterator RemoveInternal(const_iterator pos);

		/**
		 * @param it	iterator to check
		 *
		 * @asserts		it.owner == this
		 */
		void AssertOwner(iterator it) const noexcept;

		/**
		 * @param it	iterator to check
		 *
		 * @asserts		it.owner == this
		 */
		void AssertOwner(const_iterator it) const noexcept;

		/**
		 * Sets the state after a move. 
		 */
		void SetPostMoveState() noexcept;
#pragma endregion
	};

	namespace Util
	{
		template<typename TKey, typename TValue>
		[[nodiscard]] bool Contains(const HashMap<TKey, TValue>& map, const TKey& key)
		{
			return map.Contains(key);
		}
	}
}

#include "HashMap.inl"
