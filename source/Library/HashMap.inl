// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "HashMap.h"

namespace Library
{
	template<typename TKey, typename TValue, std::predicate<TKey, TKey> KeyEqual>
	KeyValuePair<TKey, TValue, KeyEqual>::operator std::pair<TKey, TValue>()
	{
		return { key, value };
	}
	
	template<typename TKey, typename TValue, std::predicate<TKey, TKey> KeyEqual>
	inline bool KeyValuePair<TKey, TValue, KeyEqual>::operator==(const KeyValuePair& other) const
	{
		return KeyEqual{}(key, other.key);
	}

	template<typename TKey, typename TValue, std::predicate<TKey, TKey> KeyEqual>
	inline bool KeyValuePair<TKey, TValue, KeyEqual>::operator!=(const KeyValuePair& other) const
	{
		return !operator==(other);
	}

#pragma region Special Members
	TEMPLATE
	template<std::forward_iterator It>
	inline HASHMAP::HashMap(It first, It last) :
		HashMap(std::distance(first, last), first, last) {}

	TEMPLATE
	template<std::forward_iterator It>
	inline HASHMAP::HashMap(size_type count, It first, It last) :
		HashMap(count)
	{
		while (first != last)
		{
			Insert(*first++);
		}
	}
	
	TEMPLATE
	inline HASHMAP::HashMap(std::initializer_list<value_type> list) :
		HashMap(list.size(), list) {}

	TEMPLATE
	inline HASHMAP& HASHMAP::operator=(std::initializer_list<value_type> list)
	{
		Clear();
		Insert(list.begin(), list.end());
		return *this;
	}

	TEMPLATE
	inline HASHMAP::HashMap(size_type count, std::initializer_list<value_type> list) :
		HashMap(count, list.begin(), list.end()) {}

	TEMPLATE
	OTHER_TEMPLATE
	inline HASHMAP::HashMap(const OTHER_HASHMAP& other) :
		HashMap(other.Size(), other.begin(), other.end()) {}

	TEMPLATE
	OTHER_TEMPLATE
	inline HASHMAP::HashMap(OTHER_HASHMAP&& other) :
		buckets(std::move(reinterpret_cast<HashMap&>(other).buckets)),
		size(other.Size())
	{
		reinterpret_cast<HashMap&>(other).SetPostMoveState();
	}

	TEMPLATE
	OTHER_TEMPLATE
	inline HASHMAP& HASHMAP::operator=(const OTHER_HASHMAP& other)
	{
		Clear();
		Insert(other.begin(), other.end());
		return *this;
	}

	TEMPLATE
	OTHER_TEMPLATE
	inline HASHMAP& HASHMAP::operator=(OTHER_HASHMAP&& other)
	{
		Clear();
		buckets = std::move(reinterpret_cast<HashMap&>(other).buckets);
		size = other.Size();
		reinterpret_cast<HashMap&>(other).SetPostMoveState();
		return *this;
	}
	
	TEMPLATE
	inline HASHMAP::HashMap(const size_type count) :
		buckets(count, ChainType())
	{
		assertm(count, "cannot construct a HashMap with 0 buckets");
	}

	TEMPLATE
	inline HASHMAP::HashMap(HashMap&& other) noexcept :
		buckets(std::move(other.buckets)),
		size(other.size)
	{
		other.SetPostMoveState();
	}

	TEMPLATE
	inline HASHMAP& HASHMAP::operator=(HashMap&& other) noexcept
	{
		if (this != &other)
		{
			buckets = std::move(other.buckets);
			size = other.size;

			other.SetPostMoveState();
		}
		return *this;
	}
#pragma endregion

#pragma region iterator
	TEMPLATE
	inline HASHMAP::iterator::iterator(BucketIt bucketIt, const HashMap& owner) noexcept :
		owner(&owner),
		bucketIt(bucketIt) {}

	TEMPLATE
	inline HASHMAP::iterator::iterator(BucketIt bucketIt, ChainIt chainIt, const HashMap& owner) noexcept :
		owner(&owner),
		bucketIt(bucketIt),
		chainIt(chainIt) {}

	TEMPLATE
	inline typename HASHMAP::iterator HASHMAP::iterator::operator++(int) noexcept
	{
		auto ret = *this;
		operator++();
		return ret;
	}

	TEMPLATE
	inline typename HASHMAP::iterator& HASHMAP::iterator::operator++() noexcept
	{
		// Go down the chain (shouldn't cause a problem if we're already at end().
		++chainIt;

		// If we're at the end of this chain...
		if (!chainIt)
		{
			// Look for the next non-empty chain.
			do
			{
				++bucketIt;
			} while (bucketIt != owner->buckets.end() && bucketIt->IsEmpty());
			// Grab the beginning of the chain if it exists.
			if (bucketIt != owner->buckets.end())
			{
				chainIt = bucketIt->begin();
			}
		}
		return *this;
	}

	TEMPLATE
	inline typename HASHMAP::iterator::reference HASHMAP::iterator::operator*() const
	{
		assertm(!IsAtEnd(), "attempted to dereference HasmHap iterator at end()");
		return chainIt.operator*();
	}

	TEMPLATE
	inline typename HASHMAP::iterator::pointer HASHMAP::iterator::operator->() const
	{
		return &operator*();
	}

	TEMPLATE
	inline HASHMAP::iterator::operator bool() const noexcept
	{
		return owner && !IsAtEnd();
	}

	TEMPLATE
	inline bool HASHMAP::iterator::operator!() const noexcept
	{
		return !operator bool();
	}

	TEMPLATE
	inline bool HASHMAP::iterator::IsAtEnd() const noexcept
	{
		assertm(owner, "uninitialized HashMap iterator in IsAtEnd()");
		return bucketIt == owner->buckets.end() || chainIt.IsAtEnd();
	}

	TEMPLATE
	inline bool HASHMAP::iterator::IsAtBegin() const noexcept
	{
		assertm(owner, "uninitialized HashMap iterator in IsAtBegin()");
		return *this == owner->begin();
	}

	TEMPLATE
	inline bool HASHMAP::iterator::operator==(const iterator& other) const noexcept
	{
		// No need to compare the bucketIt.
		return chainIt.operator==(other.chainIt);
	}

	TEMPLATE
	inline bool HASHMAP::iterator::operator!=(const iterator& other) const noexcept
	{
		return !operator==(other);
	}

	TEMPLATE
	inline HASHMAP::const_iterator::operator bool() const noexcept
	{
		return it.operator bool();
	}

	TEMPLATE
	inline bool HASHMAP::const_iterator::operator!() const noexcept
	{
		return it.operator!();
	}

	TEMPLATE
	inline bool HASHMAP::const_iterator::IsAtEnd() const noexcept
	{
		return it.IsAtEnd();
	}

	TEMPLATE
	inline bool HASHMAP::const_iterator::IsAtBegin() const noexcept
	{
		return it.IsAtBegin();
	}

	TEMPLATE
	inline typename HASHMAP::iterator HASHMAP::begin() noexcept
	{
		typename BucketType::iterator bucketIt = buckets.begin();
		while (bucketIt != buckets.end() && bucketIt->IsEmpty())
		{
			++bucketIt;
		}
		return bucketIt == buckets.end() ? end() : iterator(bucketIt, bucketIt->begin(), *this);
	}

	TEMPLATE
	inline typename HASHMAP::iterator HASHMAP::end() noexcept
	{
		return iterator(buckets.end(), *this);
	}
#pragma endregion

#pragma region Properties
	TEMPLATE
	inline constexpr bool HASHMAP::IsEmpty() const noexcept
	{
		return Size() <= 0;
	}

	TEMPLATE
	inline constexpr typename HASHMAP::size_type HASHMAP::Size() const noexcept
	{
		return size;
	}

	TEMPLATE
	inline constexpr typename HASHMAP::size_type HASHMAP::BucketCount() const noexcept
	{
		return buckets.Capacity();
	}
#pragma endregion

#pragma region Accessors
	TEMPLATE
	inline TValue& HASHMAP::At(const TKey& key)
	{
		auto it = Find(key);
		if (it.IsAtEnd())
		{
			throw std::out_of_range("key does not exist");
		}
		return it->value;
	}
	
	TEMPLATE
	inline const TValue& HASHMAP::At(const TKey& key) const
	{
		return const_cast<HashMap*>(this)->At(key);
	}

	TEMPLATE
	inline TValue& HASHMAP::operator[](const TKey& key)
	{
		auto it = Find(key);
		if (it.IsAtEnd())
		{
			it.bucketIt->PushFront({ key, TValue() });
			it.chainIt = it.bucketIt->begin();
			size++;
		}
		return it->value;
	}

	TEMPLATE
	inline const TValue& HASHMAP::operator[](const TKey& key) const
	{
		return At(key);
	}
#pragma endregion

#pragma region Insert
	TEMPLATE
	inline std::pair<typename HASHMAP::iterator, bool> HASHMAP::Insert(const TKey& key, const TValue& value)
	{
		return Insert({ key, value });
	}

	TEMPLATE
	inline std::pair<typename HASHMAP::iterator, bool> HASHMAP::Insert(TKey&& key, TValue&& value)
	{
		return Insert({ std::move(key), std::move(value) });
	}

	TEMPLATE
	inline std::pair<typename HASHMAP::iterator, bool> HASHMAP::Insert(const value_type& entry)
	{
		TryResize();
		// Find will either return end or an iterator right at the matching key.
		std::pair<iterator, bool> ret = { Find(entry.key), false };
		if (ret.first.IsAtEnd())
		{
			ret.first.bucketIt->PushFront(entry);
			ret.first.chainIt = ret.first.bucketIt->begin();
			ret.second = true;
			size++;
		}
		return ret;
	}

	TEMPLATE
	inline std::pair<typename HASHMAP::iterator, bool> HASHMAP::Insert(value_type&& entry)
	{
		TryResize();
		// Find will either return end or an iterator right at the matching key.
		std::pair<iterator, bool> ret = { Find(entry.key), false };
		if (ret.first.IsAtEnd())
		{
			ret.first.bucketIt->PushFront(std::move(entry));
			ret.first.chainIt = ret.first.bucketIt->begin();
			ret.second = true;
			size++;
		}
		return ret;
	}
	
	TEMPLATE
	template<typename ...Args>
	inline std::pair<typename HASHMAP::iterator, bool> HASHMAP::Emplace(Args&& ...args)
	{
		TryResize();
		// Construct a node directly. This is how we guarantee no moves or copies.
		auto node = new typename ChainType::Node(std::forward<Args>(args)...);
		// Find will either return end or an iterator right at the matching key.
		std::pair<iterator, bool> ret = { FindPrev(node->data.key), false };
		// No overwrite.
		if (!(ret.second = !ret.first.IsAtEnd()))
		{
			size++;
			ret.first.bucketIt->PushFront(node);
			ret.first.chainIt = ret.first.bucketIt->begin();
		}
		else
		{
			ret.first.bucketIt->RemoveAfter(ret.first.chainIt);
			ret.first.chainIt = ret.first.bucketIt->InsertAfter(ret.first.chainIt, node);
		}
		return ret;
	}

	TEMPLATE
	template<typename ...Args>
	inline std::pair<typename HASHMAP::iterator, bool> HASHMAP::TryEmplace(Args&& ...args)
	{
		TryResize();
		auto node = new typename ChainType::Node(std::forward<Args>(args)...);
		// Find will either return end or an iterator right at the matching key.
		std::pair<iterator, bool> ret = { Find(node->data.key), false };
		if (ret.first.IsAtEnd())
		{
			ret.first.bucketIt->PushFront(node);
			ret.first.chainIt = ret.first.bucketIt->begin();
			ret.second = true;
			size++;
		}
		else
		{
			delete node;
		}
		return ret;
	}
#pragma endregion

#pragma region Remove
	TEMPLATE
	inline bool HASHMAP::Remove(const TKey& key)
	{
		auto it = Find(key);
		if (it.IsAtEnd())
		{
			return false;
		}
		RemoveInternal(it);
		return true;
	}

	TEMPLATE
	inline typename HASHMAP::iterator HASHMAP::Remove(iterator pos)
	{
		AssertOwner(pos);
		if (!pos.IsAtEnd())
		{
			pos = RemoveInternal(pos);
		}
		return pos;
	}

	TEMPLATE
	inline typename HASHMAP::iterator HASHMAP::Remove(const_iterator pos)
	{
		return Remove(pos.it);
	}

	TEMPLATE
	inline typename HASHMAP::iterator HASHMAP::Remove(const_iterator first, const const_iterator last)
	{
		AssertOwner(last);
		while (first != last)
		{
			first.it = Remove(first.it);
		}
		return first.it;
	}
#pragma endregion

#pragma region Query
	TEMPLATE
	inline typename HASHMAP::iterator HASHMAP::Find(const TKey& key)
	{
		// Avoid a call to operator+ by invoking the constructor directly at the desired index.
		auto bucketIt = BucketIt(Hash{}(key) % BucketCount(), buckets);
		auto chainIt = Util::Find(*bucketIt, [&key](const auto& a) { return KeyEqual{}(a.key, key); });
		return iterator(bucketIt, chainIt, *this);
	}

	TEMPLATE
	inline typename HASHMAP::const_iterator HASHMAP::Find(const TKey& key) const
	{
		return const_cast<HashMap*>(this)->Find(key);
	}
	
	TEMPLATE
	inline bool HASHMAP::Contains(const TKey& key) const
	{
		return Find(key);
	}
#pragma endregion

#pragma region Memory
	TEMPLATE
	inline void HASHMAP::Clear()
	{
		buckets.Resize(1);
		buckets.Front().Clear();
		size = 0;
	}
	
	TEMPLATE
	inline void HASHMAP::Resize(const size_type count)
	{
		if (count < 1)
		{
			throw std::invalid_argument("need at least 1 bucket");
		}

		// Make space for the new buckets.
		BucketType newBuckets(count, ChainType());

		// Rehash everything.
		for (const auto& kvp : *this)
		{
			const auto index = Hash{}(kvp.key) % newBuckets.Size();
			newBuckets[index].PushFront(std::move(kvp));
		}

		// Reassign the buckets.
		buckets = std::move(newBuckets);
	}

	TEMPLATE
	OTHER_TEMPLATE
	inline void HASHMAP::Swap(OTHER_HASHMAP& other) noexcept
	{
		std::swap(*this, reinterpret_cast<HashMap&>(other));
	}

	TEMPLATE
	OTHER_TEMPLATE
	inline void HASHMAP::Merge(OTHER_HASHMAP& other) noexcept
	{
		// resize once for good hash performance during this operation
		TryResize(Size() + other.Size());
		const auto originalSize = Size();
		// manually iterate through the other's buckets
		for (auto& chain : reinterpret_cast<HashMap&>(other).buckets)
		{
			// manually iterate though the other's chain
			for (auto otherChainIt = chain.before_begin(); otherChainIt && otherChainIt.node->next; ++otherChainIt)
			{
				// figure out where the other's key would go into this container
				auto [_, thisBucketIt, thisChainIt] = FindPrev(otherChainIt.node->next->data.key);
				// if we don't already have that key...
				if (!thisChainIt || !thisChainIt.node->next || !KeyEqual{}(thisChainIt.node->next->data.key, otherChainIt.node->next->data.key))
				{
					auto temp = otherChainIt.node->next;
					// remove it from the other container
					otherChainIt.node->next = otherChainIt.node->next->next;
					// add it to this one
					thisBucketIt->PushFront(temp);
					++size;
					--chain.size;
					if (chain.IsEmpty())
					{
						chain.tail = nullptr;
					}
				}
			}
		}
		reinterpret_cast<HashMap&>(other).size -= Size() - originalSize;
	}

	TEMPLATE
	inline HashMap<TValue, TKey> HASHMAP::Invert() const
	{
		HashMap<TValue, TKey> ret{ Size() };
		for (const auto& [key, value] : *this)
		{
			ret.Insert({ value, key	});
		}
		return ret;
	}
#pragma endregion

#pragma region Helpers
	TEMPLATE
	inline void HASHMAP::TryResize()
	{
		TryResize(Size() + 1);
	}
	
	TEMPLATE
	inline void HASHMAP::TryResize(const size_type newSize)
	{
		if (newSize > BucketCount())
		{
			Resize(std::max(ReserveStrategy{}(newSize, BucketCount()), Size()));
		}
	}
	
	TEMPLATE
	inline typename HASHMAP::iterator HASHMAP::FindPrev(const TKey& key)
	{
		// Avoid a call to operator+ by invoking the constructor directly at the desired index.
		auto bucketIt = BucketIt(Hash{}(key) % BucketCount(), buckets);
		auto chainIt = bucketIt->FindPrev([&key](const auto& a) { return KeyEqual{}(key, a.key); });
		return iterator(bucketIt, chainIt.node->next ? chainIt : ++chainIt, *this);
	}
	
	TEMPLATE
	inline typename HASHMAP::iterator HASHMAP::RemoveInternal(const_iterator pos)
	{
		size--;
		pos.it.chainIt = pos.it.bucketIt->RemoveAt(pos.it.chainIt);
		if (!pos.it.chainIt)
		{
			++pos;
		}
		return pos.it;
	}

	TEMPLATE
	inline void HASHMAP::AssertOwner([[maybe_unused]] const iterator it) const noexcept
	{
		assertm(it.owner == this, "iterator does not belong to this HashMap");
	}

	TEMPLATE
	inline void HASHMAP::AssertOwner(const const_iterator it) const noexcept
	{
		AssertOwner(it.it);
	}

	TEMPLATE
	inline void HASHMAP::SetPostMoveState() noexcept
	{
		// Buckets should have already been moved, so all that needs to be done is reset the size.
		size = 0;
	}
#pragma endregion
}

#undef TEMPLATE
#undef HASHMAP
