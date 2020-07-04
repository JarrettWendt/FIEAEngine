#include "pch.h"
#include "Transform.h"

namespace Library
{	
	Transform::Transform(Vector3 translation, Quaternion rotation, Vector3 scale) noexcept :
		translation(std::move(translation)),
		rotation(std::move(rotation)),
		scale(std::move(scale)) {}

#pragma region Arithmetic
	Transform Transform::operator+(const Transform& other) const noexcept
	{
		return Transform
		{
			translation + other.translation,
			rotation * other.rotation,
			scale * other.scale
		};
	}

	Transform Transform::operator-(const Transform& other) const noexcept
	{
		return Transform
		{
			translation - other.translation,
			rotation / other.rotation,
			scale / other.scale
		};
	}

	Transform& Transform::operator+=(const Transform& other) noexcept
	{
		return *this = *this + other;
	}

	Transform& Transform::operator-=(const Transform& other) noexcept
	{
		return *this = *this - other;
	}
#pragma endregion
	
	bool operator==(const Transform& left, const Transform& right)
	{
		return !std::memcmp(&left, &right, sizeof(Transform));
	}
	
	bool operator!=(const Transform& left, const Transform& right)
	{
		return !operator==(left, right);
	}
	
	std::ostream& operator<<(std::ostream& stream, const Transform& t)
	{
		return stream << "t: " << t.translation << ", r: " << t.rotation << ", s: " << t.scale;
	}
}
