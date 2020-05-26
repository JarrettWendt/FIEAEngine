#include "pch.h"
#include "Foo.h"

#include "Macros.h"

namespace UnitTests
{
	Foo::Foo(const int data) :
		data(new int(data)) {}

	Foo::Foo(const Foo& other) :
		data(new int(*other.data)) {}
	
	Foo::Foo(Foo&& other) noexcept :
		data(other.data)
	{
		other.data = nullptr;
	}

	Foo& Foo::operator=(const Foo& other)
	{
		assertm(data, "null Foo::data in Foo::operator=(const Foo&)");
		assertm(other.data, "null other Foo::data in Foo::operator=(const Foo&)");
		*data = *other.data;
		return *this;
	}

	Foo& Foo::operator=(Foo&& other) noexcept
	{
		assertm(data, "null Foo::data in Foo::operator=(Foo&&)");
		*data = 0xF1EA;
		delete data;
		data = other.data;
		return *this;
	}

	Foo::~Foo()
	{
#ifdef _DEBUG
		if (data)
		{
			*data = 0xF1EA;
		}
#endif
		delete data;
		data = nullptr;
	}

	bool Foo::operator==(const Foo& other) const noexcept
	{
		assertm(data, "null Foo::data in Foo::operator==()");
		assertm(other.data, "null other Foo::data in Foo::operator==()");
		return *data == *other.data;
	}

	bool Foo::operator!=(const Foo& other) const noexcept
	{
		return !operator==(other);
	}

	int& Foo::Data() noexcept
	{
		assertm(data, "null Foo::data in Foo::Data()");
		return *data;
	}
	
	int Foo::Data() const noexcept
	{
		return const_cast<Foo*>(this)->Data();
	}
}

std::string std::to_string(const Foo& foo) noexcept
{
	return to_string(foo.Data());
}
