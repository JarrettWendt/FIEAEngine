#include "pch.h"
#include "Matrix.h"
#include "Macros.h"

namespace Library
{
	const Matrix Matrix::Identity = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };

	Matrix::Matrix(const Vector4 v1, const Vector4 v2, const Vector4 v3, const Vector4 v4) noexcept :
		v{ v1, v2, v3, v4 } {}
	
	Vector4& Matrix::operator[](const size_type i) noexcept
	{
		assertm(i < 4, "Matrix index out of bounds");
		return v[i];
	}
	
	const Vector4& Matrix::operator[](const size_type i) const noexcept
	{
		return const_cast<Matrix*>(this)->operator[](i);
	}
	
	bool operator==(const Matrix& left, const Matrix& right)
	{
		return !std::memcmp(&left, &right, sizeof(Matrix));
	}

	bool operator!=(const Matrix& left, const Matrix& right)
	{
		return !operator==(left, right);
	}

	std::ostream& operator<<(std::ostream& stream, const Matrix& m)
	{
		return stream << "[" <<
			"[" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << "] "
			"[" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << "] "
			"[" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << "] "
			"[" << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << "]]";
	}
}
