#pragma once

#include "Vector3.h"

namespace Library
{
	struct Vector4
	{
		using size_type = size_t;

		float x{}, y{}, z{}, w{};

		SPECIAL_MEMBERS(Vector4, default)

		Vector4(float x, float y, float z, float w) noexcept;
		
		Vector4(const Vector3& v, float w) noexcept;
		Vector4(float w, const Vector3& v) noexcept;

		/**
		 * implicit conversion operator
		 *
		 * @returns		this Vector4's xyz components
		 */
		 operator Vector3() const noexcept;
		
		[[nodiscard]] static float Dot(const Vector4& a, const Vector4& b) noexcept;

#pragma region operators
		[[nodiscard]] float& operator[](size_type i) noexcept;
		[[nodiscard]] float operator[](size_type i) const noexcept;

#pragma region Arithmetic
		[[nodiscard]] friend Vector4 operator/(const Vector4& v, float f) noexcept;
		[[nodiscard]] friend Vector4 operator/(float f, const Vector4& v) noexcept;
		
		[[nodiscard]] Vector4 operator+(const Vector4& other) const noexcept;
		[[nodiscard]] Vector4 operator-(const Vector4& other) const noexcept;

		/**
		 * Neither cross product nor dot product.
		 * Multiplies vectors component-wise.
		 * 
		 * @param other		vector to multiply this one against
		 * @returns			resultant vector
		 */
		[[nodiscard]] Vector4 operator*(const Vector4& other) const noexcept;
		[[nodiscard]] Vector4 operator/(const Vector4& other) const noexcept;

		Vector4& operator+=(const Vector4& other) noexcept;
		Vector4& operator-=(const Vector4& other) noexcept;
		Vector4& operator*=(const Vector4& other) noexcept;
		Vector4& operator/=(const Vector4& other) noexcept;
#pragma endregion
		
		[[nodiscard]] friend bool operator==(const Vector4& left, const Vector4& right);
		[[nodiscard]] friend bool operator!=(const Vector4& left, const Vector4& right);

		friend std::ostream& operator<<(std::ostream& stream, const Vector4& v);
#pragma endregion
	};
}
