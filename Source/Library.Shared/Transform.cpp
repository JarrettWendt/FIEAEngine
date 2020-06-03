#include "pch.h"
#include "Transform.h"

namespace Library
{
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
