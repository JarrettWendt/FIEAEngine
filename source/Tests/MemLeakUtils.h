#pragma once
#include "Macros.h"
#include "TestUtils.h"

namespace UnitTests
{
	// to be used with TEST_CASE_METHOD
	class MemLeak
	{
	protected:
		MemLeak()
		{
			Library::Math::NextPrime(500);
			TestUtil::StartMemState();
		}

		~MemLeak()
		{
			TestUtil::EndMemState();
		}

		MOVE_COPY(MemLeak, default)
	};
	
	// to be used with TEMPLATE_TEST_CASE_METHOD
	template<typename T>
	class TemplateMemLeak
	{
	protected:
		TemplateMemLeak()
		{
			Library::Math::NextPrime(500);
			TestUtil::StartMemState();
		}

		~TemplateMemLeak()
		{
			TestUtil::EndMemState();
		}

		MOVE_COPY(TemplateMemLeak, default)
	};
}
