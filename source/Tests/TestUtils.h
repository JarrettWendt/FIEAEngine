#pragma once

#define SKIP_TYPE(Type)							\
if constexpr (std::is_same_v<TestType, Type>)	\
{												\
	return;										\
}

namespace UnitTests::TestUtil
{
	void StartMemState();

	void EndMemState();
}
