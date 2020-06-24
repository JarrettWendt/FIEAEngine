#pragma once
#include "Macros.h"
#include "TestUtils.h"

namespace UnitTests
{
	class MemLeak
	{
	protected:
		MemLeak()
		{
			TestUtil::StartMemState();
		}

		~MemLeak()
		{
			TestUtil::EndMemState();
		}

		MOVE_COPY(MemLeak, default)
	};
}
