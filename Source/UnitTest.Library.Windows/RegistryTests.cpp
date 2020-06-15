// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Registry.h"

using namespace std::string_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;

template<typename T>
using Rand = UnitTests::Random<T>;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(RegistryTests)
	{
		using Attribute = Registry::Attribute;
		
		INITIALIZE_CLEANUP
		
		TEST_METHOD(Find)
		{
		}
	};
}
