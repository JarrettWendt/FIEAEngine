// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "Enum.h"
#include "RTTI.h"
#include "VariantArray.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Transform.h"

namespace Library
{
	/** 
	 * A Datum is merely a VariantArray of predefined types.
	 */
	class Datum final : public VariantArray<
		// primitives
		bool, int, float,

		// math types
		Vector2, Vector3, Vector4, Quaternion, Matrix, Transform,

		// object types
		std::string, std::shared_ptr<RTTI>>
	{
		friend class Attributed;
		
	private:
		using Base = VariantArray;
		// TODO: This bool feels like a big waste because it's taking up 8 bytes due to padding.
		// If we wanted to go *crazy* memory efficient, we could steal a bit from the Array pointer.
		// Another option might be to create a separate type: ExternalDatum.
		// Another another option might be to re-impliment std::variant to use a uint32_t instead of a size_t.
		bool isExternal{ false };
		
	public:
		/**
		 * strong enumeration for all the types Datum can hold
		 */
		enum class Type : uint8_t
		{
			None,

			// primitives
			Bool, Int, Float,

			// math types
			Vector2, Vector3, Vector4, Quaternion, Matrix, Transform,

			// object types
			String, RTTI,
			
			Begin = Bool,
			End = RTTI
		};

	private:
		template<Type T>
		struct TypeOfTypeImpl final
		{
			using type = void;
		};

#define IMPL_TYPE_OF_TYPE(Enum, Type) template<> struct TypeOfTypeImpl<Enum> final { using type = Type; };

		// primitives
		IMPL_TYPE_OF_TYPE(Type::Bool, bool)
		IMPL_TYPE_OF_TYPE(Type::Int, int)
		IMPL_TYPE_OF_TYPE(Type::Float, float)

		// math types
		IMPL_TYPE_OF_TYPE(Type::Vector2, Vector2)
		IMPL_TYPE_OF_TYPE(Type::Vector3, Vector3)
		IMPL_TYPE_OF_TYPE(Type::Vector4, Vector4)
		IMPL_TYPE_OF_TYPE(Type::Quaternion, Quaternion)
		IMPL_TYPE_OF_TYPE(Type::Matrix, Matrix)
		IMPL_TYPE_OF_TYPE(Type::Transform, Transform)

		// object types
		IMPL_TYPE_OF_TYPE(Type::String, std::string)
		IMPL_TYPE_OF_TYPE(Type::RTTI, std::shared_ptr<RTTI>)
		
#undef  IMPL_TYPE_OF_TYPE
		
	public:
		template<Type T>
		using TypeOfType = typename TypeOfTypeImpl<T>::type;

		/**
		 * exception that will be thrown when attempting an operation that would resize the container when using external storage
		 */
		class ExternalStorageException final : public std::runtime_error
		{
		public:
			explicit ExternalStorageException(const std::string& str) : std::runtime_error(str) {};
			SPECIAL_MEMBERS(ExternalStorageException, default)
		};

#pragma region Special Members
		/** inherit all ctors */
		using VariantArray::VariantArray;

		/**
		 * @param type		type to initialize this Datum as
		 * @param count		how many elements to initially reserve space for 
		 */
		Datum(Type type, size_t count = 0) noexcept;

	private:
		/**
		 * ctor for constructing an external Datum
		 *
		 * @param type		the type the array points to
		 * @param array		the array of data to never be resized
		 * @param size		how many elements are in the array
		 * @param capacity	how many elemetns the array can hold (never to change)
		 */
		Datum(Type type, void* array, size_t size, size_t capacity) noexcept;

	public:
		/**
		 * ctor for constructing an external Datum
		 *
		 * @param data		retval of Array::TakeData
		 */
		template<typename T>
		Datum(std::tuple<T*, size_t, size_t> data) noexcept;

		/**
		 * ctor for constructing an external Datum
		 *
		 * @param array		the array of data to never be resized
		 * @param size		how many elements are in the array
		 * @param capacity	how many elemetns the array can hold (never to change)
		 *
		 * @asserts			size <= capacity
		 */
		template<typename T>
		Datum(T* array, size_t size, size_t capacity) noexcept;

		/**
		 * ctor for constructing an external Datum
		 *
		 * @param array		the array of data to never be resized
		 * @param count		how many elements are in the array (also capacity)
		 */
		template<typename T>
		Datum(T* array, size_t count) noexcept;

		/**
		 * copy ctor to handle external Datums
		 * 
		 * @param other		Datum to copy
		 */
		Datum(const Datum& other);

		/**
		 * copy assignment operator to handle external Datums
		 *
		 * @param other		Datum to copy
		 */
		Datum& operator=(const Datum& other);

		/**
		 * Dtor which calls ~VariantArray() iff !IsExternal()
		 */
		~Datum();

		/** forward all operator= to base */
		template<typename T>
		Datum& operator=(const T& t);
#pragma endregion

		/**
		 * Helper for constructing a Datum of a specific type of a specific size.
		 * 
		 * @param capacity		How many elements of space to reserve.
		 */
		template<typename T>
		static Datum Construct(size_t capacity = 0);

		/**
		 * @returns		whether or not this container is using external storage (same as !IsInternal())
		 */
		[[nodiscard]] constexpr bool IsExternal() const noexcept;

		/**
		 * @returns		whether or not this container is using internal storage (same as !IsExternal())
		 */
		[[nodiscard]] constexpr bool IsInternal() const noexcept
		{
			return !IsExternal();
		}

		/**
		 * @returns		the type this container is storing
		 */
		[[nodiscard]] constexpr Type GetType() const noexcept;

		/**
		 * Sets the type for this container to hold.
		 * Will erase all data stored within.
		 *
		 * @param <T>	The new type for this container.
		 * 
		 * @throws ExternalStorageException		if using external storage
		 */
		template<typename T>
		void SetType();

		/**
		 * @returns the Datum::Type of T
		 */
		template<typename T>
		inline static constexpr Type TypeOf = Type(Datum::TypeID<T>());

#pragma region Insert
		/**
		 * check for external storage before forwarding all calls to base 
		 * 
		 * @throws ExternalStorageException		if IsExternal() and IsFull()
		 */
		template<typename... Args>
		auto Insert(Args&&... args);

		/**
		 * check for external storage before forwarding all calls to base
		 * 
		 * @throws ExternalStorageException		if IsExternal() and IsFull()
		 */
		template<typename T>
		void PushBack(const T& t);

		/**
		 * check for external storage before forwarding all calls to base
		 * 
		 * @throws ExternalStorageException		if IsExternal() and IsFull()
		 */
		template<typename T>
		void PushBack(T&& t);

		/**
		 * check for external storage before forwarding all calls to base
		 * 
		 * @throws ExternalStorageException		if IsExternal() and IsFull()
		 */
		template<std::forward_iterator It>
		void PushBack(It first, It last);

		/**
		 * check for external storage before forwarding all calls to base
		 *
		 * @throws ExternalStorageException		if IsExternal() and IsFull()
		 */
		template<typename T>
		void PushFront(const T& t);

		/**
		 * check for external storage before forwarding all calls to base
		 *
		 * @throws ExternalStorageException		if IsExternal() and IsFull()
		 */
		template<typename T>
		void PushFront(T&& t);

		/**
		 * check for external storage before forwarding all calls to base
		 *
		 * @throws ExternalStorageException		if IsExternal() and IsFull()
		 */
		template<std::forward_iterator It>
		void PushFront(It first, It last);

		/**
		 * check for external storage before forwarding all calls to base
		 *
		 * @throws ExternalStorageException		if IsExternal() and IsFull()
		 */
		template<typename T, typename... Args>
		T& EmplaceBack(Args&&... args);

		/**
		 * check for external storage before forwarding all calls to base
		 *
		 * @throws ExternalStorageException		if IsExternal() and IsFull()
		 */
		template<typename T, typename... Args>
		T& EmplaceFront(Args&&... args);
#pragma endregion

		/**
		 * Erases all memory held by this container.
		 * 
		 * @throws ExternalStorageException		if using external storage
		 */
		void Empty() noexcept;
		
#pragma region Memory
		/**
		 * Sets this container's external storage.
		 * If already using external storage, make sure you get a reference to it because this call will abandon that memory without freeing.
		 *
		 * @param <T>		a Datum type
		 * @param data		tuple retrieved from Array::TakeData
		 */
		template<typename T>
		void SetStorage(std::tuple<T*, size_t, size_t> data) noexcept;

		/**
		 * Sets this container's external storage.
		 * If already using external storage, make sure you get a reference to it because this call will abandon that memory without freeing.
		 *
		 * @param <T>		a Datum type
		 * @param array		the array of data to never be resized
		 * @param count		how many elements are in the array
		 * @param capacity	how many elemetns the array can hold (never to change)
		 */
		template<typename T>
		void SetStorage(T* array, size_t count, size_t capacity) noexcept;

		/**
		 * Sets this container's external storage.
		 * If already using external storage, make sure you get a reference to it because this call will abandon that memory without freeing.
		 *
		 * @param <T>		a Datum type
		 * @param array		the array of data to never be resized
		 * @param count		how many elements are in the array (also capacity)
		 */
		template<typename T>
		void SetStorage(T* array, size_t count) noexcept;

	private:
		/**
		 * @param array		new external storage array to reference
		 */
		void SetStorage(void* array) noexcept;
	public:

		/**
		 * Calls SetType<T>() then Reserve(capacity)
		 * O(n) where n is the current size of the container.
		 * Does nothing if newCapacity <= Capacity().
		 *
		 * @param <T>			the type to reserve space for
		 * @param capacity		the new capacity for the container.
		 *
		 * @throws std::length_error			if newCapacity > MaxSize()
		 * @throws std::bad_alloc				if memory allocation fails.
		 * @throws ExternalStorageException		if using external storage
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
		 * @throws ExternalStorageException		if using external storage
		 */
		void Reserve(size_t capacity);

		/**
		 * O(n) where n = newSize - Size(), or n = newSize if the operation requires a Reserve().
		 * If the current Size() is greater than newSize, the container is reduced to its first newSize elements.
		 *
		 * @param newSize		The new size for the container.
		 * @param prototype		The value to initialize each new element with.
		 * 
		 * @throws ExternalStorageException		if using external storage
		 */
		template<typename T>
		void Resize(size_t capacity, const T& prototype = T());

		/**
		 * Reduces memory usage such that Size() == Capacity().
		 * O(n) where n = Size().
		 * Does nothing if IsFull()
		 * 
		 * @throws ExternalStorageException		if using external storage
		 */
		void ShrinkToFit();

		/**
		 * Reduces memory usage such that Capacity() == std::max(count, Size()).
		 * O(n) where n = Size().
		 * Does nothing if Capacity() already == std::max(count, Size()).
		 * 
		 * @throws ExternalStorageException		if using external storage
		 */
		void ShrinkToFit(size_t count);
#pragma endregion

	private:
		/**
		 * @throws ExternalStorageException		if IsExternal() and IsFull()
		 */
		void ThrowExternalIfFull() const;
		
		/**
		 * @throws ExternalStorageException		if IsExternal()
		 */
		void ThrowExternal() const;

		/**
		 * Helper for "do"ing generic things when compile-time type information is not known, but run-time enum information is available
		 *
		 * @param<Invokable>	tempalted lambda type
		 * @param type			Datum::Type to "do something" for
		 * @param func			tempalted lambda which will be passed a dummy nullptr of TypeOfType<type>
		 */
		template<typename Invokable>
		void Do(Type type, Invokable func);
	};

	/**
	 * @param val	a Datum::value_type
	 * @returns		the enumerated type of the value_type
	 */
	[[nodiscard]] Datum::Type TypeOf(Datum::value_type val) noexcept;

	/**
	 * @param t		a Datum::Type to stringify
	 * @returns		the string representation of the Datum::Type
	 */
	template<>
	const std::string& Enum<Datum::Type>::ToString(Datum::Type t);

	/**
	 * @param str							a string representing a Datum::Type
	 * @param returns Datum::Type::None		if str == "None" or it could not be parsed
	 * @param returns						the Datum::Type represented by str
	 */
	template<>
	Datum::Type Enum<Datum::Type>::FromString(const std::string& str);
}

#include "Datum.inl"
