#pragma once

namespace Library
{
	struct Vector3
	{
		using size_type = size_t;

#pragma warning(push)
#pragma warning(disable: 4201) // nameless struct/union
		union
		{
			struct
			{
				float x, y, z;
			};
			float v[3];
		};
#pragma warning(pop)

		[[nodiscard]] float& operator[](size_type i) noexcept;
		[[nodiscard]] float operator[](size_type i) const noexcept;

		[[nodiscard]] friend bool operator==(const Vector3& left, const Vector3& right);
		[[nodiscard]] friend bool operator!=(const Vector3& left, const Vector3& right);

		friend std::ostream& operator<<(std::ostream& stream, const Vector3& v);
	};
}
