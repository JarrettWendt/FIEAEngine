#pragma once

namespace Library
{
	struct Vector4
	{
		using size_type = size_t;

#pragma warning(push)
#pragma warning(disable: 4201) // nameless struct/union
		union
		{
			struct
			{
				float x, y, z, w;
			};
			float v[4]{};
		};
#pragma warning(pop)
		
		[[nodiscard]] float& operator[](size_type i) noexcept;
		[[nodiscard]] float operator[](size_type i) const noexcept;

#pragma region Arithmetic
		Vector4 operator+(const Vector4& other) const noexcept;
		Vector4 operator-(const Vector4& other) const noexcept;
		Vector4 operator*(const Vector4& other) const noexcept;
		Vector4 operator/(const Vector4& other) const noexcept;

		Vector4& operator+=(const Vector4& other) noexcept;
		Vector4& operator-=(const Vector4& other) noexcept;
		Vector4& operator*=(const Vector4& other) noexcept;
		Vector4& operator/=(const Vector4& other) noexcept;
#pragma endregion
		
		[[nodiscard]] friend bool operator==(const Vector4& left, const Vector4& right);
		[[nodiscard]] friend bool operator!=(const Vector4& left, const Vector4& right);

		friend std::ostream& operator<<(std::ostream& stream, const Vector4& v);
	};
}
