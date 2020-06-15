// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"

#define TEMPLATE_TEST_METHOD_BEGIN(methodName)								\
	template<typename... Ts>												\
	typename std::enable_if<sizeof...(Ts) == 0>::type methodName##__() {}	\
	template<typename T, typename... Ts>									\
	void methodName##__() {

#define TEMPLATE_TEST_METHOD_END(methodName)																																																							\
	methodName##__<Ts...>(); }																																																											\
	static const EXPORT_METHOD ::Microsoft::VisualStudio::CppUnitTestFramework::MemberMethodInfo* CALLING_CONVENTION CATNAME(__GetTestMethodInfo_, methodName)() {																										\
		__GetTestClassInfo(); __GetTestVersion();																																																						\
		ALLOCATE_TESTDATA_SECTION_METHOD																																																								\
		static const ::Microsoft::VisualStudio::CppUnitTestFramework::MethodMetadata s_Metadata = {L"TestMethodInfo", L#methodName, reinterpret_cast<const unsigned char*>(__FUNCTION__), reinterpret_cast<const unsigned char*>(__FUNCDNAME__), __WFILE__, __LINE__};	\
		static ::Microsoft::VisualStudio::CppUnitTestFramework::MemberMethodInfo s_Info = {::Microsoft::VisualStudio::CppUnitTestFramework::MemberMethodInfo::TestMethod, NULL, &s_Metadata};																			\
		s_Info.method.pVoidMethod = reinterpret_cast<::Microsoft::VisualStudio::CppUnitTestFramework::TestClassImpl::__voidFunc>(&ThisClass::methodName);																												\
		return &s_Info; }																																																												\
	void methodName() { methodName##__<char>(); }

using namespace std::string_literals;
using namespace Library;
using namespace Literals;
using namespace UnitTests;

template<typename T>
using Rand = UnitTests::Random<T>;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(StackTests)
	{
		INITIALIZE_CLEANUP

		TEMPLATE_TEST_METHOD_BEGIN(IsEmpty)
		{
			Stack<T> s;
			Assert::IsTrue(s.IsEmpty());
			s.Push(Library::Random::Next<T>());
			Assert::IsFalse(s.IsEmpty());
			s.Pop();
			Assert::IsTrue(s.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(IsEmpty)

		TEMPLATE_TEST_METHOD_BEGIN(Size)
		{
			Stack<T> s;
			Assert::AreEqual(0_z, s.Size());
			s.Push(Library::Random::Next<T>());
			Assert::AreEqual(1_z, s.Size());
			s.Pop();
			Assert::AreEqual(0_z, s.Size());
		}
		TEMPLATE_TEST_METHOD_END(Size)

		TEMPLATE_TEST_METHOD_BEGIN(Top)
		{
			Stack<T> s;
			Assert::ExpectException<std::exception>([&] { s.Top(); });
			T t = Library::Random::Next<T>();
			s.Push(t);
			Assert::AreEqual(t, s.Top());
			const Stack<T> cs = s;
			Assert::AreEqual(t, cs.Peek());
		}
		TEMPLATE_TEST_METHOD_END(Top)

		TEMPLATE_TEST_METHOD_BEGIN(Clear)
		{
			Stack<T> s;
			s.Push(Library::Random::Next<T>());
			s.Clear();
			Assert::IsTrue(s.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(Clear)
	};
}
