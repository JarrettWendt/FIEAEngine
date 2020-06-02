// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include <cassert>
#include <concepts>

namespace Library
{
	class RTTI
	{
	public:
		using IDType = size_t;
		
		virtual ~RTTI() = default;

		virtual IDType TypeID() const noexcept = 0;

		// TODO: I would have preffered for this to also be named Is()
		// but them MSVC gives me "illegal use of this type as an expression" for the templated Is()
		virtual bool HasID(const IDType id) const noexcept { return id == RTTI::typeID; }

		template<typename T>
		bool Is() const noexcept { return HasID(T::typeID); }

		template<typename T>
		const T* As() const noexcept { return const_cast<RTTI*>(this)->As<T>(); }

		template<typename T>
		T* As() noexcept { return Is<T>() ? reinterpret_cast<T*>(this) : nullptr; }

		/**
		 * Faster than As()
		 *
		 * @asserts Is<T>()
		 */
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
	public:
		inline static const IDType typeID = nextID++;
	};

#define RTTI_DECLARATIONS(BaseType)																						\
	public:																												\
		using Base = BaseType;																							\
		inline static const Library::RTTI::IDType typeID = nextID++;													\
		virtual Library::RTTI::IDType TypeID() const noexcept override { return typeID; }								\
		virtual bool HasID(Library::RTTI::IDType id) const noexcept override { return (id == typeID) || Base::HasID(id); }	\
		virtual bool Equals(const RTTI& other) noexcept override														\
		{																												\
			using DerivedType = std::remove_pointer_t<decltype(this)>;													\
			if constexpr (Library::Util::HasOperatorEqual<DerivedType>)													\
			{																											\
				if (auto derived = other.As<DerivedType>())																\
				{																										\
					return *derived == *this;																			\
				}																										\
			}																											\
			return false;																								\
		}																												\
	private:
}
