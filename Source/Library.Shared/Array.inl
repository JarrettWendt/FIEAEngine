// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Array.h"

namespace Library
{
#pragma region Constructors
	TEMPLATE
	inline ARRAY::Array(const size_t capacity) :
		array(Memory::Malloc<T>(capacity)),
		capacity(internal_size_type(capacity)) {}

	TEMPLATE
	inline ARRAY::Array(const size_t count, const T& prototype) :
		array(Memory::Malloc<T>(count)),
		size(internal_size_type(count)),
		capacity(internal_size_type(count))
	{
		for (size_t i = 0; i < count; i++)
		{
			new (array + i) T(prototype);
		}
	}

	TEMPLATE
	template<std::forward_iterator It>
	inline ARRAY::Array(const It first, const It last)
	{
		PushBack(first, last);
	}

	TEMPLATE
	template<std::random_access_iterator It>
	inline ARRAY::Array(const It first, const It last) :
		Array(std::distance(first, last))
	{
		PushBackNoResize(first, last);
	}

	TEMPLATE
	template<Concept::RangeOf<T> Range>
	inline ARRAY::Array(const Range& range) :
		Array(Util::GetSize(range))
	{
		PushBackNoResize(range.begin(), range.end());
	}

	TEMPLATE
	template<Concept::RangeOf<T> Range>
	inline ARRAY& ARRAY::operator=(const Range& range)
	{
		Clear();
		Reserve(Util::GetSize(range));
		PushBackNoResize(range.begin(), range.end());
		return *this;
	}

	TEMPLATE
	inline ARRAY::Array(const std::initializer_list<T> list) :
		array(Memory::Malloc<T>(list.size())),
		capacity(internal_size_type(list.size()))
	{
		Copy(list);
	}

	TEMPLATE
	inline ARRAY& ARRAY::operator=(const std::initializer_list<T> list)
	{
		Clear();
		Reserve(list.size());
		Copy(list);
		return *this;
	}

	TEMPLATE
	inline ARRAY::Array(const Array& other) :
		array(Memory::Malloc<T>(other.Capacity())),
		size(other.size),
		capacity(other.capacity)
	{
		Copy(other);
	}

	TEMPLATE
	inline ARRAY::Array(Array&& other) noexcept :
		array(other.array),
		size(other.size),
		capacity(other.capacity)
	{
		other.SetPostMoveState();
	}

	TEMPLATE
	inline ARRAY& ARRAY::operator=(const Array& other)
	{
		return CopyAssign(other);
	}

	TEMPLATE
	inline ARRAY& ARRAY::operator=(Array&& other) noexcept
	{
		return MoveAssign(std::move(other));
	}

	TEMPLATE
	template<Concept::ReserveStrategy OtherReserveStrategy>
	inline ARRAY::Array(const Array<T, OtherReserveStrategy>& other) :
		array(Memory::Malloc<T>(other.Capacity())),
		size(internal_size_type(other.Size())),
		capacity(internal_size_type(other.Capacity()))
	{
		Copy(other);
	}

	TEMPLATE
	template<Concept::ReserveStrategy OtherReserveStrategy>
	inline ARRAY::Array(Array<T, OtherReserveStrategy>&& other) noexcept :
		array(other.Data()),
		size(internal_size_type(other.Size())),
		capacity(internal_size_type(other.Capacity()))
	{
		other.TakeData();
	}

	TEMPLATE
	template<Concept::ReserveStrategy OtherReserveStrategy>
	inline ARRAY& ARRAY::operator=(const Array<T, OtherReserveStrategy>& other)
	{
		return CopyAssign(other);
	}

	TEMPLATE
	template<Concept::ReserveStrategy OtherReserveStrategy>
	inline ARRAY& ARRAY::operator=(Array<T, OtherReserveStrategy>&& other) noexcept
	{
		return MoveAssign(std::move(other));
	}

	TEMPLATE
	inline ARRAY::~Array()
	{
		Empty();
	}
#pragma endregion

#pragma region iterator
	TEMPLATE
	inline typename ARRAY::iterator ARRAY::iterator::operator+(const difference_type i) const noexcept
	{
		AssertInitialized();
#ifdef _DEBUG
		return iterator(data + i, owner);
#else

		return iterator(data + i, nullptr);
#endif
	}

	TEMPLATE
	inline typename ARRAY::iterator::reference ARRAY::iterator::operator*() const
	{
		return *data;
	}

	TEMPLATE
	inline bool ARRAY::iterator::operator==(const iterator& other) const noexcept
	{
		return data == other.data;
	}

	TEMPLATE
	inline bool ARRAY::iterator::operator<(const iterator& other) const noexcept
	{
		AssertInitialized();
		return data < other.data;
	}

	TEMPLATE
	inline constexpr void ARRAY::iterator::AssertInitialized() const noexcept
	{
		assertm(owner, "Array::iterator is uninitialized");
		assertm(data, "Array::iterator is uninitialized");
	}

	TEMPLATE
	inline typename ARRAY::iterator ARRAY::begin() noexcept
	{
		return iterator(size_type(0), *this);
	}

	TEMPLATE
	inline typename ARRAY::iterator ARRAY::end() noexcept
	{
		return iterator(Size(), *this);
	}
#pragma endregion

#pragma region Properties
	TEMPLATE
	inline constexpr bool ARRAY::IsEmpty() const noexcept
	{
		return Size() == 0;
	}

	TEMPLATE
	inline constexpr bool ARRAY::IsFull() const noexcept
	{
		assertm(Size() <= Capacity(), "Array Size() has gone over Capacity()");
		return Size() == Capacity();
	}

	TEMPLATE
	inline constexpr size_t ARRAY::Size() const noexcept
	{
		return size;
	}

	TEMPLATE
	inline constexpr size_t ARRAY::Capacity() const noexcept
	{
		return capacity;
	}

	TEMPLATE
	inline constexpr typename ARRAY::size_type ARRAY::IndexOf(const const_iterator it) const noexcept
	{
		return it.it.data - array;
	}
#pragma endregion

#pragma region Element Access
	TEMPLATE
	inline T& ARRAY::At(const size_t index)
	{
		if (index >= Size())
		{
			throw std::out_of_range(std::to_string(index) + " is beyond Array Size() of " + std::to_string(Size()));
		}
		return array[index];
	}
	
	TEMPLATE
	inline const T& ARRAY::At(const size_t index) const
	{
		return const_cast<Array*>(this)->At(index);
	}
	
	TEMPLATE
	inline T& ARRAY::operator[](const size_t index)
	{
		return At(index);
	}
	
	TEMPLATE
	inline const T& ARRAY::operator[](const size_t index) const
	{
		return const_cast<Array*>(this)->operator[](index);
	}
	
	TEMPLATE
	inline T& ARRAY::Front()
	{
		return At(0);
	}
	
	TEMPLATE
	inline const T& ARRAY::Front() const
	{
		return const_cast<Array*>(this)->Front();
	}
	
	TEMPLATE
	inline T& ARRAY::Back()
	{
		return At(Size() - 1);
	}
	
	TEMPLATE
	inline const T& ARRAY::Back() const
	{
		return const_cast<Array*>(this)->Back();
	}
#pragma endregion

#pragma region Insert
	TEMPLATE
	inline void ARRAY::Insert(const size_t index, const T& t)
	{
		Emplace(index, t);
	}

	TEMPLATE
	inline void ARRAY::Insert(const size_t index, T&& t)
	{
		Emplace(index, std::move(t));
	}

	TEMPLATE
	inline typename ARRAY::iterator ARRAY::Insert(const size_t index, const size_t count, const T& t)
	{
		for (internal_size_type i = 0; i < count; i++)
		{
			Emplace(index + i, t);
		}
		return iterator(index + count, *this);
	}

	TEMPLATE
	inline typename ARRAY::iterator ARRAY::Insert(const size_t index, std::initializer_list<T> list)
	{
		return Insert(index, list.begin(), list.end());
	}
	
	TEMPLATE
	inline void ARRAY::Insert(const const_iterator it, const T& t)
	{
		AssertOwner(it);
		Insert(IndexOf(it), t);
	}

	TEMPLATE
	inline void ARRAY::Insert(const const_iterator it, T&& t)
	{
		AssertOwner(it);
		return Insert(IndexOf(it), std::move(t));
	}

	TEMPLATE
	inline typename ARRAY::iterator ARRAY::Insert(const const_iterator it, const size_t count, const T& t)
	{
		AssertOwner(it);
		return Insert(IndexOf(it), count, t);
	}

	TEMPLATE
	inline typename ARRAY::iterator ARRAY::Insert(const const_iterator it, std::initializer_list<T> list)
	{
		AssertOwner(it);
		return Insert(IndexOf(it), list);
	}
	
	TEMPLATE
	template<std::forward_iterator It>
	inline typename ARRAY::iterator ARRAY::Insert(const size_t index, It first, const It last)
	{
		if constexpr (std::is_same_v<It, iterator>)
		{
			assertm(first.owner != this, "iterator cannot be to this container");
		}
		else if constexpr (std::is_same_v<It, const_iterator>)
		{
			assertm(first.it.owner != this, "iterator cannot be to this container");
		}

		// If we can get a size out of this iterator, lets allocate space ahead of time.
		// No need to divide by sizeof(T), C++ auto-casts the pointer type for us.
		if constexpr (std::random_access_iterator<It> || std::is_pointer_v<It>)
		{
			Reserve(std::distance(first, last));
		}
		
		size_type i = 0;
		while (first != last)
		{
			Emplace(i++, *first++);
		}
		return iterator(index + i, *this);
	}

	TEMPLATE
	template<std::forward_iterator It>
	inline typename ARRAY::iterator ARRAY::Insert(const const_iterator it, const It first, const It last)
	{
		AssertOwner(it);
		return Insert(IndexOf(it), first, last);
	}

	TEMPLATE
	template<typename ...Args>
	inline T& ARRAY::Emplace(const size_t index, Args&& ...args)
	{
		if (index > Size())
		{
			throw std::out_of_range("Cannot emplace at index " + std::to_string(index) + " in Size() " + std::to_string(Size()) + " Array");
		}

		if (IsFull())
		{
			// A little extra work here to make sure we're not moving data twice.
			const size_type newCapacity = InvokeReserveStrategy();
			T* newArray = Memory::Malloc<T>(newCapacity);

			// This is how we avoid the extra move. By copying things after index one space ahead.
			Memory::Memcpy(newArray, array, index);
			Memory::Memcpy(newArray + index + 1, array + index, size - index);

			Memory::Free(array);
			array = newArray;
			capacity = internal_size_type(newCapacity);
			size++;
		}
		else
		{
			ShiftRight(index);
		}
		new (array + index) T(std::forward<Args>(args)...);
		return array[index];
	}

	TEMPLATE
	template<typename ...Args>
	inline T& ARRAY::Emplace(const const_iterator it, Args&& ...args)
	{
		AssertOwner(it);
		return Emplace(IndexOf(it), std::forward<Args>(args)...);
	}

	TEMPLATE
	template<typename ...Args>
	inline T& ARRAY::EmplaceBack(Args&& ...args)
	{
		if (IsFull())
		{
			Reserve(InvokeReserveStrategy());
		}
		new (array + size) T(std::forward<Args>(args)...);
		return array[size++];
	}

	TEMPLATE
	template<typename ...Args>
	inline T& ARRAY::EmplaceFront(Args&& ...args)
	{
		return Emplace(0, std::forward<Args>(args)...);
	}

	TEMPLATE
	inline void ARRAY::PushBack(const T& t)
	{
		EmplaceBack(t);
	}
	
	TEMPLATE
	inline void ARRAY::PushBack(T&& t)
	{
		EmplaceBack(std::move(t));
	}

	TEMPLATE
	inline void ARRAY::PushFront(const T& t)
	{
		EmplaceFront(t);
	}
	
	TEMPLATE
	inline void ARRAY::PushFront(T&& t)
	{
		EmplaceFront(std::move(t));
	}
	
	TEMPLATE
	template<std::forward_iterator It>
	inline void ARRAY::PushBack(const It first, const It last)
	{
		Reserve(Size() + std::distance(first, last));
		PushBackNoResize(first, last);
	}

	TEMPLATE
	template<std::forward_iterator It>
	inline void ARRAY::PushFront(It first, const It last)
	{
		const auto dist = std::distance(first, last);
		Reserve(Size() + dist);
		ShiftRight(0, dist);
		for (internal_size_type i = 0; first != last; i++, ++first)
		{
			new (array + i) T(*first);
		}
	}

	TEMPLATE
		inline void ARRAY::Fill(const T& prototype)
	{
		for (internal_size_type i = size; i < capacity; i++)
		{
			new (array + i) T(prototype);
		}
		size = capacity;
	}
#pragma endregion

#pragma region Remove
	TEMPLATE
	inline bool ARRAY::Remove(const T& t)
	{
		return Remove([&t](const auto& a) { return t == a; });
	}

	TEMPLATE
	template<std::predicate<T> Predicate>
	inline bool ARRAY::Remove(const Predicate predicate)
	{
		for (internal_size_type i = 0; i < Size(); i++)
		{
			if (predicate(array[i]))
			{
				array[i].~T();
				ShiftLeft(i);
				return true;
			}
		}
		return false;
	}

	TEMPLATE
	inline void ARRAY::RemoveAt(const size_t index)
	{
		Remove(index, index + 1);
	}
	
	TEMPLATE
	inline size_t ARRAY::RemoveAll(const T& t)
	{
		return RemoveAll([&t](const auto& a) { return t == a; });
	}

	TEMPLATE
		template<std::predicate<T> Predicate>
	inline size_t ARRAY::RemoveAll(const Predicate predicate)
	{
		const size_type oldSize = size;
		Remove(iterator(std::remove_if(begin(), end(), predicate)), end());
		return oldSize - size;
	}
	
	TEMPLATE
	inline size_t ARRAY::Remove(size_type first, size_type last)
	{
		if (first > last)
		{
			throw std::invalid_argument("Must pass indices in increasing order.");
		}
		
		last = std::min(last, size_type(size));
		first = std::min(first, size_type(size));
		const size_type ret = last - first;

		// Remove these elements by destroying them.
		DestructAll(first, last);
		ShiftLeft(first, ret);
		return ret;
	}

	TEMPLATE
	inline size_t ARRAY::Remove(const_iterator first, const_iterator last)
	{
		AssertOwner(first);
		AssertOwner(last);
		const size_type ret = Remove(first.it.data - array, last.it.data - array);
		return ret;
	}

	TEMPLATE
	inline void ARRAY::PopBack()
	{
		if (!IsEmpty())
		{
			array[--size].~T();
		}
	}
	
	TEMPLATE
	inline void ARRAY::PopFront()
	{
		if (!IsEmpty())
		{
			array[0].~T();
			ShiftLeft();
		}
	}
	
	TEMPLATE
	inline void ARRAY::Clear() noexcept
	{
		DestructAll();
		size = 0;
	}
	
	TEMPLATE
	inline void ARRAY::Empty() noexcept
	{
		Clear();
		ShrinkToFit();
	}
#pragma endregion

#pragma region Query
	TEMPLATE
	inline size_t ARRAY::IndexOf(const T& t) const
	{
		return Util::Find(*this, t).data - array;
	}

	TEMPLATE
	template<std::predicate<T> Predicate>
	inline size_t ARRAY::IndexOf(const Predicate predicate) const
	{
		return Util::Find(*this, predicate).data - array;
	}
#pragma endregion

#pragma region Memory	
	TEMPLATE
	inline T* ARRAY::Data() noexcept
	{
		return array;
	}
	
	TEMPLATE
	inline const T* ARRAY::Data() const noexcept
	{
		return const_cast<Array*>(this)->Data();
	}

	TEMPLATE
	inline std::tuple<gsl::owner<T*>, size_t, size_t> ARRAY::TakeData() noexcept
	{
		const std::tuple<gsl::owner<T*>, size_type, size_type> ret = { array, size, capacity };
		SetPostMoveState();
		return ret;
	}

	TEMPLATE
	inline void ARRAY::SetData(std::tuple<T*, size_type, size_type> data) noexcept
	{
		SetData(std::get<0>(data), std::get<1>(data), std::get<2>(data));
	}

#pragma warning(push)
#pragma warning(disable: 4458)
	TEMPLATE
	inline void ARRAY::SetData(T* array, const size_type size, const size_type capacity) noexcept
	{
		Clear();
		ShrinkToFit();
		this->array = array;
		this->size = internal_size_type(size);
		this->capacity = internal_size_type(capacity);
	}
#pragma warning(pop)
	
	TEMPLATE
	inline void ARRAY::Reserve(const size_type newCapacity)
	{
		if (newCapacity > Capacity())
		{
			Memory::Realloc(array, newCapacity);
			capacity = internal_size_type(newCapacity);
		}
	}

	TEMPLATE
	inline void ARRAY::Resize(size_type newSize, const T& prototype)
	{
		DestructAll(newSize, size);
		Memory::Realloc(array, newSize);
		capacity = internal_size_type(newSize);
		Fill(prototype);
	}

	TEMPLATE
	inline void ARRAY::ShrinkToFit()
	{
		ShrinkToFit(size);
	}

	TEMPLATE
	inline void ARRAY::ShrinkToFit(const size_type count)
	{
		// No operation to perform if we're already shrunk.
		if (Capacity() != count)
		{
			capacity = internal_size_type(std::max(count, Size()));
			Memory::Realloc(array, capacity);
		}
	}

	TEMPLATE
	inline void ARRAY::Reverse() noexcept
	{
		std::reverse(begin(), end());
	}

	TEMPLATE
	template<Concept::ReserveStrategy OtherReserveStrategy>
	inline void ARRAY::Swap(Array<T, OtherReserveStrategy>& other) noexcept
	{
		std::swap(*this, reinterpret_cast<Array&>(other));
	}
#pragma endregion

#pragma region Helpers
	TEMPLATE
	template<std::forward_iterator It>
	inline void ARRAY::PushBackNoResize(It first, It last)
	{
		while (first != last)
		{
			new (array + size++) T(*first++);
		}
	}
	
	TEMPLATE
	template<typename OtherReserveStrategy>
	inline ARRAY& ARRAY::CopyAssign(const Array<T, OtherReserveStrategy>& other)
	{
		if (this != reinterpret_cast<const Array*>(&other))
		{
			Clear();
			Reserve(other.Capacity());
			Copy(other);
		}
		return *this;
	}

	TEMPLATE
	template<typename OtherReserveStrategy>
	inline ARRAY& ARRAY::MoveAssign(Array<T, OtherReserveStrategy>&& other)
	{
		if (this != reinterpret_cast<Array*>(&other))
		{
			Empty();

			auto [a, s, c] = other.TakeData();
			array = a;
			size = internal_size_type(s);
			capacity = internal_size_type(c);
		}
		return *this;
	}
	
	TEMPLATE
	inline void ARRAY::AssertOwner([[maybe_unused]] const iterator it) const
	{
		assertm(it.owner == this, "iterator does not belong to this Array");
	}

	TEMPLATE
	inline void ARRAY::AssertOwner(const const_iterator it) const
	{
		AssertOwner(it.it);
	}

	TEMPLATE
	inline size_t ARRAY::InvokeReserveStrategy() const noexcept
	{
		return std::max(ReserveStrategy{}(size, capacity), size_type(size) + 1);
	}
	
	TEMPLATE
	inline void ARRAY::DestructAll() noexcept
	{
		DestructAll(0, size);
	}

	TEMPLATE
	inline void ARRAY::DestructAll(size_type first, const size_type last) noexcept
	{
		while (first < last)
		{
			array[first++].~T();
		}
	}

	TEMPLATE
	inline void ARRAY::ShiftRight(size_type startIndex, size_type shiftAmount) noexcept
	{
		Memory::Memmove(array + startIndex + shiftAmount, array + startIndex, std::max(difference_type(0), difference_type(size) - difference_type(shiftAmount) - difference_type(startIndex) + difference_type(1)));
		size += internal_size_type(shiftAmount);
	}

	TEMPLATE
	inline void ARRAY::ShiftLeft(size_type startIndex, size_type shiftAmount) noexcept
	{
		Memory::Memmove(array + startIndex, array + startIndex + shiftAmount, size - shiftAmount - startIndex);
		size -= internal_size_type(shiftAmount);
	}
	
	TEMPLATE
	inline void ARRAY::Copy(const std::initializer_list<T> list)
	{
		for (const auto& t : list)
		{
			PushBack(t);
		}
	}
	
	TEMPLATE
	template<typename OtherReserveStrategy>
	inline void ARRAY::Copy(const Array<T, OtherReserveStrategy>& other)
	{
		size = internal_size_type(other.Size());
		for (internal_size_type i = 0; i < size; i++)
		{
			new (array + i) T(other[i]);
		}
	}

	TEMPLATE
	inline void ARRAY::SetPostMoveState()
	{
		array = nullptr;
		size = capacity = 0;
	}
#pragma endregion
}

#undef TEMPLATE
#undef ARRAY
