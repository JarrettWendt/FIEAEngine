// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include "RTTI.h"

namespace UnitTests
{
	class Foo : public Library::RTTI
	{
		RTTI_DECLARATIONS(RTTI)
		
		int* data{ nullptr };

	public:
		explicit Foo(int data = 0);
		Foo(const Foo& other);
		Foo(Foo&& other) noexcept;
		Foo& operator=(const Foo& other);
		Foo& operator=(Foo&& other) noexcept;
		virtual ~Foo();

		bool operator==(const Foo& other) const noexcept;
		bool operator!=(const Foo& other) const noexcept;

		[[nodiscard]] int& Data() noexcept;
		[[nodiscard]] int Data() const noexcept;
	};
}

namespace std
{
	[[nodiscard]] std::string to_string(const UnitTests::Foo& foo) noexcept;
}
