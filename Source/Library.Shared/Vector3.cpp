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
	
	bool operator==(const Vector3& left, const Vector3& right)
	{
		return !std::memcmp(&left, &right, sizeof(Vector3));
	}

	bool operator!=(const Vector3& left, const Vector3& right)
	{
		return !operator==(left, right);
	}

	std::ostream& operator<<(std::ostream& stream, const Vector3& v)
	{
		return stream << "<" << v.x << ", " << v.y << ", " << v.z << ">";
	}
}
