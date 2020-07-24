#include "Memory.h"

#include <new>

// ReSharper disable CppParameterNamesMismatch

// https://en.cppreference.com/w/cpp/memory/new/operator_new
#pragma region new
#pragma region allocation
[[nodiscard]] void* operator new(const std::size_t count)
{
	return Library::Memory::Malloc(count);
}

[[nodiscard]] void* operator new[](const std::size_t count)
{
	return Library::Memory::Malloc(count);
}

[[nodiscard]] void* operator new(const std::size_t count, [[maybe_unused]] const std::align_val_t al)
{
	return Library::Memory::Malloc(count);
}

[[nodiscard]] void* operator new[](const std::size_t count, [[maybe_unused]] const std::align_val_t al)
{
	return Library::Memory::Malloc(count);
}
#pragma endregion

#pragma region non-throwing allocation
[[nodiscard]] void* operator new(const std::size_t count, [[maybe_unused]] const std::nothrow_t& tag) noexcept
{
	return std::malloc(count);
}

[[nodiscard]] void* operator new[](const std::size_t count, [[maybe_unused]] const std::nothrow_t& tag) noexcept
{
	return std::malloc(count);
}

[[nodiscard]] void* operator new(const std::size_t count, [[maybe_unused]] const std::align_val_t al, [[maybe_unused]] const std::nothrow_t& tag) noexcept
{
	return std::malloc(count);
}

[[nodiscard]] void* operator new[](const std::size_t count, [[maybe_unused]] const std::align_val_t al, [[maybe_unused]] const std::nothrow_t& tag) noexcept
{
	return std::malloc(count);
}
#pragma endregion
#pragma endregion

// https://en.cppreference.com/w/cpp/memory/new/operator_delete
#pragma region delete
#pragma region usual deallocation
void operator delete(void* ptr) noexcept
{
	Library::Memory::Free(ptr);
}

void operator delete[](void* ptr) noexcept
{
	Library::Memory::Free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] const std::align_val_t al) noexcept
{
	Library::Memory::Free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] const std::align_val_t al) noexcept
{
	Library::Memory::Free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] const std::size_t sz) noexcept
{
	Library::Memory::Free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] const std::size_t sz) noexcept
{
	Library::Memory::Free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] const std::size_t sz, [[maybe_unused]] const std::align_val_t al) noexcept
{
	Library::Memory::Free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] const std::size_t sz, [[maybe_unused]] const std::align_val_t al) noexcept
{
	Library::Memory::Free(ptr);
}
#pragma endregion

#pragma region placement deallocation
void operator delete(void* ptr, [[maybe_unused]] const std::nothrow_t& tag) noexcept
{
	Library::Memory::Free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] const std::nothrow_t& tag) noexcept
{
	Library::Memory::Free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] std::align_val_t al, [[maybe_unused]] const std::nothrow_t& tag ) noexcept
{
	Library::Memory::Free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] std::align_val_t al, [[maybe_unused]] const std::nothrow_t& tag ) noexcept
{
	Library::Memory::Free(ptr);
}
#pragma endregion
#pragma endregion
