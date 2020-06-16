// MIT License Copyright (c) 2020 Jarrett Wendt

#pragma once

#include <sstream>

#include "CppUnitTest.h"

#include "Macros.h"
#include "Util.h"

namespace UnitTests
{
	namespace msvscpput = Microsoft::VisualStudio::CppUnitTestFramework;

	template<typename T>
	auto ToString(const T& t)
	{
		return msvscpput::ToString(t);
	}

	/**
	 * The whole point of this is to address the fact that Microsoft's Assert always calls ToString.
	 * This can severely slow down the speed of unit tests.
	 * You only really want them stringified when something goes wrong anyway, which is what this does.
	 */
	class Assert final
	{
		STATIC_CLASS(Assert)

	public:
#pragma region stringstream
		template<typename Expected, typename Actual>
		static void AreEqual(const Expected& expected, const Actual& actual, const std::stringstream& message)
		{
			AreEqual(expected, actual, message.str());
		}

		template<typename Expected, typename Actual>
		static void AreNotEqual(const Expected& expected, const Actual& actual, const std::stringstream& message)
		{
			AreNotEqual(expected, actual, message.str());
		}

		template<typename Expected, typename Actual>
		static void AreSame(const Expected& expected, const Actual& actual, const std::stringstream& message)
		{
			AreSameEqual(expected, actual, message.str());
		}

		template<typename Expected, typename Actual>
		static void AreNotSame(const Expected& expected, const Actual& actual, const std::stringstream& message)
		{
			AreNotSame(expected, actual, message.str());
		}

		template<typename Exception>
		static void ExpectException(const std::function<void()>& functor, const std::stringstream& message)
		{
			ExpectException<Exception>(functor, message.str());
		}

		template<typename T>
		static void IsNull(const T& ptr, const std::stringstream& message)
		{
			IsNull(ptr, message.str());
		}

		template<typename T>
		static void IsNotNull(const T& ptr, const std::stringstream& message)
		{
			IsNotNull(ptr, message.str());
		}

		template<typename T>
		static void IsTrue(const T& b, const std::stringstream& message)
		{
			IsTrue(b, message.str());
		}

		template<typename T>
		static void IsFalse(const T& b, const std::stringstream& message)
		{
			IsFalse(b, message.str());
		}

		static void Fail(const std::stringstream& message)
		{
			Fail(message.str());
		}
#pragma endregion
		
#pragma region wstringstream
		template<typename Expected, typename Actual>
		static void AreEqual(const Expected& expected, const Actual& actual, const std::wstringstream& message)
		{
			AreEqual(expected, actual, message.str());
		}

		template<typename Expected, typename Actual>
		static void AreNotEqual(const Expected& expected, const Actual& actual, const std::wstringstream& message)
		{
			AreNotEqual(expected, actual, message.str());
		}

		template<typename Expected, typename Actual>
		static void AreSame(const Expected& expected, const Actual& actual, const std::wstringstream& message)
		{
			AreSameEqual(expected, actual, message.str());
		}

		template<typename Expected, typename Actual>
		static void AreNotSame(const Expected& expected, const Actual& actual, const std::wstringstream& message)
		{
			AreNotSame(expected, actual, message.str());
		}

		template<typename Exception>
		static void ExpectException(const std::function<void()>& functor, const std::wstringstream& message)
		{
			ExpectException<Exception>(functor, message.str());
		}

		template<typename T>
		static void IsNull(const T& ptr, const std::wstringstream& message)
		{
			IsNull(ptr, message.str());
		}

		template<typename T>
		static void IsNotNull(const T& ptr, const std::wstringstream& message)
		{
			IsNotNull(ptr, message.str());
		}

		template<typename T>
		static void IsTrue(const T& b, const std::wstringstream& message)
		{
			IsTrue(b, message.str());
		}

		template<typename T>
		static void IsFalse(const T& b, const std::wstringstream& message)
		{
			IsFalse(b, message.str());
		}

		static void Fail(const std::wstringstream& message)
		{
			Fail(message.str());
		}
#pragma endregion
		
#pragma region string
		template<typename Expected, typename Actual>
		static void AreEqual(const Expected& expected, const Actual& actual, const std::string& message)
		{
			using namespace std;
			using namespace Library;
			using namespace Library::Util;
			AreEqual(expected, actual, to_wstring(message));
		}

		template<typename Expected, typename Actual>
		static void AreNotEqual(const Expected& expected, const Actual& actual, const std::string& message)
		{
			using namespace std;
			using namespace Library;
			using namespace Library::Util;
			AreNotEqual(expected, actual, to_wstring(message));
		}

		template<typename Expected, typename Actual>
		static void AreSame(const Expected& expected, const Actual& actual, const std::string& message)
		{
			using namespace std;
			using namespace Library;
			using namespace Library::Util;
			AreSameEqual(expected, actual, to_wstring(message));
		}

		template<typename Expected, typename Actual>
		static void AreNotSame(const Expected& expected, const Actual& actual, const std::string& message)
		{
			using namespace std;
			using namespace Library;
			using namespace Library::Util;
			AreNotSame(expected, actual, to_wstring(message));
		}

		template<typename Exception>
		static void ExpectException(const std::function<void()>& functor, const std::string& message)
		{
			using namespace std;
			using namespace Library;
			using namespace Library::Util;
			ExpectException<Exception>(functor, to_wstring(message));
		}

		template<typename T>
		static void IsNull(const T& ptr, const std::string& message)
		{
			using namespace std;
			using namespace Library;
			using namespace Library::Util;
			IsNull(ptr, to_wstring(message));
		}

		template<typename T>
		static void IsNotNull(const T& ptr, const std::string& message)
		{
			using namespace std;
			using namespace Library;
			using namespace Library::Util;
			IsNotNull(ptr, to_wstring(message));
		}

		template<typename T>
		static void IsTrue(const T& b, const std::string& message)
		{
			using namespace std;
			using namespace Library;
			using namespace Library::Util;
			IsTrue(b, to_wstring(message));
		}

		template<typename T>
		static void IsFalse(const T& b, const std::string& message)
		{
			using namespace std;
			using namespace Library;
			using namespace Library::Util;
			IsFalse(b, to_wstring(message));
		}

		static void Fail(const std::string& message)
		{
			using namespace std;
			using namespace Library;
			using namespace Library::Util;
			Fail(to_wstring(message));
		}
#pragma endregion

#pragma region wstring
		template<typename Expected, typename Actual>
		static void AreEqual(const Expected& expected, const Actual& actual, const std::wstring& message = L"")
		{
			if (!(expected == actual))
			{
				std::wstringstream stream;
				stream << "AreEqual. Expected: " << msvscpput::ToString(expected) << ", ";
				stream << "Actual: " << msvscpput::ToString(actual) << " ";
				stream << message;
				Fail(stream);
			}
		}

		template<typename Expected, typename Actual>
		static void AreNotEqual(const Expected& expected, const Actual& actual, const std::wstring& message = L"")
		{
			if (!(expected != actual))
			{
				std::wstringstream stream;
				stream << "AreNotEqual. Expected: " << msvscpput::ToString(expected) << ", ";
				stream << "Actual: " << msvscpput::ToString(actual) << " ";
				stream << message;
				Fail(stream);
			}
		}

		template<typename Expected, typename Actual>
		static void AreSame(const Expected& expected, const Actual& actual, const std::wstring& message = L"")
		{
			if (&expected != &actual)
			{
				std::wstringstream stream;
				stream << "AreSame. Expected: " << msvscpput::ToString(expected) << ", ";
				stream << "Actual: " << msvscpput::ToString(actual) << " ";
				stream << message;
				Fail(stream);
			}
		}

		template<typename Expected, typename Actual>
		static void AreNotSame(const Expected& expected, const Actual& actual, const std::wstring& message = L"")
		{
			if (&expected != &actual)
			{
				std::wstringstream stream;
				stream << "AreNotSame. Expected: " << msvscpput::ToString(expected) << ", ";
				stream << "Actual: " << msvscpput::ToString(actual) << " ";
				stream << message;
				Fail(stream);
			}
		}

		template<typename Exception, typename Functor>
		static void ExpectException(Functor functor, const std::wstring& message = L"")
		{
			msvscpput::Assert::ExpectException<Exception>(functor, message.c_str());
		}

		template<typename T>
		static void IsNull(const T& ptr, const std::wstring& message = L"")
		{
			if (ptr != nullptr)
			{
				Fail(message.c_str());
			}
		}

		template<typename T>
		static void IsNotNull(const T& ptr, const std::wstring& message = L"")
		{
			if (ptr == nullptr)
			{
				Fail(message.c_str());
			}
		}

		template<typename T>
		static void IsTrue(const T& b, const std::wstring& message = L"")
		{
			if (!bool(b))
			{
				Fail(message);
			}
		}

		template<typename T>
		static void IsFalse(const T& b, const std::wstring& message = L"")
		{
			IsTrue(!bool(b), message);
		}

		static void Fail(const std::wstring& message = L"")
		{
			msvscpput::Assert::Fail(message.c_str());
		}
#pragma endregion
	};
}

/**
 * Macro for creating the Initialize/Cleanup methods for every test.
 */
#if defined(DEBUG) || defined(_DEBUG)
#define INITIALIZE_CLEANUP																	\
private:																					\
inline static _CrtMemState startMemState;													\
public:																						\
TEST_CLASS_INITIALIZE(ClassInitialize)														\
{																							\
	Math::NextPrime(10000);																	\
	Enum<Digit>::ToString(Digit());															\
	Enum<Digit>::FromString("Zero");														\
	Enum<Datum::Type>::ToString(Datum::Type());												\
	Enum<Datum::Type>::FromString("Zero");													\
	Enum<Input::KeyCode>::ToString(Input::KeyCode());										\
	Enum<Input::KeyCode>::FromString("None");												\
	Enum<Input::KeyState>::ToString(Input::KeyState());										\
	Enum<Input::KeyState>::FromString("up");												\
}																							\
TEST_METHOD_INITIALIZE(MethodInitialize)													\
{																							\
	Engine::Init({});																		\
	Coroutines::StopAll();																	\
	Engine::Update();																		\
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);													\
	_CrtMemCheckpoint(&startMemState);														\
}																							\
TEST_METHOD_CLEANUP(MethodCleanup)															\
{																							\
	Coroutines::StopAll();																	\
	Engine::Update();																		\
	_CrtMemState endMemState, diffMemState;													\
	_CrtMemCheckpoint(&endMemState);														\
	if (_CrtMemDifference(&diffMemState, &startMemState, &endMemState))						\
	{																						\
		_CrtMemDumpStatistics(&diffMemState);												\
		Assert::Fail(L"Memory Leaks!");														\
	}																						\
}
#else
#define INITIALIZE_CLEANUP
#endif
