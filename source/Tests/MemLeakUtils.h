#pragma once
//#include "Coroutine.h"
#include "Datum.h"
#include "Engine.h"
#include "Macros.h"
#include "Input.h"

#include "Digit.h"
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
			Library::Memory::Manager::Defrag();
			TestUtil::StartMemState();
		}

		~MemLeak()
		{
			Library::Memory::Manager::Defrag();
			TestUtil::EndMemState();
		}

		MOVE_COPY(MemLeak, default)

	private:
		static void Singleton()
		{
			using namespace Library;
			
			static bool b{ false };
			if (b)
			{
				return;
			}
			b = true;
			
			Math::NextPrime(500);
			Enum<Digit>::ToString(Digit());
			Enum<Digit>::FromString("Zero");
			Enum<Datum::Type>::ToString(Datum::Type());
			Enum<Datum::Type>::FromString("Zero");
			Enum<Input::KeyCode>::ToString(Input::KeyCode());
			Enum<Input::KeyCode>::FromString("None");
			Enum<Input::KeyState>::ToString(Input::KeyState());
			Enum<Input::KeyState>::FromString("up");
		}
	};
	
	// to be used with TEMPLATE_TEST_CASE_METHOD
	template<typename T>
	class TemplateMemLeak
	{
		MemLeak memLeak{};
	};
}
