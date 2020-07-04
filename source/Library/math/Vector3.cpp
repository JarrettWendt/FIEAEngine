#include "pch.h"
#include "Vector3.h"
#include "Macros.h"

namespace Library
{
	const Vector3 Vector3::Zero = { 0, 0, 0 };
	const Vector3 Vector3::One = { 1, 1, 1 };
	
	Vector3 Vector3::Cross(const Vector3& a, const Vector3& b) noexcept
	{
		return
		{
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}

	float Vector3::Dot(const Vector3& a, const Vector3& b) noexcept
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

#pragma region operators
	float& Vector3::operator[](const size_type i) noexcept
	{
		assertm(i <= 3, "Vector3 index out of bounds");
		return *(reinterpret_cast<float*>(this) + i);
	}

	float Vector3::operator[](const size_type i) const noexcept
	{
		return const_cast<Vector3*>(this)->operator[](i);
	}

#pragma region Arithmetic
	Vector3 Vector3::operator-() const noexcept
	{
		return { -x, -y, -z };
	}
	
#pragma region Scalar	
	Vector3 operator+(const Vector3& v, const float f) noexcept
	{
		return { v.x + f, v.y + f, v.z + f };
	}

	Vector3 operator+(const float f, const Vector3& v) noexcept
	{
		return v + f;
	}
	
	Vector3 operator-(const Vector3& v, const float f) noexcept
	{
		return { v.x - f, v.y - f, v.z - f };
	}

	Vector3 operator-(const float f, const Vector3& v) noexcept
	{
		return { f - v.x, f - v.y, f - v.z };
	}
	
	Vector3 operator*(const Vector3& v, const float f) noexcept
	{
		return { v.x * f, v.y * f, v.z * f };
	}

	Vector3 operator*(const float f, const Vector3& v) noexcept
	{
		return v * f;
	}
	
	Vector3 operator/(const Vector3& v, const float f) noexcept
	{
		return { v.x / f, v.y / f, v.z / f };
	}

	Vector3 operator/(const float f, const Vector3& v) noexcept
	{
		return { f / v.x, f / v.y, f / v.z };
	}

	Vector3& Vector3::operator+=(const float f) noexcept
	{
		return *this = *this + f;
	}

	Vector3& Vector3::operator-=(const float f) noexcept
	{
		return *this = *this - f;
	}
	
	Vector3& Vector3::operator*=(const float f) noexcept
	{
		return *this = *this * f;
	}

	Vector3& Vector3::operator/=(const float f) noexcept
	{
		return *this = *this / f;
	}
#pragma endregion
	
	Vector3 Vector3::operator+(const Vector3& other) const noexcept
	{
		return { x + other.x, y + other.y, z + other.z };
	}

	Vector3 Vector3::operator-(const Vector3& other) const noexcept
	{
		return { x - other.x, y - other.y, z - other.z };
	}
	
	Vector3 Vector3::operator*(const Vector3& other) const noexcept
	{
		return { x * other.x, y * other.y, z * other.z };
	}

	Vector3 Vector3::operator/(const Vector3& other) const noexcept
	{
		return { x / other.x, y / other.y, z / other.z };
	}

	Vector3& Vector3::operator+=(const Vector3& other) noexcept
	{
		*this = *this + other;
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& other) noexcept
	{
		*this = *this - other;
		return *this;
	}

	Vector3& Vector3::operator*=(const Vector3& other) noexcept
	{
		*this = *this * other;
		return *this;
	}
	
	Vector3& Vector3::operator/=(const Vector3& other) noexcept
	{
		*this = *this / other;
		return *this;
	}
#pragma endregion

	bool operator==(const Vector3& left, const Vector3& right) noexcept
	{
		return !std::memcmp(&left, &right, sizeof(Vector3));
	}

	bool operator!=(const Vector3& left, const Vector3& right) noexcept
	{
		return !operator==(left, right);
	}

	std::ostream& operator<<(std::ostream& stream, const Vector3& v)
	{
		return stream << "<" << v.x << ", " << v.y << ", " << v.z << ">";
	}
#pragma endregion
}
