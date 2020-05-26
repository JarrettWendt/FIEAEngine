// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include <cassert>

namespace Library
{
	class RTTI
	{
	public:
		using IDType = size_t;

		virtual ~RTTI() = default;

		static constexpr IDType TypeIDClass() noexcept { return 0; }

		virtual IDType TypeIDInstance() const noexcept = 0;

		virtual bool Is(const IDType id) const noexcept { return id == 0; }

		template<typename T>
		bool Is() const noexcept { return Is(T::TypeIDClass()); }

		template<typename T>
		const T* As() const noexcept { return const_cast<RTTI*>(this)->As<T>(); }

		template<typename T>
		T* As() noexcept { return Is<T>() ? reinterpret_cast<T*>(this) : nullptr; }

		template<typename T>
		constexpr T& AssertAs()
		{
			assert(Is<T>());
			return static_cast<T&>(*this);
		}

		template<typename T>
		constexpr const T& AssertAs() const { return const_cast<RTTI*>(this)->AssertAs<T>(); }

		virtual bool Equals(const RTTI&) { return false; }

	protected:
		inline static IDType nextID = 0;
	};

#define RTTI_DECLARATIONS(Base)																							\
	public:																												\
		static constexpr Library::RTTI::IDType TypeIDClass() noexcept { return typeID; }								\
		virtual Library::RTTI::IDType TypeIDInstance() const noexcept override { return TypeIDClass(); }				\
		virtual bool Is(Library::RTTI::IDType id) const noexcept override { return (id == typeID) || Base::Is(id); }	\
	private:																											\
		inline static const Library::RTTI::IDType typeID = nextID++;

#define RTTI_EQUALS(Derived)																							\
	public:																												\
		virtual bool Equals(const RTTI& other) noexcept override														\
		{																												\
			if (const Derived* derived = other.As<Derived>())															\
			{																											\
				return *derived == *this;																				\
			}																											\
			return false;																								\
		}
}
