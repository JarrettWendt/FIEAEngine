#include "pch.h"
#include "Vector4.h"
#include "Macros.h"

namespace Library
{
	float& Vector4::operator[](const size_type i) noexcept
	{
		assertm(i < 4, "Vector4 index out of bounds");
		return v[i];
	}
	
	float Vector4::operator[](const size_type i) const noexcept
	{
		return const_cast<Vector4*>(this)->operator[](i);
	}

#pragma region Arithmetic
	Vector4 Vector4::operator+(const Vector4& other) const noexcept
	{
		return Vector4{ x + other.x, y + other.y, z + other.z, w + other.w };
	}

	Vector4 Vector4::operator-(const Vector4& other) const noexcept
	{
		return Vector4{ x - other.x, y - other.y, z - other.z, w - other.w };
	}

	Vector4 Vector4::operator*(const Vector4& other) const noexcept
	{
		return Vector4{ x * other.x, y * other.y, z * other.z, w * other.w };
	}

	Vector4 Vector4::operator/(const Vector4& other) const noexcept
	{
		return Vector4{ x / other.x, y / other.y, z / other.z, w / other.w };
	}

	Vector4& Vector4::operator+=(const Vector4& other) noexcept
	{
		*this = *this + other;
		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& other) noexcept
	{
		*this = *this - other;
		return *this;
	}

	Vector4& Vector4::operator*=(const Vector4& other) noexcept
	{
		*this = *this * other;
		return *this;
	}

	Vector4& Vector4::operator/=(const Vector4& other) noexcept
	{
		*this = *this / other;
		return *this;
	}
#pragma endregion
	
	bool operator==(const Vector4& left, const Vector4& right)
	{
		return !std::memcmp(&left, &right, sizeof(Vector4));
	}

	bool operator!=(const Vector4& left, const Vector4& right)
	{
		return !operator==(left, right);
	}

	std::ostream& operator<<(std::ostream& stream, const Vector4& v)
	{
		return stream << "<" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ">";
	}
}
