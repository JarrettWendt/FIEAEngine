#include "../../pch.h"

#include "python/pch.h"
#include "python/modules/FIEAEngine.h"

using namespace std::string_literals;
using namespace Library;
using namespace Library::Literals;

#define TEST(name) TEST_CASE_METHOD(MemLeak, "py::Entity::" #name, "[py::Entity]")

namespace UnitTests
{
	TEST(Transform)
	{
		// TODO: Have a better way to unit test Python because this hijacks the console from Catch2
//		PyImport_AppendInittab("FIEAEngine", &PyInit_FIEAEngine);
//		Py_Initialize();
//		PyRun_SimpleString(R"(
//import unittest
//from FIEAEngine import Entity
//
//class Transform(unittest.TestCase):
//	def test_ParentChangesChild(self):
//		p = Entity.Entity()
//		c = p.Adopt(Entity.Entity())
//
//		p.worldTransform.translation.x = 1
//
//		self.assertEqual(1, p.localTransform.translation.x)
//		self.assertEqual(1, c.worldTransform.translation.x)
//		self.assertEqual(0, c.localTransform.translation.x)
//
//	def test_ChildDoesNotChangeParent(self):
//		p = Entity.Entity()
//		c = p.Adopt(Entity.Entity())
//
//		c.worldTransform.translation.x = 1;
//
//		self.assertEqual(1, c.localTransform.translation.x)
//		self.assertEqual(0, p.localTransform.translation.x)
//		self.assertEqual(0, p.worldTransform.translation.x)
//
//if __name__ == '__main__':
//    unittest.main()
//)");
//		Py_Finalize();
	}
}
