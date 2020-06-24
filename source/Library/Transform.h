#pragma once

#include "Vector3.h"
#include "Quaternion.h"
#include "Macros.h"

namespace Library
{
	/**
	 * 
	 */
	struct Transform
	{
		Vector3 translation{};
		Quaternion rotation{};
		Vector3 scale{ Vector3::One };

		SPECIAL_MEMBERS(Transform, default)

		Transform(Vector3 translation, Quaternion rotation, Vector3 scale) noexcept;

#pragma region operators
#pragma region Arithmetic
		/**
		 * accumulates another Transform into this one
		 * 
		 * @param other		the Transform to accumulate
		 */
		[[nodiscard]] Transform operator+(const Transform& other) const noexcept;
		
		/**
		 * if you think of operator+ as "appending" Transforms, this "truncates"
		 *
		 * @param other		the Transform to accumulate
		 */
		[[nodiscard]] Transform operator-(const Transform& other) const noexcept;

		Transform& operator+=(const Transform& other) noexcept;
		Transform& operator-=(const Transform& other) noexcept;
#pragma endregion
		
		friend bool operator==(const Transform& left, const Transform& right);
		friend bool operator!=(const Transform& left, const Transform& right);
		
		friend std::ostream& operator<<(std::ostream& stream, const Transform&);
#pragma endregion
	};
}
