// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Hash.h"

// Macros which expands to defining a recursive template method.
// The purpose of this is to run a TEST_METHOD but with different data types.
// I managed to make one macro which combined these, but intellisense didn't work for blocks within them.
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
	void methodName() { methodName##__<char, uint16_t, size_t, float, double, long, int64_t, std::string>(); }

#define CSTRING_TEST_METHOD_BEGIN(methodName)								\
	template<typename... Ts>												\
	typename std::enable_if<sizeof...(Ts) == 0>::type methodName##__() {}	\
	template<typename T, typename... Ts>									\
	void methodName##__() {

#define CSTRING_TEST_METHOD_END(methodName)																																																							\
	methodName##__<Ts...>(); }																																																											\
	static const EXPORT_METHOD ::Microsoft::VisualStudio::CppUnitTestFramework::MemberMethodInfo* CALLING_CONVENTION CATNAME(__GetTestMethodInfo_, methodName)() {																										\
		__GetTestClassInfo(); __GetTestVersion();																																																						\
		ALLOCATE_TESTDATA_SECTION_METHOD																																																								\
		static const ::Microsoft::VisualStudio::CppUnitTestFramework::MethodMetadata s_Metadata = {L"TestMethodInfo", L#methodName, reinterpret_cast<const unsigned char*>(__FUNCTION__), reinterpret_cast<const unsigned char*>(__FUNCDNAME__), __WFILE__, __LINE__};	\
		static ::Microsoft::VisualStudio::CppUnitTestFramework::MemberMethodInfo s_Info = {::Microsoft::VisualStudio::CppUnitTestFramework::MemberMethodInfo::TestMethod, NULL, &s_Metadata};																			\
		s_Info.method.pVoidMethod = reinterpret_cast<::Microsoft::VisualStudio::CppUnitTestFramework::TestClassImpl::__voidFunc>(&ThisClass::methodName);																												\
		return &s_Info; }																																																												\
	void methodName() { methodName##__<char*, const char*, char* const, const char* const>(); }

using namespace Library;
using namespace UnitTests;

template<typename T>
using Rand = UnitTests::Random<T>;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashTests)
	{
		// No Initialize/Cleanup to check for memory leaks because...
		// - There's no way for my TEMPLATE_TEST_METHODs to know if a T is heap or stack allocated.
		// - I really don't expect any hash functions to leak memory.
	public:		
		TEMPLATE_TEST_METHOD_BEGIN(Function)
		{
			Hash<T> hash = Hash<T>();

			const T a = Library::Random::Next<T>();

			Assert::AreEqual(hash(a), hash(a), L"Same objects should hash to equivalent values");

			T b = Rand<T>::NotEqualTo(a);
			Assert::AreNotEqual(hash(a), hash(b), L"Hashes of different data should (most likely) not be equivalent");

			const T a1 = a;

			Assert::AreEqual(hash(a), hash(a1), L"Hashes of same data should always be equivalent.");
		}
		TEMPLATE_TEST_METHOD_END(Function)

		CSTRING_TEST_METHOD_BEGIN(CString)
		{
			Hash<T> hash = Hash<T>();

			const T a = Rand<T>::Next();

			Assert::AreEqual(hash(a), hash(a), L"Same objects should hash to equivalent values");

			std::remove_const_t<T> b{ nullptr };
			do
			{
				delete[] b;
				b = Rand<T>::Next();
			} while (!std::strcmp(a, b));
			Assert::AreNotEqual(hash(a), hash(b), L"Hashes of different data should (most likely) not be equivalent");

			const T a1 = a;

			Assert::AreEqual(hash(a), hash(a1), L"Hashes of same data should always be equivalent.");
			
			delete[] a;
			delete[] b;
		}
		CSTRING_TEST_METHOD_END(CString)
	};
}
