// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "SList.h"

namespace Library
{
#pragma region Ctors
	template<typename T>
	template<typename ...Args>
	inline SList<T>::Node::Node(Node* next, Args&& ...args) :
		next(next),
		data{ std::forward<Args>(args)... } {}
	
	template<typename T>
	template<typename ...Args>
	inline SList<T>::Node::Node(Args&& ...args) :
		data{ std::forward<Args>(args)... } {}

	template<typename T>
	inline SList<T>::SList(const std::initializer_list<T> list) :
		SList(list.begin(), list.end()) {}

	template<typename T>
	inline SList<T>& SList<T>::operator=(const std::initializer_list<T> list)
	{
		Clear();
		Append(list.begin(), list.end());
		return *this;
	}

	template<typename T>
	template<std::forward_iterator It>
	inline SList<T>::SList(It first, It last)
	{
		Append(first, last);
	}

	template<typename T>
	template<std::random_access_iterator It>
	inline SList<T>::SList(It first, It last) :
		SList(std::distance(first, last), first, last) {}

	template<typename T>
	template<Concept::RangeOf<T> Range>
	inline SList<T>::SList(const Range& range) :
		SList(Util::GetSize(range), range.begin(), range.end()) {}

	template<typename T>
	template<Concept::RangeOf<T> Range>
	inline SList<T>& SList<T>::operator=(const Range& range)
	{
		Clear();
		Append(range.begin(), range.end());
		return *this;
	}

	template<typename T>
	template<std::forward_iterator It>
	inline SList<T>::SList(const size_type size, It first, It last) :
		size(size)
	{
		head = tail = new Node(*first++);
		while (first != last)
		{
			tail->next = new Node(*first++);
			tail = tail->next;
		}
	}

	template<typename T>
	inline SList<T>::SList(const SList& other) :
		size(other.size)
	{
		Copy(other);
	}

	template<typename T>
	inline SList<T>::SList(SList&& other) noexcept :
		head(other.head),
		tail(other.tail),
		size(other.size)
	{
		other.SetPostMoveState();
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(const SList& other)
	{
		if (this != &other)
		{
			Clear();
			Copy(other);
			size = other.Size();
		}
		return *this;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(SList&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			
			head = other.head;
			tail = other.tail;
			size = other.size;

			other.SetPostMoveState();
		}
		return *this;
	}

	template<typename T>
	inline SList<T>::~SList()
	{
		Clear();
	}
#pragma endregion

#pragma region iterator
	template<typename T>
	inline SList<T>::iterator::iterator(Node* node, [[maybe_unused]] const SList<T>* owner) noexcept :
#ifdef _DEBUG
		owner(owner),
#endif
		node(node) {}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::iterator::operator++(int) noexcept
	{
		const iterator ret = *this;
		operator++();
		return ret;
	}

	template<typename T>
	inline typename SList<T>::iterator& SList<T>::iterator::operator++() noexcept
	{
		if (node)
		{
			node = node->next;
		}
		return *this;
	}

	template<typename T>
	inline typename SList<T>::iterator::reference SList<T>::iterator::operator*() const
	{
		if (!node)
		{
			throw std::out_of_range("iterator is at end()");
		}
		return node->data;
	}

	template<typename T>
	inline typename SList<T>::iterator::pointer SList<T>::iterator::operator->() const
	{
		return &operator*();
	}

	template<typename T>
	inline bool SList<T>::iterator::operator==(const iterator other) const noexcept
	{
		return node == other.node;
	}

	template<typename T>
	inline bool SList<T>::iterator::operator!=(const iterator other) const noexcept
	{
		return !operator==(other);
	}

	template<typename T>
	inline SList<T>::iterator::operator bool() const noexcept
	{
		return !IsAtEnd();
	}

	template<typename T>
	inline bool SList<T>::iterator::operator!() const noexcept
	{
		return !operator bool();
	}

	template<typename T>
	inline bool SList<T>::iterator::IsAtEnd() const noexcept
	{
		AssertInitialized();
		return !node;
	}

	template<typename T>
	inline void SList<T>::iterator::AssertInitialized() const noexcept
	{
		assertm(owner, "uninitialized SList iterator");
	}

	template<typename T>
	inline SList<T>::const_iterator::operator bool() const noexcept
	{
		return it.operator bool();
	}

	template<typename T>
	inline bool SList<T>::const_iterator::operator!() const noexcept
	{
		return it.operator!();
	}

	template<typename T>
	inline bool SList<T>::const_iterator::IsAtEnd() const noexcept
	{
		return it.IsAtEnd();
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::begin() noexcept
	{
		return iterator(head, this);
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::end() noexcept
	{
		return iterator(nullptr, this);
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::before_begin() noexcept
	{
		// The head pointer is the first data member of an SList.
		// Since SList has no vtable pointer, we can treat the SList itself as another node, just with no data.
		return iterator(reinterpret_cast<Node*>(this), this);
	}

	template<typename T>
	inline typename SList<T>::const_iterator SList<T>::before_begin() const noexcept
	{
		return const_cast<SList*>(this)->before_begin();
	}

	template<typename T>
	inline typename SList<T>::const_iterator SList<T>::cbefore_begin() const noexcept
	{
		return before_begin();
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::before_end() noexcept
	{
		return iterator(tail, this);
	}

	template<typename T>
	inline typename SList<T>::const_iterator SList<T>::before_end() const noexcept
	{
		return const_cast<SList*>(this)->before_end();
	}

	template<typename T>
	inline typename SList<T>::const_iterator SList<T>::cbefore_end() const noexcept
	{
		return before_end();
	}
#pragma endregion

#pragma region Properties
	template<typename T>
	inline constexpr typename SList<T>::size_type SList<T>::Size() const noexcept
	{
		return size;
	}

	template<typename T>
	inline constexpr bool SList<T>::IsEmpty() const noexcept
	{
		return Size() <= 0;
	}
#pragma endregion

#pragma region Element Access
	template<typename T>
	inline typename SList<T>::reference SList<T>::Front()
	{
		ThrowEmpty();
		return head->data;
	}

	template<typename T>
	inline const typename SList<T>::reference SList<T>::Front() const
	{
		return const_cast<SList*>(this)->Front();
	}

	template<typename T>
	inline typename SList<T>::reference SList<T>::Back()
	{
		ThrowEmpty();
		return tail->data;
	}

	template<typename T>
	inline const typename SList<T>::reference SList<T>::Back() const
	{
		return const_cast<SList*>(this)->Back();
	}

	template<typename T>
	inline typename SList<T>::reference SList<T>::At(size_type pos)
	{
		ThrowIndex(pos);
		Node* node = head;
		while (pos--)
		{
			node = node->next;
		}
		assertm(node, "null node in SList::At");
#pragma warning(push)
#pragma warning(disable: 6011)
		return node->data;
#pragma warning(pop)
	}

	template<typename T>
	inline const typename SList<T>::reference SList<T>::At(const size_type pos) const
	{
		return const_cast<SList*>(this)->At(pos);
	}
#pragma endregion

#pragma region Insert
	template<typename T>
	inline typename SList<T>::iterator SList<T>::InsertAfter(const const_iterator pos, const T& t)
	{
		return EmplaceAfter(pos, t);
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::InsertAfter(const const_iterator pos, T&& t)
	{
		return EmplaceAfter(pos, std::move(t));
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::InsertAfter(const_iterator pos, const size_type count, const T& prototype)
	{
		AssertOwner(pos);
		if (!pos)
		{
			Append(count, prototype);
			return TailIt();
		}
		for (size_t i = 0; i < count; i++)
		{
			pos.it.node = pos.it.node->next = new Node(pos.it.node->next, prototype);
		}
		return pos.it;
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::InsertAfter(const const_iterator pos, const std::initializer_list<T> list)
	{
		return InsertAfter(pos, list.begin(), list.end());
	}
	
	template<typename T>
	template<std::forward_iterator It>
	inline typename SList<T>::iterator SList<T>::InsertAfter(const const_iterator pos, const It first, const It last)
	{
		AssertOwner(pos);

		if (!pos)
		{
			Append(first, last);
			return TailIt();
		}

		const auto oldSize = size;
		if constexpr (std::random_access_iterator<It>)
		{
			size += std::distance(first, last);
		}
		
		auto p = pos.it;
		for (auto it = first; it != last; ++it)
		{
			if constexpr (!std::random_access_iterator<It>)
			{
				size++;
			}
			p.node = p.node->next = new Node(p.node->next, *it);
		}
		if (oldSize == 1)
		{
			tail = p.node;
		}
		return p;
	}
	
	template<typename T>
	template<typename ...Args>
	inline typename SList<T>::iterator SList<T>::EmplaceAfter(const_iterator pos, Args&& ...args)
	{
		AssertOwner(pos);
		if (!pos || pos.it.node == tail)
		{
			EmplaceBack(std::forward<Args>(args)...);
			return iterator(tail, this);
		}
		pos.it.node->next = new Node(pos.it.node->next, std::forward<Args>(args)...);
		pos.it.node = pos.it.node->next;
		return pos.it;
	}

	template<typename T>
	template<typename ...Args>
	inline typename SList<T>::reference SList<T>::EmplaceBack(Args&& ...args)
	{
		Node* node = new Node(std::forward<Args>(args)...);
		if (IsEmpty())
		{
			head = node;
		}
		else
		{
			tail->next = node;
		}
		tail = node;
		size++;
		return tail->data;
	}

	template<typename T>
	template<typename ...Args>
	inline typename SList<T>::reference SList<T>::EmplaceFront(Args&& ...args)
	{
		PushFront(new Node(head, std::forward<Args>(args)...));
		return head->data;
	}

	template<typename T>
	inline void SList<T>::PushBack(const T& t)
	{
		EmplaceBack(t);
	}
	
	template<typename T>
	inline void SList<T>::PushBack(T&& t)
	{
		EmplaceBack(std::move(t));
	}
	
	template<typename T>
	inline void SList<T>::PushFront(const T& t)
	{
		EmplaceFront(t);
	}
	
	template<typename T>
	inline void SList<T>::PushFront(T&& t)
	{
		EmplaceFront(std::move(t));
	}
	
	template<typename T>
	inline void SList<T>::Append(size_type count, const T& prototype)
	{
		size += count;
		while (count--)
		{
			tail->next = new Node(prototype);
			tail = tail->next;
		}
	}

	template<typename T>
	inline void SList<T>::Append(const std::initializer_list<T> list)
	{
		Append(list.begin(), list.end());
	}

	template<typename T>
	inline void SList<T>::Append(const SList<T>& list)
	{
		Append(list.begin(), list.end());
	}

	template<typename T>
	template<std::forward_iterator It>
	inline void SList<T>::Append(It first, const It last)
	{
		if (IsEmpty())
		{
			head = tail = new Node(*first++);
			size++;
		}
		if constexpr (std::random_access_iterator<It>)
		{
			size += std::distance(first, last);
		}
		while (first != last)
		{
			if constexpr (!std::random_access_iterator<It>)
			{
				size++;
			}
			tail->next = new Node(*first++);
			tail = tail->next;
		}
	}
#pragma endregion

#pragma region Remove
	template<typename T>
	inline typename SList<T>::iterator SList<T>::Remove(const T& t)
	{
		return Remove([&t](const auto& a) { return a == t; });
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::Remove(T&& t)
	{
		return Remove([t = std::move(t)](const auto& a) { return a == t; });
	}

	template<typename T>
	template<std::predicate<T> Predicate>
	inline typename SList<T>::iterator SList<T>::Remove(const Predicate predicate)
	{
		if (IsEmpty())
		{
			return begin();
		}
		if (predicate(Front()))
		{
			PopFront();
			return begin();
		}
		return RemoveAfter(FindPrev(predicate));
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::RemoveAt(const_iterator pos)
	{
		if (!pos)
		{
			AssertOwner(pos);
			PopBack();
			return end();
		}
		const auto prev = pos++;
		return Remove(prev, pos);
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::Remove(size_type first, size_type last)
	{
		if (first > last)
		{
			throw std::invalid_argument("Must pass indices in increasing order.");
		}

		last = std::min(last, Size());
		first = std::min(first, Size());
		
		auto fit = cbegin();
		for (size_type i = 0; i < first; i++, ++fit);
		auto lit = fit;
		for (; first < last; first++, ++lit);
		return Remove(fit, lit);
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::Remove(const_iterator first, const const_iterator last)
	{
		AssertOwner(first);
		AssertOwner(last);

		// Case when last = end().
		// We delete every node from first to end().
		if (!last)
		{
			// Case of 1 node.
			if (head == tail)
			{
				Clear();
			}
			else
			{
				// Get the node just before first as the new tail.
				tail = GetPrev(first.it.node);
				tail->next = nullptr;
				size -= Delete(first.it.node);
				if (Size() == 1)
				{
					head = tail;
				}
			}
			return iterator(tail, this);
		}

		// Move the data from last into first.
		// This means that we'll technically be deleting the node at last instead of the node at first.
		// This keeps us from having to find the node right before first.
		first.it.node->data.~T();
		new (&first.it.node->data) T(std::move(last.it.node->data));
		Node* temp = first.it.node->next;
		first.it.node->next = last.it.node->next;

		for (auto node = temp; temp != last.it.node; node = temp)
		{
			size--;
			temp = temp->next;
			delete node;
		}
		delete last.it.node;
		size--;
		
		if (Size() == 1)
		{
			tail = head;
		}
		return first.it;
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::RemoveAfter(const_iterator pos)
	{
		AssertOwner(pos);
		auto prev = pos++.it;
		if (pos && prev)
		{
			const auto temp = pos.it.node;
			prev.node->next = (++pos).it.node;
			delete temp;
			size--;
		}
		return pos.it;
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::RemoveAfter(const_iterator first, const const_iterator last)
	{
		AssertOwner(first);
		AssertOwner(last);
		if (first && first.it.node->next)
		{
			const auto temp = first.it.node->next;
			first.it.node->next = last.it.node;
			size -= DeleteUntil(temp, last.it.node);
		}
		return last.it;
	}

	template<typename T>
	inline void SList<T>::RemoveAllAfter(const const_iterator pos)
	{
		AssertOwner(pos);
		RemoveAfter(pos, end());
	}

	template<typename T>
	inline typename SList<T>::size_type SList<T>::RemoveAll(const T& t)
	{
		return RemoveAll([&t](const auto& a) { return t == a; });
	}

	template<typename T>
	inline typename SList<T>::size_type SList<T>::RemoveAll(T&& t)
	{
		return RemoveAll([t = std::move(t)](const auto& a) { return t == a; });
	}

	template<typename T>
	template<std::predicate<T> Predicate>
	inline typename SList<T>::size_type SList<T>::RemoveAll(const Predicate predicate)
	{
		const size_t oldSize = size;
		Remove(const_iterator(std::remove_if(begin(), end(), predicate)), end());
		return oldSize - size;
	}

	template<typename T>
	inline void SList<T>::PopFront()
	{
		if (!IsEmpty())
		{
			Node* temp = head;
			head = head->next;
			delete temp;
			size--;

			if (Size() == 1)
			{
				tail = head;
			}
		}		
	}
	
	template<typename T>
	inline void SList<T>::PopBack()
	{
		if (!IsEmpty())
		{
			Node* temp = tail;
			tail = GetPenultimate();
			delete temp;
			size--;

			if (IsEmpty())
			{
				head = nullptr;
			}
			else
			{
				tail->next = nullptr;
			}
		}
	}

	template<typename T>
	inline void SList<T>::Clear()
	{
		// Banking on the compiler being smart enough to not put any of the code for counting deletions here.
		Delete(head);
		head = tail = nullptr;
		size = 0;
	}
#pragma endregion

#pragma region Query
	template<typename T>
	inline typename SList<T>::iterator SList<T>::FindPrev(const T& t)
	{
		return FindPrev([&t](const auto& a) { return a == t; });
	}

	template<typename T>
	inline typename SList<T>::const_iterator SList<T>::FindPrev(const T& t) const
	{
		return const_cast<SList*>(this)->FindPrev(t);
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::FindPrev(T&& t)
	{
		return FindPrev([t = std::move(t)](const auto& a) { return a == t; });
	}

	template<typename T>
	inline typename SList<T>::const_iterator SList<T>::FindPrev(T&& t) const
	{
		return const_cast<SList*>(this)->FindPrev(std::move(t));
	}

	template<typename T>
	template<std::predicate<T> Predicate>
	inline typename SList<T>::iterator SList<T>::FindPrev(const Predicate predicate)
	{
		auto it = before_begin();
		while (it.node->next && !predicate(it.node->next->data))
		{
			++it;
		}
		return it;
	}

	template<typename T>
	template<std::predicate<T> Predicate>
	inline typename SList<T>::const_iterator SList<T>::FindPrev(const Predicate predicate) const
	{
		return const_cast<SList*>(this)->FindPrev(predicate);
	}
#pragma endregion

#pragma region Memory
	template<typename T>
	inline void SList<T>::ShrinkTo(const size_type count)
	{
		if (count > Size())
		{
			throw std::invalid_argument("Cannot shrink to size " + std::to_string(count) + " from existing size " + std::to_string(Size()));
		}
		Remove(count, Size());
	}
	
	template<typename T>
	inline void SList<T>::Resize(const size_type count, const T& prototype)
	{
		if (count < Size())
		{
			ShrinkTo(count);
		}
		else
		{
			InsertAfter(end(), count - Size(), prototype);
		}
	}
	
	template<typename T>
	inline void SList<T>::Swap(SList& other) noexcept
	{
		std::swap(*this, other);
	}
	
	template<typename T>
	inline void SList<T>::Reverse() noexcept
	{
		auto curr = head;
		Node* prev = nullptr;
		while (curr)
		{
			auto next = curr->next;
			curr->next = prev;
			prev = curr;
			curr = next;
		}
		head = prev;
	}

	template<typename T>
	inline void SList<T>::Sort()
	{
		Sort(std::less<T>());
	}
	
	template<typename T>
	template<std::predicate<T, T> LessThan>
	inline void SList<T>::Sort(const LessThan less)
	{
		if (Size() > 1)
		{
			// Find the midway point.
			const auto mid = GetMiddle();

			// Make another list starting at mid.
			SList temp;
			temp.head = mid.node->next;
			temp.tail = tail;
			temp.size = size - size / 2;

			// Make this list end at mid.
			tail = mid.node;
			tail->next = nullptr;
			size = size / 2;

			// Sort both halves.
			Sort(less);
			temp.Sort(less);

			// Merge the two halves into one sorted list.
			Merge(temp);
		}
	}

	template<typename T>
	template<std::predicate<T, T> LessThan>
	inline void SList<T>::Merge(SList<T>& other, const LessThan less)
	{
		// A moved list is the same as an empty list.
		Merge(std::move(other), less);
	}

	template<typename T>
	inline void SList<T>::Merge(SList<T>& other)
	{
		Merge(other, std::less<T>());
	}

	template<typename T>
	template<std::predicate<T, T> LessThan>
	inline void SList<T>::Merge(SList<T>&& other, const LessThan less)
	{
		if (!other.IsEmpty())
		{
			if (IsEmpty())
			{
				*this = std::move(other);
				return;
			}
			head = Merge(head, other.head, less);
			tail = tail->next ? other.tail : tail;
		}
		size += other.Size();
		other.SetPostMoveState();
	}
	
	template<typename T>
	inline void SList<T>::Merge(SList<T>&& other)
	{
		Merge(std::move(other), std::less<T>());
	}
#pragma endregion
	
#pragma region Operators
	template<typename T>
	inline bool SList<T>::operator==(const SList<T>& other) const
	{
		return this == &other || size == other.size && std::equal(begin(), end(), other.begin(), other.end());
	}
	
	template<typename T>
	inline bool SList<T>::operator!=(const SList<T>& other) const
	{
		return !operator==(other);
	}
#pragma endregion

#pragma region Helpers
	template<typename T>
	inline typename SList<T>::iterator SList<T>::InsertAfter(const_iterator pos, Node* node)
	{
		AssertOwner(pos);
		node->next = pos.it.node->next;
		pos.it.node->next = node;
		return ++pos.it;
	}
	
	template<typename T>
	inline void SList<T>::PushFront(Node* node) noexcept
	{
		head = node;
		if (IsEmpty())
		{
			tail = head;
		}
		size++;
	}
	
	template<typename T>
	inline typename SList<T>::iterator SList<T>::TailIt() noexcept
	{
		return iterator(tail, this);
	}
	
	template<typename T>
	inline typename SList<T>::Node* SList<T>::GetPenultimate() const noexcept
	{
		return GetPrev(tail);
	}

	template<typename T>
	inline typename SList<T>::Node* SList<T>::GetPrev(const Node* node) const noexcept
	{	
		auto it = before_begin();
		while (it && it.it.node->next != node)
		{
			++it;
		}
		return it.it.node;
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::GetMiddle() const noexcept
	{
		auto ret = begin();
		for (size_type i = 0; i < Size() / 2 - 1; i++, ++ret);
		return ret.it;
	}

	template<typename T>
	template<std::predicate<T, T> LessThan>
	inline typename SList<T>::Node*SList<T>::Merge(Node* a, Node* b, const LessThan less)
	{
		// These base cases will never be hit because we always check IsEmpty before calling this.
		// For completeness of the algorithm, the code is still included, just commented out.
		//if (!a)
		//{
		//	return b;
		//}
		//if (!b)
		//{
		//	return a;
		//}

		// Save the head for returning.
		Node* head;
		if (less(a->data, b->data))
		{
			head = a;
		}
		else
		{
			head = b;
			// Make sure 1 starts at the actual head.
			b = a;
			a = head;
		}

		while (a->next)
		{
			if (less(b->data, a->next->data))
			{				
				auto temp = a->next;
				a->next = b;
				b = temp;
			}
			a = a->next;
		}
		a->next = b;
		
		return head;
	}

	template<typename T>
	inline typename SList<T>::size_type SList<T>::Delete(Node* node)
	{
		return DeleteUntil(node, nullptr);
	}

	template<typename T>
	inline typename SList<T>::size_type SList<T>::DeleteUntil(Node* node, const Node* until)
	{
		size_type ret = 0;
		for (auto prev = node; node != until; prev = node, ret++)
		{
			node = node->next;
			delete prev;
		}
		return ret;
	}

	template<typename T>
	inline void SList<T>::AssertOwner([[maybe_unused]] const iterator it) const
	{
		assertm(it.owner == this, "iterator does not belong to this SList");
	}

	template<typename T>
	inline void SList<T>::AssertOwner(const const_iterator it) const
	{
		AssertOwner(it.it);
	}

	template<typename T>
	inline void SList<T>::ThrowEmpty() const
	{
		if (IsEmpty())
		{
			throw std::out_of_range("Cannot dereference empty container");
		}
	}

	template<typename T>
	inline void SList<T>::ThrowIndex(const size_type index) const
	{
		if (index >= Size())
		{
			throw std::out_of_range(std::to_string(index) + " is beyond this container's size of " + std::to_string(Size()));
		}
	}
	
	template<typename T>
	inline void SList<T>::Copy(const SList& other)
	{
		if (!other.IsEmpty())
		{
			Node* node = other.head;
			head = tail = new Node(node->data);
			while (node->next)
			{
				node = node->next;
				tail->next = new Node(node->data);
				tail = tail->next;
			}
		}
	}

	template<typename T>
	inline void SList<T>::SetPostMoveState() noexcept
	{
		head = tail = nullptr;
		size = 0;
	}
#pragma endregion
}
