#pragma once

#include "Vector3.h"
#include "Quaternion.h"

namespace Library
{
	class Transform
	{
	protected:
		Vector3 translation;
		Quaternion rotation;
		Vector3 scale;

	public:
		[[nodiscard]] friend bool operator==(const Transform& left, const Transform& right);
		[[nodiscard]] friend bool operator!=(const Transform& left, const Transform& right);
		
		friend std::ostream& operator<<(std::ostream& stream, const Transform&);
	};
}
