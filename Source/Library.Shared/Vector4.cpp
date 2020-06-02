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
