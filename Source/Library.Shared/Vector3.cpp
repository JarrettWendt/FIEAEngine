#include "pch.h"
#include "Vector3.h"
#include "Macros.h"

namespace Library
{
	float& Vector3::operator[](const size_type i) noexcept
	{
		assertm(i < 4, "Vector3 index out of bounds");
		return v[i];
	}

	float Vector3::operator[](const size_type i) const noexcept
	{
		return const_cast<Vector3*>(this)->operator[](i);
	}

#pragma region Arithmetic
	Vector3 Vector3::operator+(const Vector3& other) const noexcept
	{
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}

	Vector3 Vector3::operator-(const Vector3& other) const noexcept
	{
		return Vector3{ x - other.x, y - other.y, z - other.z };
	}
	
	Vector3 Vector3::operator*(const Vector3& other) const noexcept
	{
		return Vector3{ x * other.x, y * other.y, z * other.z };
	}

	Vector3 Vector3::operator/(const Vector3& other) const noexcept
	{
		return Vector3{ x / other.x, y / other.y, z / other.z };
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
}
