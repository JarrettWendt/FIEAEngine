#pragma once

namespace Library
{
	struct Vector2
	{
		using size_type = size_t;

#pragma warning(push)
#pragma warning(disable: 4201) // nameless struct/union
		union
		{
			struct
			{
				float x, y;
			};
			float v[2];
		};
#pragma warning(pop)

		[[nodiscard]] float& operator[](size_type i) noexcept;
		[[nodiscard]] float operator[](size_type i) const noexcept;

		[[nodiscard]] friend bool operator==(const Vector2& left, const Vector2& right);
		[[nodiscard]] friend bool operator!=(const Vector2& left, const Vector2& right);

		friend std::ostream& operator<<(std::ostream& stream, const Vector2& v);
	};
}
