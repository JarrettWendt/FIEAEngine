// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "VariantArray.h"

namespace Library
{
#pragma region value_type
	template<typename ...Ts>
	inline VariantArray<Ts...>::value_type::value_type(const size_type index, VariantArray& owner) :
		index(index),
		owner(owner) {}

	template<typename ...Ts>
	inline constexpr size_t VariantArray<Ts...>::value_type::TypeID() const noexcept
	{
		return owner.TypeID();
	}
	
	template<typename ...Ts>
	template<typename T>
	inline typename VariantArray<Ts...>::value_type& VariantArray<Ts...>::value_type::operator=(const T& t)
	{
		owner.Get<T>(index) = t;
		return *this;
	}

	template<typename ...Ts>
	template<typename T>
	inline VariantArray<Ts...>::value_type::operator T& ()
	{
		return owner.Get<T>(index);
	}

	template<typename ...Ts>
	template<typename T>
	inline VariantArray<Ts...>::value_type::operator const T& () const
	{
		return owner.Get<T>(index);
	}
	
	template<typename ...Ts>
	template<typename T>
	inline constexpr bool VariantArray<Ts...>::value_type::IsType() const noexcept
	{
		return owner.IsType<T>();
	}

	template<typename ...Ts>
	bool VariantArray<Ts...>::value_type::operator==(const value_type& other) const noexcept
	{
		if (&owner == &other.owner && index == other.index)
		{
			return true;
		}
#pragma warning(push)
#pragma warning(disable: 4805)	// '==': unsafe use of type 'const T' and 'const T' in operation
		return std::visit([i = index, j = other.index](const auto& a, const auto& b)
		{
			using A = decltype(a);
			using B = decltype(b);
			using AVal = decltype(a[i]);
			using BVal = decltype(b[j]);
			constexpr bool aComparable = std::equality_comparable<AVal>;
			constexpr bool bComparable = std::equality_comparable<BVal>;

			if constexpr (std::equality_comparable_with<AVal, BVal>)
			{
				return a[i] == b[j];
			}
			// The order here is to make sure we convert with the following priorities:
			// 1.	nothrow conversions have priority over throwing conversions
			// 2.	convert right before converting left
			else if constexpr (aComparable && std::is_nothrow_convertible_v<B, A>)
			{
				return a[i] == static_cast<const A&>(b[i]);
			}
			else if constexpr (bComparable && std::is_nothrow_convertible_v<A, B>)
			{
				return static_cast<const B&>(a[i]) == b[i];
			}
			else if constexpr (aComparable && std::is_convertible_v<B, A>)
			{
				return a[i] == static_cast<const A&>(b[i]);
			}
			else if constexpr (bComparable && std::is_convertible_v<A, B>)
			{
				return static_cast<const B&>(a[i]) == b[i];
			}
			else
			{
				return false;
			}
		}, owner.variants, other.owner.variants);
#pragma warning(pop)
	}

	template<typename ...Ts>
	bool VariantArray<Ts...>::value_type::operator<(const value_type& other) const noexcept
	{
		if (owner == other.owner && index == other.index)
		{
			return false;
		}

#pragma warning(push)
#pragma warning(disable: 4804)	// '<': unsafe use of type 'bool' in operation
		return std::visit([i = index, j = other.index]([[maybe_unused]] const auto& a, [[maybe_unused]] const auto& b)
		{
			using A = decltype(a);
			using B = decltype(b);
			using AVal = decltype(a[i]);
			using BVal = decltype(b[i]);
			constexpr bool aComparable = Concept::LessThanComparable<AVal>;
			constexpr bool bComparable = Concept::LessThanComparable<BVal>;
			
			if constexpr (Concept::LessThanComparable<AVal, BVal>)
			{
				return a[i] < b[j];
			}
			// The order here is to make sure we convert with the following priorities:
			// 1.	nothrow conversions have priority over throwing conversions
			// 2.	convert right before converting left
			else if constexpr (aComparable && std::is_nothrow_convertible_v<B, A>)
			{
				return a[i] < static_cast<const A&>(b[i]);
			}
			else if constexpr (bComparable && std::is_nothrow_convertible_v<A, B>)
			{
				return static_cast<const B&>(a[i]) < b[i];
			}
			else if constexpr (aComparable && std::is_convertible_v<B, A>)
			{
				return a[i] < static_cast<const A&>(b[i]);
			}
			else if constexpr (bComparable && std::is_convertible_v<A, B>)
			{
				return static_cast<const B&>(a[i]) < b[i];
			}
			else
			{
				return false;
			}
		}, owner.variants, other.owner.variants);
#pragma warning(pop)
	}
#pragma endregion
	
#pragma region Special Members
	template<typename ...Ts>
	template<typename T>
	inline VariantArray<Ts...>::VariantArray(const size_type count, const T& prototype) :
		variants(InPlaceType<T>(), count, prototype) {}

	template<typename ...Ts>
	template<std::forward_iterator It>
	inline VariantArray<Ts...>::VariantArray(const It first, const It last) :
		variants(InPlaceType<typename It::value_type>(), first, last) {}

	template<typename ...Ts>
	template<std::random_access_iterator It>
	inline VariantArray<Ts...>::VariantArray(const It first, const It last) :
		variants(InPlaceType<typename It::value_type>(), first, last) {}

	template<typename ...Ts>
	template<std::ranges::range Range>
	inline VariantArray<Ts...>::VariantArray(const Range& range) :
		VariantArray(range.begin(), range.end()) {}

	template<typename ...Ts>
	template<typename T>
	inline VariantArray<Ts...>::VariantArray(std::initializer_list<T> list) :
		variants(InPlaceType<T>(), *reinterpret_cast<std::initializer_list<Util::BestMatch<T, Ts...>>*>(&list)) {}
	
	template<typename ...Ts>
	template<typename T>
	inline VariantArray<Ts...>& VariantArray<Ts...>::operator=(const T& t)
	{
		Assign(t);
		return *this;
	}
#pragma endregion

#pragma region iterator
#pragma region non-const
	template <typename ... Ts>
	typename VariantArray<Ts...>::iterator VariantArray<Ts...>::iterator::operator+(const difference_type i) const noexcept
	{
		AssertInitialized();
		// prevent overflow
		if (i > difference_type(owner->Size()))
		{
			return owner->end();
		}
		auto newIndex = difference_type(index) + i;
		newIndex = std::max(newIndex, difference_type(0));
		newIndex = std::min(size_type(newIndex), owner->Size());
		return iterator(newIndex, *owner);
	}

	template<typename ...Ts>
	inline typename VariantArray<Ts...>::iterator::reference VariantArray<Ts...>::iterator::operator*() const
	{
		return value_type(index, *owner);
	}

	template<typename ...Ts>
	inline typename VariantArray<Ts...>::iterator::reference VariantArray<Ts...>::iterator::operator[](const difference_type i) const noexcept
	{
		return *(*this + i);
	}

	template<typename ...Ts>
	inline bool VariantArray<Ts...>::iterator::operator==(const iterator& other) const noexcept
	{
		AssertInitialized();
		return owner == other.owner && index == other.index;
	}

	template<typename ...Ts>
	inline bool VariantArray<Ts...>::iterator::operator<(const iterator& other) const noexcept
	{
		AssertInitialized();
		return owner == other.owner && index < other.index;
	}

	template<typename ...Ts>
	inline VariantArray<Ts...>::iterator::operator bool() const noexcept
	{
		return owner && !IsAtEnd();
	}

	template<typename ...Ts>
	inline bool VariantArray<Ts...>::iterator::operator!() const noexcept
	{
		return !owner || IsAtEnd();
	}

	template<typename ...Ts>
	inline bool VariantArray<Ts...>::iterator::IsAtEnd() const noexcept
	{
		AssertInitialized();
		return index >= owner->Size();
	}

	template<typename ...Ts>
	inline bool VariantArray<Ts...>::iterator::IsAtBegin() const noexcept
	{
		return index <= 0;
	}

	template<typename ...Ts>
	inline constexpr void VariantArray<Ts...>::iterator::AssertInitialized() const noexcept
	{
		assertm(owner, "Array::iterator is uninitialized");
	}
#pragma endregion

#pragma region const
	template<typename ...Ts>
	inline VariantArray<Ts...>::const_iterator::operator bool() const noexcept
	{
		return it.operator bool();
	}

	template<typename ...Ts>
	inline bool VariantArray<Ts...>::const_iterator::operator!() const noexcept
	{
		return it.operator!();
	}

	template<typename ...Ts>
	inline bool VariantArray<Ts...>::const_iterator::IsAtEnd() const noexcept
	{
		return it.IsAtEnd();
	}

	template<typename ...Ts>
	inline bool VariantArray<Ts...>::const_iterator::IsAtBegin() const noexcept
	{
		return it.IsAtBegin();
	}
#pragma endregion

	template <typename ... Ts>
	typename VariantArray<Ts...>::iterator VariantArray<Ts...>::begin() noexcept
	{
		return iterator(0, *this);
	}

	template <typename ... Ts>
	typename VariantArray<Ts...>::iterator VariantArray<Ts...>::end() noexcept
	{
		return iterator(Size(), *this);
	}
#pragma endregion
	
#pragma region Properties
	template<typename ...Ts>
	inline constexpr size_t VariantArray<Ts...>::Capacity() const noexcept
	{
		return Visit([](const auto& v) { return v.Capacity(); });
	}

	template<typename ...Ts>
	inline constexpr size_t VariantArray<Ts...>::Size() const noexcept
	{
		return Visit([](const auto& v) { return v.Size(); });
	}

	template<typename ...Ts>
	inline constexpr bool VariantArray<Ts...>::IsEmpty() const noexcept
	{
		return Size() <= 0;
	}

	template<typename ...Ts>
	inline constexpr bool VariantArray<Ts...>::IsFull() const noexcept
	{
		assertm(Size() <= Capacity(), "VariantArray Size() has gone over Capacity()");
		return Size() == Capacity();
	}

	template<typename ...Ts>
	template<typename T>
	inline constexpr bool VariantArray<Ts...>::IsType() const noexcept
	{
		return std::get_if<Array<Util::BestMatch<T, Ts...>>>(&variants);
	}

	template<typename ...Ts>
	inline constexpr size_t VariantArray<Ts...>::TypeID() const noexcept
	{
		return variants.valueless_by_exception() ? 0 : variants.index() + 1;
	}

	template<typename ...Ts>
	template<typename T>
	inline constexpr size_t VariantArray<Ts...>::TypeID() noexcept
	{
		using Type = Util::BestMatch<T, Ts...>;
		if constexpr (!Util::IsConvertible<T, Ts...>() || std::numeric_limits<size_t>::max() == Util::Index<Type, Ts...>())
		{
			return 0;
		}
		else
		{
			return 1 + Util::Index<Type, Ts...>();
		}
	}
#pragma endregion
	
#pragma region Element Access
	template<typename ...Ts>
	template<typename T>
	inline T& VariantArray<Ts...>::Get(const size_t index)
	{
		return At<T>(index);
	}

	template<typename ...Ts>
	template<typename T>
	inline const T& VariantArray<Ts...>::Get(const size_t index) const
	{
		return const_cast<VariantArray*>(this)->Get<T>(index);
	}
	
	template<typename ...Ts>
	template<typename T>
	inline T& VariantArray<Ts...>::At(const size_t index)
	{
		AssertType<T>();
		return GetArray<T>().At(index);
	}
	
	template<typename ...Ts>
	template<typename T>
	inline const T& VariantArray<Ts...>::At(const size_t index) const
	{
		return const_cast<VariantArray*>(this)->At<T>(index);
	}
	
	template<typename ...Ts>
	inline typename VariantArray<Ts...>::value_type VariantArray<Ts...>::operator[](const size_t index)
	{
		return value_type(index, *this);
	}

	template<typename ...Ts>
	inline const typename VariantArray<Ts...>::value_type VariantArray<Ts...>::operator[](const size_t index) const
	{
		return const_cast<VariantArray*>(this)->operator[](index);
	}

	template<typename ...Ts>
	template<typename T>
	inline T& VariantArray<Ts...>::Front()
	{
		return Get<T>();
	}
	
	template<typename ...Ts>
	template<typename T>
	inline const T& VariantArray<Ts...>::Front() const
	{
		return const_cast<VariantArray*>(this)->Front<T>();
	}
	
	template<typename ...Ts>
	template<typename T>
	inline T& VariantArray<Ts...>::Back()
	{
		return Get<T>(Size() - 1);
	}
	
	template<typename ...Ts>
	template<typename T>
	inline const T& VariantArray<Ts...>::Back() const
	{
		return const_cast<VariantArray*>(this)->Back<T>();
	}
#pragma endregion
	
#pragma region Setters	
	template<typename ...Ts>
	template<typename T>
	inline void VariantArray<Ts...>::SetType()
	{
		if (!IsType<T>())
		{
			variants = Array<Util::BestMatch<T, Ts...>>();
		}
	}
	
	template<typename ...Ts>
	template<typename T>
	inline void VariantArray<Ts...>::Set(const T& t, const size_t index)
	{
		if (IsEmpty())
		{
			PushBack(t);
		}
		else
		{
			AssertType<T>();
			GetArray<T>().At(index) = t;
		}
	}
	
	template<typename ...Ts>
	template<typename T>
	inline void VariantArray<Ts...>::Assign(const T& t)
	{
		Set(t);
	}
#pragma endregion
	
#pragma region Insert
	template<typename ...Ts>
	template<typename T>
	inline void VariantArray<Ts...>::Insert(const size_t index, const T& t)
	{
		AssertSetType<T>();
		GetArray<T>().Insert(index, t);
	}
	
	template<typename ...Ts>
	template<typename T>
	inline typename VariantArray<Ts...>::iterator VariantArray<Ts...>::Insert(const size_t index, const size_t count, const T& t)
	{
		AssertSetType<T>();
		auto& a = GetArray<T>();
		const auto it = a.Insert(index, count, t);
		return iterator(a.IndexOf(it), *this);
	}

	template<typename ...Ts>
	template<typename T>
	inline typename VariantArray<Ts...>::iterator VariantArray<Ts...>::Insert(const size_t index, const std::initializer_list<T> list)
	{
		AssertSetType<T>();
		auto& a = GetArray<T>();
		const auto it = a.Insert(index, list);
		return iterator(a.IndexOf(it), *this);
	}

	template<typename ...Ts>
	template<std::forward_iterator It>
	inline typename VariantArray<Ts...>::iterator VariantArray<Ts...>::Insert(const size_t index, const It first, const It last)
	{
		using T = typename It::value_type;
		AssertSetType<T>();
		auto& a = GetArray<T>();
		const auto it = a.Insert(index, first, last);
		return iterator(a.IndexOf(it), *this);
	}

	template<typename ...Ts>
	template<typename T>
	inline void VariantArray<Ts...>::Insert(const const_iterator it, const T& t)
	{
		Insert(it.it.index, t);
	}

	template<typename ...Ts>
	template<typename T>
	inline typename VariantArray<Ts...>::iterator VariantArray<Ts...>::Insert(const const_iterator it, const size_t count, const T& t)
	{
		return Insert(it.it.index, count, t);
	}

	template<typename ...Ts>
	template<typename T>
	inline typename VariantArray<Ts...>::iterator VariantArray<Ts...>::Insert(const const_iterator it, const std::initializer_list<T> list)
	{
		return Insert(it.it.index, list);
	}
	
	template<typename ...Ts>
	template<std::forward_iterator It>
	inline typename VariantArray<Ts...>::iterator VariantArray<Ts...>::Insert(const const_iterator it, const It first, const It last)
	{
		return Insert(it.it.index, first, last);
	}

	template<typename ...Ts>
	template<typename T, typename ...Args>
	inline T& VariantArray<Ts...>::Emplace(const size_t index, Args&& ...args)
	{
		AssertSetType<T>();
		GetArray<T>().Emplace(index, std::forward<Args>(args)...);
		return Get<T>(index);
	}

	template<typename ...Ts>
	template<typename T, typename ...Args>
	inline T& VariantArray<Ts...>::Emplace(const const_iterator it, Args&& ...args)
	{
		return Emplace<T>(it.it.index, std::forward<Args>(args)...);
	}

	template<typename ...Ts>
	template<typename T>
	inline void VariantArray<Ts...>::PushBack(const T& t)
	{
		EmplaceBack<T>(t);
	}

	template<typename ...Ts>
	template<typename T>
	inline void VariantArray<Ts...>::PushBack(T&& t)
	{
		EmplaceBack<T>(std::move(t));
	}

	template<typename ...Ts>
	template<typename T, typename ...Args>
	inline T& VariantArray<Ts...>::EmplaceBack(Args&& ...args)
	{
		AssertSetType<T>();
		return GetArray<T>().EmplaceBack(std::forward<Args>(args)...);
	}

	template<typename ...Ts>
	template<typename T>
	inline void VariantArray<Ts...>::PushFront(const T& t)
	{
		EmplaceFront<T>(t);
	}
	
	template<typename ...Ts>
	template<typename T>
	inline void VariantArray<Ts...>::PushFront(T&& t)
	{
		EmplaceFront<T>(std::move(t));
	}

	template<typename ...Ts>
	template<typename T, typename ...Args>
	inline T& VariantArray<Ts...>::EmplaceFront(Args&& ...args)
	{
		AssertSetType<T>();
		return GetArray<T>().EmplaceFront(std::forward<Args>(args)...);
	}

	template<typename ...Ts>
	template<std::forward_iterator It>
	inline void VariantArray<Ts...>::PushBack(const It first, const It last)
	{
		using T = typename It::value_type;
		AssertSetType<T>();
		GetArray<T>().PushBack(first, last);
	}

	template<typename ...Ts>
	template<std::forward_iterator It>
	inline void VariantArray<Ts...>::PushFront(const It first, const It last)
	{
		using T = typename It::value_type;
		AssertSetType<T>();
		GetArray<T>().PushFront(first, last);
	}

	template<typename ...Ts>
	template<typename T>
	inline void VariantArray<Ts...>::Fill(const T& prototype)
	{
		AssertSetType<T>();
		GetArray<T>().Fill(prototype);
	}
#pragma endregion

#pragma region Remove
	template<typename ...Ts>
	template<typename T>
	inline bool VariantArray<Ts...>::Remove(const T& t)
	{
		return GetArray<T>().Remove(t);
	}

	template<typename ...Ts>
	template<typename T, std::predicate<T> Predicate>
	inline bool VariantArray<Ts...>::Remove(const Predicate predicate)
	{
		return GetArray<T>().Remove(predicate);
	}

	template<typename ...Ts>
	inline void VariantArray<Ts...>::RemoveAt(const size_type index)
	{
		Visit([index](auto& a) { a.RemoveAt(index); });
	}

	template<typename ...Ts>
	template<typename T>
	inline size_t VariantArray<Ts...>::RemoveAll(const T& t)
	{
		return GetArray<T>().RemoveAll(t);
	}

	template<typename ...Ts>
	template<typename T, std::predicate<T> Predicate>
	inline size_t VariantArray<Ts...>::RemoveAll(const Predicate predicate)
	{
		return GetArray<T>().RemoveAll(predicate);
	}

	template<typename ...Ts>
	inline size_t VariantArray<Ts...>::Remove(const size_type first, const size_type last)
	{
		return Visit([first, last](auto& a) { return a.Remove(first, last); });
	}

	template<typename ...Ts>
	inline size_t VariantArray<Ts...>::Remove(const const_iterator first, const const_iterator last)
	{
		return Remove(first.it.index, last.it.index);
	}

	template<typename ...Ts>
	inline void VariantArray<Ts...>::PopBack()
	{
		Visit([](auto& a) { a.PopBack(); });
	}
	
	template<typename ...Ts>
	inline void VariantArray<Ts...>::PopFront()
	{
		Visit([](auto& a) { a.PopFront(); });
	}
	
	template<typename ...Ts>
	inline void VariantArray<Ts...>::Clear() noexcept
	{
		Visit([](auto& a) { a.Clear(); });
	}

	template<typename ...Ts>
	inline void VariantArray<Ts...>::Empty() noexcept
	{
		Visit([](auto& a) { a.Empty(); });
	}
#pragma endregion

#pragma region Query
	template<typename ...Ts>
	template<typename T>
	inline size_t VariantArray<Ts...>::IndexOf(const T& t) const
	{
		return GetArray<T>().IndexOf(t);
	}

	template<typename ...Ts>
	template<typename T, std::predicate<T> Predicate>
	inline size_t VariantArray<Ts...>::IndexOf(const Predicate pred) const
	{
		return GetArray<T>().IndexOf(pred);
	}
#pragma endregion
	
#pragma region Memory
	template<typename ...Ts>
	template<typename T>
	inline constexpr Array<T>& VariantArray<Ts...>::GetArray() noexcept
	{
		auto& array = std::get<Array<Util::BestMatch<T, Ts...>>>(variants);
		return *reinterpret_cast<Array<T>*>(&array);
	}

	template<typename ...Ts>
	template<typename T>
	inline constexpr const Array<T>& VariantArray<Ts...>::GetArray() const noexcept
	{
		return const_cast<VariantArray*>(this)->GetArray<T>();
	}
	
	template<typename ...Ts>
	template<typename T>
	inline void VariantArray<Ts...>::Reserve(const size_t capacity)
	{
		AssertSetType<T>();
		Reserve(capacity);
	}

	template<typename ...Ts>
	inline void VariantArray<Ts...>::Reserve(const size_t capacity)
	{
		Visit([capacity](auto& v) { return v.Reserve(capacity); });
	}

	template<typename ...Ts>
	template<typename T>
	inline void VariantArray<Ts...>::Resize(const size_t capacity, const T& prototype)
	{
		AssertSetType<T>();
		GetArray<T>().Resize(capacity, prototype);
	}

	template<typename ...Ts>
	inline void VariantArray<Ts...>::ShrinkToFit()
	{
		ShrinkToFit(Size());
	}
	
	template<typename ...Ts>
	inline void VariantArray<Ts...>::ShrinkToFit(const size_t count)
	{
		Visit([count](auto& v) { return v.ShrinkToFit(count); });
	}
	
	template<typename ...Ts>
	inline void VariantArray<Ts...>::Reverse() noexcept
	{
		Visit([](auto& v) { v.Reverse(); });
	}
#pragma endregion
	
#pragma region Helpers
	template<typename ...Ts>
	template<typename T>
	inline constexpr auto VariantArray<Ts...>::InPlaceType() noexcept
	{
		return std::in_place_type<Array<Util::BestMatch<T, Ts...>>>;
	}
	
	template<typename ...Ts>
	template<typename T>
	inline void VariantArray<Ts...>::AssertSetType()
	{
		static_assert(Util::IsConvertible<T, Ts...>(), "type is not in variadic template parameters");
		if (!IsType<T>())
		{
			if (IsEmpty())
			{
				SetType<T>();
			}
			else
			{
				throw InvalidTypeException(std::string("Unknown type ") + typeid(T).name());
			}
		}
	}

	template<typename ...Ts>
	template<typename T>
	inline void VariantArray<Ts...>::AssertType() const
	{
		static_assert(Util::IsConvertible<T, Ts...>(), "type is not in variadic template parameters");
		if (!IsType<T>())
		{
			throw InvalidTypeException(std::string("Unknown type ") + typeid(T).name());
		}
	}

	template<typename ...Ts>
	template<typename Callable>
	inline constexpr auto VariantArray<Ts...>::Visit(Callable&& callable)
	{
		return std::visit(std::forward<Callable>(callable), variants);
	}

	template<typename ...Ts>
	template<typename Callable>
	inline constexpr auto VariantArray<Ts...>::Visit(Callable&& callable) const
	{
		return const_cast<VariantArray*>(this)->Visit(std::forward<Callable>(callable));
	}
#pragma endregion
}
