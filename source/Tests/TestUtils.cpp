#include "pch.h"
#include "TestUtils.h"

namespace UnitTests::TestUtil
{
#if defined(DEBUG) || defined(_DEBUG)
	static _CrtMemState startMemState;
#endif
	
	void StartMemState()
	{
#if defined(DEBUG) || defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
		_CrtMemCheckpoint(&startMemState);
#endif
	}

	void EndMemState()
	{
#if defined(DEBUG) || defined(_DEBUG)
		_CrtMemState endMemState, diffMemState;
		_CrtMemCheckpoint(&endMemState);
		if (_CrtMemDifference(&diffMemState, &startMemState, &endMemState))	
		{
			_CrtMemDumpStatistics(&diffMemState);
			REQUIRE_FALSE("Memory Leaks!");
		}
#endif
	}
}
