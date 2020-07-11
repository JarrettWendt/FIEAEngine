#pragma once

#include "Macros.h"

namespace Library
{
	struct Vector2
	{
		using size_type = size_t;
		
		float x, y;

		/**
		 * Get component at compile-time.
		 * Use with Literals::operator""_zc
		 */
		template<size_t index>
		float& operator[](std::integral_constant<size_t, index>) noexcept
		{
			static_assert(index < 2, "invalid index");
			if constexpr (index == 0)
			{
				return x;
			}
			else
			{
				return y;
			}
		}

		/**
		 * Get component at compile-time.
		 * Use with Literals::operator""_zc
		 */
		template<size_t index>
		float operator[](std::integral_constant<size_t, index> i) const noexcept
		{
			return const_cast<Vector2*>(this)->operator[](i);
		}
		
		[[nodiscard]] float& operator[](size_type i) noexcept;
		[[nodiscard]] float operator[](size_type i) const noexcept;

		friend bool operator==(const Vector2& left, const Vector2& right);
		friend bool operator!=(const Vector2& left, const Vector2& right);

		friend std::ostream& operator<<(std::ostream& stream, const Vector2& v);
	};
}
