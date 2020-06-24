// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

namespace Library::Memory
{
	/**
	 * Wrapper to std::malloc which asserts if std::malloc fails.
	 * 
	 * @param count		How many bytes of memoery to allocate.
	 * @returns			A pointer to newly allocated memory, or nullptr if count was 0.
	 *
	 * @asserts			That std::malloc succeeds.
	 */
	static void* Malloc(size_t count = 1) noexcept;

	/**
	 * Templated wrapper to std::malloc which asserts if std::malloc fails.
	 * Parameter is an element count rather than byte count.
	 *
	 * @param count		How many elements of T's worth of memoery to allocate.
	 * @returns			A pointer to newly allocated memory, or nullptr if count was 0.
	 *
	 * @asserts			That std::malloc succeeds.
	 */
	template<typename T>
	static T* Malloc(size_t count = 1) noexcept;

	/**
	 * void specialization of Malloc which expects a number of bytes rather than number of elements.
	 *
	 * @param count		How many bytes of memoery to allocate.
	 * @returns			A pointer to newly allocated memory, or nullptr if count was 0.
	 *
	 * @asserts			That std::malloc succeeds.
	 */
	template<>
	void* Malloc<void>(size_t count) noexcept;

	/** 
	 * Templated wrapper to std::free to avoid manually calling reinterpret_cast.
	 * Accepts a pointer by reference to set it to nullptr for safety.
	 * 
	 * @param array		Pointer to deallocate.
	 */
	template<typename T>
	static void Free(T*& array) noexcept;

	/**
	 * Templated wrapper to std::realloc which takes a number of elements rather than a byte count.
	 * Asserts if std::realloc fails.
	 * 
	 * @param <T>		The type of data in the array.
	 * @param array		The array which will be set to a new address.
	 * @param count		How many elements of T the array should be able to contain.
	 *
	 * @asserts			That std::realloc succeeds.
	 */
	template<typename T>
	static void Realloc(T*& array, size_t count = 1) noexcept;

	/** 
	 * void specialization of Realloc where the last parameter is assumed to be the size in bytes.
	 * Asserts if std::realloc fails.
	 * 
	 * @param	pointer to an array.
	 * @param	number of bytes.
	 *
	 * @asserts			That std::realloc succeeds.
	 */
	template<>
	void Realloc<void>(void*& array, size_t byteCount) noexcept;

	/** 
	 * Templated wrapper to std::memcpy where the last parameter is an element count rather than a byte count.
	 * If the objects overlap, the behavior is undefined.
	 * If either dest or source is an invalid or null pointer, the behavior is undefined, even if count is zero.
	 * Does nothing if count is zero.
	 * If the objects are potentially-overlapping or not TriviallyCopyable, the behavior of memcpy is not specified and may be undefined.
	 * asserts if either dest or source are nullptr.
	 * 
	 * @param dest			Destination memory address.
	 * @param source		Source memory address.
	 * @param count			Number of elements to copy.
	 *
	 * @asserts				That std::memcpy succeeds.
	 */
	template<typename T>
	static void Memcpy(T* dest, const T* source, size_t count = 1) noexcept;

	/** 
	 * void specialization of Memcpy where the last parameter is assumed to be the size in bytes.
	 * 
	 * @param dest			Destination memory address.
	 * @param source		Source memory address.
	 * @param byteCount		Number of bytes to copy.
	 *
	 * @asserts				That std::memcpy succeeds.
	 */
	template<>
	void Memcpy<void>(void* dest, const void* source, size_t byteCount) noexcept;

	/** 
	 * Templated wrapper to std::memmove where the last parameter is an element count rather than a byte count.
	 * The objects may overlap: copying takes place as if the memory were copied to a temp array and then copied from the temp to dest.
	 * If either dest or source is an invalid or null pointer, the behavior is undefined, even if count is zero.
	 * Does nothing if count is zero.
	 * If the objects are potentially-overlapping or not TriviallyCopyable, the behavior of memmove is not specified and may be undefined.
	 * asserts if either dest or source are nullptr.
	 * 
	 * @param dest			Destination memory address.
	 * @param source		Source memory address.
	 * @param count			Number of elements to copy.
	 *
	 * @asserts				That std::memmove succeeds.
	 */
	template<typename T>
	static void Memmove(T* dest, const T* source, size_t count = 1) noexcept;

	/**
	 * void specialization of Memmove where the last parameter is the number of bytes.
	 * 
	 * @param dest			Destination memory address.
	 * @param source		Source memory address.
	 * @param byteCount		Number of bytes to copy.
	 *
	 * @asserts				That std::memmove succeeds.
	 */
	template<>
	void Memmove<void>(void* dest, const void* source, size_t byteCount) noexcept;

	/**
	 * Templated wrapper to std::memset where the last parameter is an element count rather than a byte count.
	 *
	 * @param dest			destination memory address
	 * @param byte			byte to fill the destination with
	 * @param count			number of elements to fill
	 */
	template<typename T>
	static void Memset(T* dest, uint8_t byte, size_t count = 1) noexcept;

	/**
	 * void specialization of Memset where the last parameter is the number of bytes
	 *
	 * @param dest			destination memory address
	 * @param byte			byte to fill the destination with
	 * @param byteCount		number of bytes to fill
	 */
	template<>
	void Memset<void>(void* dest, uint8_t byte, size_t byteCount) noexcept;
}

#include "Memory.inl"
