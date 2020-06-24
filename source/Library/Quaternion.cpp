#include "pch.h"
#include "Quaternion.h"

namespace Library
{
	Quaternion::Quaternion() noexcept :
		Vector4(0, 0, 0, 1) {}
	
	Quaternion::Quaternion(const Vector4& v) noexcept :
		Vector4(v) {}
	
	Quaternion Quaternion::Conjugate(const Quaternion& q) noexcept
	{
		return { -Vector3(q), q.w };
	}

	Quaternion Quaternion::Inverse(const Quaternion& q)
	{
		return Conjugate(q) / Dot(q, q);
	}

#pragma region operators
	Quaternion Quaternion::operator*(const Quaternion& other) const noexcept
	{
		const Vector3 v = *this;
		const Vector3 ov = other;
		return { w * ov + other.w * v + Vector3::Cross(v, ov), w * other.w - Vector3::Dot(v, ov) };
	}
	
	Quaternion& Quaternion::operator*=(const Quaternion& other) noexcept
	{
		*this = *this * other;
		return *this;
	}
	
	Quaternion Quaternion::operator/(const Quaternion& other) const noexcept
	{
		return *this * Inverse(other);
	}

	Quaternion& Quaternion::operator/=(const Quaternion& other) noexcept
	{
		*this = *this / other;
		return *this;
	}
	
	std::ostream& operator<<(std::ostream& stream, const Quaternion& q)
	{
		return stream << "{" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << "}";
	}
#pragma endregion
}
