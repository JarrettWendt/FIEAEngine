#include "pch.h"
#include "Vector2.h"
#include "Macros.h"

namespace Library
{
	float& Vector2::operator[](const size_type i) noexcept
	{
		assertm(i < 2, "Vector2 index out of bounds");
		return v[i];
	}

	float Vector2::operator[](const size_type i) const noexcept
	{
		return const_cast<Vector2*>(this)->operator[](i);
	}

	bool operator==(const Vector2& left, const Vector2& right)
	{
		return !std::memcmp(&left, &right, sizeof(Vector2));
	}

	bool operator!=(const Vector2& left, const Vector2& right)
	{
		return !operator==(left, right);
	}

	std::ostream& operator<<(std::ostream& stream, const Vector2& v)
	{
		return stream << "<" << v.x << ", " << v.y << ">";
	}
}
