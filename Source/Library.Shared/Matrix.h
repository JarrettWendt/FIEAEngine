#pragma once

#include "Vector4.h"

namespace Library
{
	struct Matrix
	{
		using size_type = size_t;

#pragma warning(push)
#pragma warning(disable: 4201) // nameless struct/union
	private:
		union
		{
			float m[4][4];
			Vector4 v[4];
		};
#pragma warning(pop)
		
	public:
		[[nodiscard]] Vector4& operator[](size_type i) noexcept;
		[[nodiscard]] const Vector4& operator[](size_type i) const noexcept;
		
		[[nodiscard]] friend bool operator==(const Matrix& left, const Matrix& right);
		[[nodiscard]] friend bool operator!=(const Matrix& left, const Matrix& right);

		friend std::ostream& operator<<(std::ostream& stream, const Matrix& m);
	};
}
