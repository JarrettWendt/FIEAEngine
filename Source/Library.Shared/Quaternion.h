#pragma once

#include "Vector4.h"
#include "Macros.h"

namespace Library
{
	struct Quaternion : public Vector4
	{
		/** inherit all ctors */
		using Vector4::Vector4;
		
		SPECIAL_MEMBERS(Quaternion, default)

		/**
		 * Implicit conversion ctor.
		 * 
		 * @param v		Vector4 to construct this quat from
		 */
		Quaternion(const Vector4& v) noexcept;
	};
}
