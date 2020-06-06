#pragma once

namespace Library
{
	struct Vector3
	{
		using size_type = size_t;

#pragma warning(push)
#pragma warning(disable: 4201) // nameless struct/union
		union
		{
			struct
			{
				float x, y, z;
			};
			float v[3]{};
		};
#pragma warning(pop)

		[[nodiscard]] float& operator[](size_type i) noexcept;
		[[nodiscard]] float operator[](size_type i) const noexcept;

#pragma region Arithmetic
		Vector3 operator+(const Vector3& other) const noexcept;
		Vector3 operator-(const Vector3& other) const noexcept;
		Vector3 operator*(const Vector3& other) const noexcept;
		Vector3 operator/(const Vector3& other) const noexcept;

		Vector3& operator+=(const Vector3& other) noexcept;
		Vector3& operator-=(const Vector3& other) noexcept;
		Vector3& operator*=(const Vector3& other) noexcept;
		Vector3& operator/=(const Vector3& other) noexcept;
#pragma endregion

		[[nodiscard]] friend bool operator==(const Vector3& left, const Vector3& right) noexcept;
		[[nodiscard]] friend bool operator!=(const Vector3& left, const Vector3& right) noexcept;

		friend std::ostream& operator<<(std::ostream& stream, const Vector3& v);
	};
}
