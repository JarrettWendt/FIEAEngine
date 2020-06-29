#pragma once
#include "Macros.h"
#include "TestUtils.h"

namespace UnitTests
{
	// to be used with TEST_CASE_METHOD
	class MemLeak
	{
		template<typename T>
		friend class TemplateMemLeak;
	protected:
		MemLeak()
		{
			MemLeak::Singleton();
			TestUtil::StartMemState();
		}

		~MemLeak()
		{
			TestUtil::EndMemState();
		}

		MOVE_COPY(MemLeak, default)

	private:
		static void Singleton()
		{
			static bool b{ false };
			if (b)
			{
				return;
			}
			Library::Math::NextPrime(500);
		}
	};
	
	// to be used with TEMPLATE_TEST_CASE_METHOD
	template<typename T>
	class TemplateMemLeak
	{
	protected:
		TemplateMemLeak()
		{
			MemLeak::Singleton();
			TestUtil::StartMemState();
		}

		~TemplateMemLeak()
		{
			TestUtil::EndMemState();
		}

		MOVE_COPY(TemplateMemLeak, default)
	};
}
