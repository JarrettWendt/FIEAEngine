// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

/**
 * Utility for when invoking other macros.
 * For macros, commas are always treated as delimiters, and symbols such as > are interpreted as "greater than"
 * So a macro invocation like MY_MACRO(std::map<int, int>) pretty much won't do as you expect.
 * This is a fix: MY_MACRO(SINGLE_ARG(std::map<int, int>))
 */
#define SINGLE_ARG(...) __VA_ARGS__

/**
 * Assert with a message.
 * 
 * @param exp   your usual expression you want to assert (optional, will always assert if omitted)
 * @param msg   the message you want to pair with this assert (required)
 */
#ifdef _DEBUG
#include <cassert>
#define assertm1(msg) assert(false && msg)
#define assertm2(exp, msg) assert(exp && msg)
#else
#define assertm1(msg) ;
#define assertm2(exp, msg) ;
#endif
#define GET_ASSERT(_1, _2, NAME, ...) NAME
#define assertm(...) GET_ASSERT(__VA_ARGS__, assertm2, assertm1)(__VA_ARGS__)

/**
 * Cute macro for being able to write a `for EVER` loop.
 */
#define EVER (;;)

 /**
  * Macro for when you want to default or delete the copy ctor and operator=.
  *
  * @param Type		the name of the type
  * @param D		default or delete
  */
#define COPY_SEMANTICS(Type, D)             \
Type(const Type&) = D;					    \
Type& operator=(const Type&) = D;

/**
 * Macro for when you want to default or delete the move ctor and operator=.
 *
 * @param Type		the name of the type
 * @param D			default or delete
 */
#define MOVE_SEMANTICS(Type, D)				\
Type(Type&&) noexcept = D;                  \
Type& operator=(Type&&) noexcept = D;

/**
 * Macro for when you've defined a custom ctor/dtor and want to default the move/copy ctor and operator=.
 *
 * @param Type		the name of the type
  * @param D		default or delete
 */
#define MOVE_COPY(Type, D)                  \
COPY_SEMANTICS(Type, D)                     \
MOVE_SEMANTICS(Type, D)

/**
 * Macro for when you've defined a custom ctor and want to default the (non-virtual) dtor, move/copy ctor, and operator=.
 *
 * @param Type		the name of the type
  * @param D		default or delete
 */
#define MOVE_COPY_DTOR(Type, D)				\
MOVE_COPY(Type, D)							\
~Type() = D;

/**
 * Macro for when you've defined a custom ctor and want to default the (virtual) dtor, move/copy ctor, and operator=.
 *
 * @param Type		the name of the type
  * @param D		default or delete
 */
#define MOVE_COPY_VDTOR(Type, D)			\
MOVE_COPY(Type, D)							\
virtual ~Type() = D;

/**
 * Macro for when you just want to default all the special members.
 *
 * @param Type		the name of the type
  * @param D		default or delete
 */
#define SPECIAL_MEMBERS(Type, D)			\
Type() = D;									\
MOVE_COPY_DTOR(Type, D)

 /**
  * Macro for when you just want to default all the special members with a virtual dtor.
  *
  * @param Type		the name of the type
  * @param D		default or delete
  */
#define SPECIAL_MEMBERS_V(Type, D)		    \
Type() = D;									\
MOVE_COPY_VDTOR(Type, D)

/**
 * For when you want to be more explicit about the fact that you're making a static class.
 *
 * @param Class     the name of your static class
 */
#define STATIC_CLASS(Class)                 \
SPECIAL_MEMBERS(Class, delete)

/**
 * Defines operator!=, operator<=, operator>, and operator>= based off of operator== and operator<
 *
 * @param Class     the class type
 */
#define COMPARISONS(Class)                                          \
[[nodiscard]] bool operator==(const Class& other) const noexcept;   \
[[nodiscard]] bool operator<(const Class& other) const noexcept;    \
[[nodiscard]] bool operator!=(const Class& other) const noexcept    \
{ return !operator==(other); }                                      \
[[nodiscard]] bool operator<=(const Class& other) const noexcept    \
{ return operator==(other) || operator<(other); }                   \
[[nodiscard]] bool operator>(const Class& other) const noexcept     \
{ return operator!=(other) && !operator<=(other); }                 \
[[nodiscard]] bool operator>=(const Class& other) const noexcept    \
{ return operator==(other) || operator>(other); }

/**
 * Defines operator!=, operator<=, operator>, and operator>= based off of operator== and operator<
 *
 * @param Class     the class type
 */
#define FRIEND_COMPARISONS(Class)												\
[[nodiscard]] friend bool operator!=(const Class& a, const Class& b) noexcept   \
{ return !operator==(a, b); }													\
[[nodiscard]] friend bool operator<=(const Class& a, const Class& b) noexcept   \
{ return operator==(a, b) || operator<(a, b); }									\
[[nodiscard]] friend bool operator>(const Class& a, const Class& b) noexcept    \
{ return operator!=(a, b) && !operator<=(a, b); }								\
[[nodiscard]] friend bool operator>=(const Class& a, const Class& b) noexcept   \
{ return operator==(a, b) || operator>(a, b); }

/**
 * Defines all other arithmetic operators for an iterator based off of operator+
 *
 * @param IT        the iterator type
 */
#define RANDOM_ITER_ARITHMETIC_OPS(IT)                                                                  \
IT operator++(int) noexcept { const auto ret = *this; operator++(); return ret; }                       \
IT& operator++() noexcept { *this = operator+(1); return *this; }                                       \
IT operator--(int) noexcept { const auto ret = *this; operator--(); return ret; }                       \
IT& operator--() noexcept { *this = operator-(1); return *this; }                                       \
[[nodiscard]] friend IT operator+(const difference_type i, const IT it) { return it.operator+(i); }     \
[[nodiscard]] IT operator-(const difference_type i) const noexcept { return operator+(-i); }            \
IT& operator+=(const difference_type i) noexcept { return *this = operator+(i); }                       \
IT& operator-=(const difference_type i) noexcept { return *this = operator-(i); }

/**
 * Defines most of the other iterator operators given operator+, operator*, operator==, and operator<
 *
 * @param IT        the iterator type
 */
#define RANDOM_ITER_OPS(IT)                                                                             \
RANDOM_ITER_ARITHMETIC_OPS(IT)                                                                          \
COMPARISONS(IT)                                                                                         \
[[nodiscard]] pointer operator->() const { return &operator*(); }                                       \
[[nodiscard]] reference operator[](const difference_type i) const { return *(*this + i); }

/**
 * Declares all the begin/end cbegin/cend methods for a class.
 * Defines all of them by invoking the non-const begin/end who's definition you must still define.
 *
 * @param IT        the non-const iterator type
 * @param CIT       the const iterator type
 * @param CLASS     the enclosing type for this iterator
 */
#define BEGIN_END(IT, CIT, CLASS)                                                                       \
[[nodiscard]] IT begin() noexcept;                                                                      \
[[nodiscard]] CIT begin() const noexcept { return CIT(const_cast<CLASS*>(this)->begin()); }             \
[[nodiscard]] CIT cbegin() const noexcept { return begin(); }                                           \
[[nodiscard]] IT end() noexcept;                                                                        \
[[nodiscard]] CIT end() const noexcept { return CIT(const_cast<CLASS*>(this)->end()); }                 \
[[nodiscard]] CIT cend() const noexcept { return end(); }

 /**
  * Declares all the rbegin/rend crbegin/crend methods for a class.
  * Defines all of them by calling std::make_reverse_iterator on the other begin/end methods.
  */
#define MAKE_REVERSE_BEGIN_END                                                                          \
[[nodiscard]] auto rbegin() noexcept { return std::make_reverse_iterator(end()); }                      \
[[nodiscard]] auto rbegin() const noexcept { return std::make_reverse_iterator(cend()); }               \
[[nodiscard]] auto crbegin() const noexcept { return std::make_reverse_iterator(cend()); }              \
[[nodiscard]] auto rend() noexcept { return std::make_reverse_iterator(begin()); }                      \
[[nodiscard]] auto rend() const noexcept { return std::make_reverse_iterator(cbegin()); }               \
[[nodiscard]] auto crend() const noexcept { return std::make_reverse_iterator(cbegin()); }

// TODO: Combine these FORWARD vs BIDIRECTIONAL vs RANDOM_ACCESS macros using SFINAE
// For example, conditionally compile operator== iff IT::operator== exists.

/**
 * Macro for defining a const forward iterator.
 * Uses the non-const iterator as its only data member and just calls its methods.
 *
 * @param CIT	the name of this const iterator
 * @param IT	the name of the non-const iterator
 */
#define CONST_FORWARD_ITERATOR(CIT, IT)														            \
private:																					            \
	IT it;																					            \
public:																						            \
	using iterator_category = typename IT::iterator_category;								            \
	using value_type = typename IT::value_type;												            \
	using difference_type = typename IT::difference_type;									            \
	using pointer = const typename IT::pointer;												            \
	using reference = const typename IT::reference;											            \
	CIT(const IT it) noexcept : it(it) {}													            \
	SPECIAL_MEMBERS(CIT, default)															            \
	CIT operator++(int) noexcept { return it.operator++(0); }								            \
	CIT& operator++() noexcept { it.operator++(); return *this; }							            \
	[[nodiscard]] reference operator*() const { return it.operator*(); }							    \
	[[nodiscard]] pointer operator->() const { return it.operator->(); }								\
	[[nodiscard]] bool operator==(const CIT other) const noexcept { return it == other.it; }            \
	[[nodiscard]] bool operator!=(const CIT other) const noexcept { return it != other.it; }

 /**
  * Macro for defining a const bidirectional iterator.
  * Uses the non-const iterator as its only data member and just calls its methods.
  *
  * @param CIT	the name of this const iterator
  * @param IT	the name of the non-const iterator
  */
#define CONST_BIDIRECTIONAL_ITERATOR(CIT, IT)												            \
	CONST_FORWARD_ITERATOR(CIT, IT)															            \
	CIT operator--(int) noexcept { return it.operator--(0); }								            \
	CIT& operator--() noexcept { it.operator--(); return *this; }

  /**
   * Macro for defining a const random access iterator.
   * Uses the non-const iterator as its only data member and just calls its methods.
   *
   * @param CIT	the name of this const iterator
   * @param IT	the name of the non-const iterator
   */
#define CONST_RANDOM_ACCESS_ITERATOR(CIT, IT)												            \
	CONST_BIDIRECTIONAL_ITERATOR(CIT, IT)													            \
	[[nodiscard]] friend difference_type operator-(const CIT lhs, const CIT rhs) noexcept	            \
	{ return lhs.it - rhs.it; }																            \
	[[nodiscard]] friend CIT operator+(const difference_type i, const CIT cit) noexcept 	            \
	{ return cit.operator+(i); }																        \
	[[nodiscard]] CIT operator+(difference_type i) const { return CIT(it.operator+(i)); }				\
	[[nodiscard]] CIT operator-(difference_type i) const { return CIT(it.operator-(i)); }				\
	CIT& operator+=(difference_type i) { it.operator+=(i); return *this; }				                \
	CIT& operator-=(difference_type i) { it.operator-=(i); return *this; }				                \
	[[nodiscard]] reference operator[](difference_type i) const { return it.operator[](i); }			\
	[[nodiscard]] bool operator<(const CIT other) const noexcept { return it < other.it; }	        	\
	[[nodiscard]] bool operator<=(const CIT other) const noexcept { return it <= other.it; }            \
	[[nodiscard]] bool operator>(const CIT other) const noexcept { return it > other.it; }      		\
	[[nodiscard]] bool operator>=(const CIT other) const noexcept { return it >= other.it; }
