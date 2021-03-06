#include "pch.h"
#include "Vector4.h"

namespace Library
{	
	const Vector4 Vector4::Zero = { 0, 0, 0, 0 };
	const Vector4 Vector4::One = { 1, 1, 1, 1 };
	
#pragma region Special Members
	Vector4::Vector4(const float x, const float y, const float z, const float w) noexcept :
		x(x), y(y), z(z), w(w) {}
	
	Vector4::Vector4(const Vector3& v, const float w) noexcept :
		Vector4(v.x, v.y, v.z, w) {}
	
	Vector4::Vector4(const float w, const Vector3& v) noexcept :
		Vector4(v, w) {}
#pragma endregion
	
	Vector4::operator Vector3() const noexcept
	{
		return { x, y, z };
	}

	float Vector4::Dot(const Vector4& a, const Vector4& b) noexcept
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

#pragma region operators
	float& Vector4::operator[](const size_type i) noexcept
	{
		assertm(i <= 4, "Vector4 index out of bounds");
		return *(reinterpret_cast<float*>(this) + i);
	}
	
	float Vector4::operator[](const size_type i) const noexcept
	{
		return const_cast<Vector4*>(this)->operator[](i);
	}

#pragma region Arithmetic
#pragma region scalar	
	Vector4 operator+(const Vector4& v, const float f) noexcept
	{
		return { v.x + f, v.y + f, v.z + f, v.w + f };
	}
	
	Vector4 operator+(const float f, const Vector4& v) noexcept
	{
		return v + f;
	}

	Vector4 operator-(const Vector4& v, const float f) noexcept
	{
		return { v.x - f, v.y - f, v.z - f, v.w - f };
	}

	Vector4 operator-(const float f, const Vector4& v) noexcept
	{
		return { f - v.x, f - v.y, f - v.z, f - v.w };
	}
	
	Vector4 operator*(const Vector4& v, const float f) noexcept
	{
		return { v.x * f, v.y * f, v.z * f, v.w * f };
	}

	Vector4 operator*(const float f, const Vector4& v) noexcept
	{
		return v * f;
	}

	Vector4 operator/(const Vector4& v, const float f) noexcept
	{
		return { v.x / f, v.y / f, v.z / f, v.w / f };
	}

	Vector4 operator/(const float f, const Vector4& v) noexcept
	{
		return { f / v.x, f / v.y, f / v.z, f / v.w };
	}

	Vector4& operator+=(Vector4& v, const float f)
	{
		return v = v + f;
	}

	Vector4& operator-=(Vector4& v, const float f)
	{
		return v = v - f;
	}

	Vector4& operator*=(Vector4& v, const float f)
	{
		return v = v * f;
	}

	Vector4& operator/=(Vector4& v, const float f)
	{
		return v = v / f;
	}
#pragma endregion
	
#pragma region vector
	Vector4 Vector4::operator-() const noexcept
	{
		return { -x, -y, -z, -w };
	}
	
	Vector4 Vector4::operator+(const Vector4& other) const noexcept
	{
		return { x + other.x, y + other.y, z + other.z, w + other.w };
	}

	Vector4 Vector4::operator-(const Vector4& other) const noexcept
	{
		return { x - other.x, y - other.y, z - other.z, w - other.w };
	}

	Vector4 Vector4::operator*(const Vector4& other) const noexcept
	{
		return { x * other.x, y * other.y, z * other.z, w * other.w };
	}

	Vector4 Vector4::operator/(const Vector4& other) const noexcept
	{
		return { x / other.x, y / other.y, z / other.z, w / other.w };
	}

	Vector4& Vector4::operator+=(const Vector4& other) noexcept
	{		
		return *this = *this + other;
	}

	Vector4& Vector4::operator-=(const Vector4& other) noexcept
	{		
		return *this = *this - other;
	}

	Vector4& Vector4::operator*=(const Vector4& other) noexcept
	{		
		return *this = *this * other;
	}

	Vector4& Vector4::operator/=(const Vector4& other) noexcept
	{		
		return *this = *this / other;
	}
#pragma endregion
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
#pragma endregion
}
