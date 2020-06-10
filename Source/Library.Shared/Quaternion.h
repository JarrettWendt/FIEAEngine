#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "Macros.h"

namespace Library
{
	struct Quaternion : public Vector4
	{
		/** inherit all ctors */
		using Vector4::Vector4;

		/**
		 * Default ctor.
		 * Initializes xyz = 0 and w = 1.
		 */
		Quaternion() noexcept;
		
		/**
		 * implicit converting ctor
		 * 
		 * @param v		Vector4 to convert to a Quaternion
		 */
		Quaternion(const Vector4& v) noexcept;

		[[nodiscard]] static Quaternion Conjugate(const Quaternion& q) noexcept;
		[[nodiscard]] static Quaternion Inverse(const Quaternion& q);

#pragma region operators
#pragma region Arithmetic
		[[nodiscard]] Quaternion operator*(const Quaternion& other) const noexcept;
		Quaternion& operator*=(const Quaternion& other) noexcept;
		
		[[nodiscard]] Quaternion operator/(const Quaternion& other) const noexcept;
		Quaternion& operator/=(const Quaternion& other) noexcept;
#pragma endregion
#pragma endregion
	};
}
