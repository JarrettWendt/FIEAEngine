// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once
#include "Datum.h"

namespace Library
{
#pragma region Special Members
	template<typename T>
	inline Datum::Datum(std::tuple<T*, size_t, size_t> data) noexcept :
		Datum(std::get<0>(data), std::get<1>(data), std::get<2>(data)) {}
	
	template<typename T>
	inline Datum::Datum(T* array, const size_t size, const size_t capacity) noexcept :
		isExternal(true)
	{
		assert(size <= capacity);
		// need to SetType before we can SetData
		// call SetType without worrying about IsExternal
		Base::SetType<T>();
		GetArray<T>().SetData(array, size, capacity);
	}
	
	template<typename T>
	inline Datum::Datum(T* array, const size_t count) noexcept :
		Datum(array, count, count) {}
	
	template<typename T>
	inline Datum& Datum::operator=(const T& t)
	{
		Base::operator=(t);
		return *this;
	}
#pragma endregion
	
	template<typename T>
	inline Datum Datum::Construct(const size_t capacity)
	{
		Datum ret;
		ret.Reserve<T>(capacity);
		return ret;
	}
	
	template<typename T>
	inline void Datum::SetType()
	{
		if (!IsType<T>())
		{
			ThrowExternal();
		}
		Base::SetType<T>();
	}

	inline constexpr Datum::Type Datum::GetType() const noexcept
	{
		return Type(TypeID());
	}

#pragma region Insert
	template<typename ...Args>
	inline auto Datum::Insert(Args&& ...args)
	{
		ThrowExternalIfFull();
		return Base::Insert(std::forward<Args>(args)...);
	}
	
	template<typename T>
	inline void Datum::PushBack(const T& t)
	{
		ThrowExternalIfFull();
		Base::PushBack(t);
	}
	
	template<typename T>
	inline void Datum::PushBack(T&& t)
	{
		ThrowExternalIfFull();
		Base::PushBack(std::move(t));
	}
	
	template<std::forward_iterator It>
	inline void Datum::PushBack(const It first, const It last)
	{
		ThrowExternalIfFull();
		Base::PushBack(first, last);
	}
	
	template<typename T>
	inline void Datum::PushFront(const T& t)
	{
		ThrowExternalIfFull();
		Base::PushFront(t);
	}

	template<typename T>
	inline void Datum::PushFront(T&& t)
	{
		ThrowExternalIfFull();
		Base::PushFront(std::move(t));
	}

	template<std::forward_iterator It>
	inline void Datum::PushFront(const It first, const It last)
	{
		ThrowExternalIfFull();
		Base::PushFront(first, last);
	}

	template<typename T, typename... Args>
	inline T& Datum::EmplaceBack(Args&&... args)
	{
		ThrowExternalIfFull();
		return Base::EmplaceBack<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	inline T& Datum::EmplaceFront(Args&&... args)
	{
		ThrowExternalIfFull();
		return Base::EmplaceFront<T>(std::forward<Args>(args)...);
	}
#pragma endregion

#pragma region Memory
	template<typename T>
	inline void Datum::SetStorage(std::tuple<T*, size_t, size_t> data) noexcept
	{
		SetStorage(std::get<0>(data), std::get<1>(data), std::get<2>(data));
	}
	
	template<typename T>
	inline void Datum::SetStorage(T* array, const size_t count, const size_t capacity) noexcept
	{
		// If we're already external, relinquish what data we have because Array::SetData would free it.
		if (IsExternal())
		{
#pragma warning(push)
#pragma warning(disable: 4834)	// discarding [[nodiscard]] return value
			GetArray<T>().TakeData();
#pragma warning(pop)
		}
		GetArray<T>().SetData(array, count, capacity);
		isExternal = true;
	}
	
	template<typename T>
	inline void Datum::SetStorage(T* array, const size_t count) noexcept
	{
		SetStorage(array, count, count);
	}
	
	template<typename T>
	inline void Datum::Reserve(const size_t capacity)
	{
		ThrowExternal();
		Base::Reserve<T>(capacity);
	}
	
	template<typename T>
	inline void Datum::Resize(const size_t capacity, const T& prototype)
	{
		ThrowExternal();
		Base::Resize(capacity, prototype);
	}
#pragma endregion

	template<typename Invokable>
	inline void Datum::Do(const Type type, Invokable func)
	{
		switch (type)
		{
			// primitives
		case Type::Bool:		func.template operator()<bool>();					break;
		case Type::Int:			func.template operator()<int>();					break;
		case Type::Float:		func.template operator()<float>();					break;

			// math types
		case Type::Vector2:		func.template operator()<Vector2>();				break;
		case Type::Vector3:		func.template operator()<Vector3>();				break;
		case Type::Vector4:		func.template operator()<Vector4>();				break;
		case Type::Quaternion:	func.template operator()<Quaternion>();				break;
		case Type::Matrix:		func.template operator()<Matrix>();					break;
		case Type::Transform:	func.template operator()<Transform>();				break;

			// object types
		case Type::String:		func.template operator()<std::string>();			break;
		case Type::RTTI:		func.template operator()<std::shared_ptr<RTTI>>();	break;
			
		case Type::None: [[fallthrough]];
		default: assertm("unexpected type");
		}
	}
}
