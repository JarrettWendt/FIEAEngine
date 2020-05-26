#pragma once
#include "Memory.h"
#include "Macros.h"

namespace Library::Memory
{
	void* Malloc(const size_t count) noexcept
	{
		if (count == 0)
		{
			return nullptr;
		}
		void* ret = std::malloc(count);
		assertm(ret, "std::malloc returned nullptr");
		return ret;
	}
	
	template<typename T>
	inline T* Malloc(const size_t count) noexcept
	{
		return reinterpret_cast<T*>(Malloc(count * sizeof(T)));
	}

	template<>
	inline void* Malloc(const size_t count) noexcept
	{
		return Malloc(count);
	}

	template<typename T>
	void Free(T*& array) noexcept
	{
		std::free(reinterpret_cast<void*>(array));
		array = nullptr;
	}

	template<typename T>
	void Realloc(T*& array, const size_t count) noexcept
	{
		Realloc<void>(reinterpret_cast<void*&>(array), count * sizeof(T));
	}

	template<>
	inline void Realloc(void*& array, const size_t byteCount) noexcept
	{
		if (byteCount == 0)
		{
			Free(array);
		}
		else
		{
#pragma warning(push)
#pragma warning(disable: 6308)
			array = std::realloc(reinterpret_cast<void*>(array), byteCount);
			assertm(array, "std::realloc returned nullptr");
#pragma warning(pop)
		}
	}

	template<typename T>
	void Memcpy(T* dest, const T* source, const size_t count) noexcept
	{
		Memcpy<void>(reinterpret_cast<void*>(dest), reinterpret_cast<const void*>(source), count * sizeof(T));
	}

	template<>
	inline void Memcpy(void* dest, const void* source, const size_t byteCount) noexcept
	{
		// std::memcpy already safely does nothing when passing a byteCount of 0.
		assertm((dest && source && byteCount != 0) || byteCount == 0, "undefined behavior in Memory::Memcpy");
		std::memcpy(dest, source, byteCount);
	}

	template<typename T>
	inline void Memmove(T* dest, const T* source, const size_t count) noexcept
	{
		Memmove<void>(reinterpret_cast<void*>(dest), reinterpret_cast<const void*>(source), count * sizeof(T));
	}

	template<>
	inline void Memmove(void* dest, const void* source, const size_t byteCount) noexcept
	{
		// std::memcpy already safely does nothing when passing a byteCount of 0.
		assertm((dest && source && byteCount != 0) || byteCount == 0, "undefined behavior in Memory::Memmove");
		std::memmove(dest, source, byteCount);
	}
	
	template<typename T>
	inline void Memset(T* dest, const uint8_t byte, const size_t count) noexcept
	{
		Memset<void>(reinterpret_cast<void*>(dest), byte, count * sizeof(T));
	}
	
	template<>
	inline void Memset(void* dest, const uint8_t byte, const size_t byteCount) noexcept
	{
		std::memset(dest, byte, byteCount);
	}
}
