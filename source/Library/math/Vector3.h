#pragma once

#include "Vector2.h"

namespace Library
{
	struct Vector3
	{
		using size_type = size_t;

		const static Vector3 Zero, One;
		
		float x{}, y{}, z{};

		[[nodiscard]] static Vector3 Cross(const Vector3& a, const Vector3& b) noexcept;
		[[nodiscard]] static float Dot(const Vector3& a, const Vector3& b) noexcept;

#pragma region operators
		template<size_t Index>
		[[nodiscard]] constexpr float& operator[](std::integral_constant<size_t, Index>) noexcept
		{
			return *(reinterpret_cast<float*>(this) + Index);
		}
		
		template<size_t Index>
		[[nodiscard]] constexpr float operator[](std::integral_constant<size_t, Index> i) const noexcept
		{
			return const_cast<Vector3*>(this)->operator[](i);
		}
		
		[[nodiscard]] float& operator[](size_type i) noexcept;
		[[nodiscard]] float operator[](size_type i) const noexcept;

#pragma region Arithmetic
		[[nodiscard]] Vector3 operator-() const noexcept;

#pragma region Scalar
		friend Vector3 operator+(const Vector3& v, float f) noexcept;
		friend Vector3 operator+(float f, const Vector3& v) noexcept;
		
		friend Vector3 operator-(const Vector3& v, float f) noexcept;
		friend Vector3 operator-(float f, const Vector3& v) noexcept;
		
		friend Vector3 operator*(const Vector3& v, float f) noexcept;
		friend Vector3 operator*(float f, const Vector3& v) noexcept;
		
		friend Vector3 operator/(const Vector3& v, float f) noexcept;
		friend Vector3 operator/(float f, const Vector3& v) noexcept;
		
		Vector3& operator+=(float f) noexcept;
		Vector3& operator-=(float f) noexcept;
		Vector3& operator*=(float f) noexcept;
		Vector3& operator/=(float f) noexcept;
#pragma endregion
		
		[[nodiscard]] Vector3 operator+(const Vector3& other) const noexcept;
		[[nodiscard]] Vector3 operator-(const Vector3& other) const noexcept;
		
		/**
		 * Neither cross product nor dot product.
		 * Multiplies vectors component-wise.
		 *
		 * @param other		vector to multiply this one against
		 * @returns			resultant vector
		 */
		[[nodiscard]] Vector3 operator*(const Vector3& other) const noexcept;
		[[nodiscard]] Vector3 operator/(const Vector3& other) const noexcept;

		Vector3& operator+=(const Vector3& other) noexcept;
		Vector3& operator-=(const Vector3& other) noexcept;
		Vector3& operator*=(const Vector3& other) noexcept;
		Vector3& operator/=(const Vector3& other) noexcept;
#pragma endregion

		friend bool operator==(const Vector3& left, const Vector3& right) noexcept;
		friend bool operator!=(const Vector3& left, const Vector3& right) noexcept;

		friend std::ostream& operator<<(std::ostream& stream, const Vector3& v);
#pragma endregion
	};
}
