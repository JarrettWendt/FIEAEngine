#pragma once

#include "Vector4.h"

namespace Library
{
	struct Matrix
	{
		using size_type = size_t;
		const static Matrix Identity;
	private:
		Vector4 v[4];
		
	public:
		Matrix(Vector4, Vector4, Vector4, Vector4) noexcept;
		SPECIAL_MEMBERS(Matrix, default)
		
		[[nodiscard]] Vector4& operator[](size_type i) noexcept;
		[[nodiscard]] const Vector4& operator[](size_type i) const noexcept;
		
		friend bool operator==(const Matrix& left, const Matrix& right);
		friend bool operator!=(const Matrix& left, const Matrix& right);

		friend std::ostream& operator<<(std::ostream& stream, const Matrix& m);
	};
}
