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
	TEST_CLASS(QueueTests)
	{
		INITIALIZE_CLEANUP

		TEMPLATE_TEST_METHOD_BEGIN(IsEmpty)
		{
			Queue<T> q;
			Assert::IsTrue(q.IsEmpty());
			q.Enqueue(Library::Random::Next<T>());
			Assert::IsFalse(q.IsEmpty());
			q.Dequeue();
			Assert::IsTrue(q.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(IsEmpty)

		TEMPLATE_TEST_METHOD_BEGIN(Size)
		{
			Queue<T> q;
			Assert::AreEqual(0_z, q.Size());
			q.Enqueue(Library::Random::Next<T>());
			Assert::AreEqual(1_z, q.Size());
			q.Dequeue();
			Assert::AreEqual(0_z, q.Size());
		}
		TEMPLATE_TEST_METHOD_END(Size)

		TEMPLATE_TEST_METHOD_BEGIN(Front)
		{
			Queue<T> q;
			Assert::ExpectException<std::exception>([&] { q.Front(); });
			T t = Library::Random::Next<T>();
			q.Enqueue(t);
			Assert::AreEqual(t, q.Front());
			const Queue<T> cq = q;
			Assert::AreEqual(t, cq.Front());
		}
		TEMPLATE_TEST_METHOD_END(Front)

		TEMPLATE_TEST_METHOD_BEGIN(Back)
		{
			Queue<T> q;
			Assert::ExpectException<std::exception>([&] { q.Back(); });
			T t = Library::Random::Next<T>();
			q.Enqueue(t);
			Assert::AreEqual(t, q.Back());
			const Queue<T> cq = q;
			Assert::AreEqual(t, cq.Back());
		}
		TEMPLATE_TEST_METHOD_END(Back)

		TEMPLATE_TEST_METHOD_BEGIN(Clear)
		{
			Queue<T> q;
			q.Enqueue(Library::Random::Next<T>());
			q.Clear();
			Assert::IsTrue(q.IsEmpty());
		}
		TEMPLATE_TEST_METHOD_END(Clear)
	};
}
