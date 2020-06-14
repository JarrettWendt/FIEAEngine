# MIT License Copyright (c) 2020 Jarrett Wendt

import unittest
from FIEAEngine import Entity

class MyPyEntity(Entity.Entity):
	def _Update(self):
		super()._Update()

class MyPyEntityTests(unittest.TestCase):

	def testCtor(self):
		e = MyPyEntity()

	def testUpdate(self):
		e = MyPyEntity()
		e._Update()

class EntityTests(unittest.TestCase):

	def testCtor(self):
		e = Entity.Entity('hi')
		self.assertEqual('hi', e.name)
		e = Entity.Entity(name='hello')
		self.assertEqual('hello', e.name)

	def testName(self):
		e = Entity.Entity()
		self.assertEqual('Entity', e.name)
		e.name = 'jimbob'
		self.assertEqual('jimbob', e.name)

	def testEnabled(self):
		e = Entity.Entity()
		self.assertTrue(e.enabled)
		e.enabled = False
		self.assertFalse(e.enabled)

	def testNumChildren(self):
		e = Entity.Entity()
		self.assertEqual(0, e.NumChildren())

	def testHasChildren(self):
		e = Entity.Entity()
		self.assertFalse(e.HasChildren())

	def testChild(self):
		e = Entity.Entity()
		self.assertIsNone(e.Child('hi'))

	def testAdopt(self):
		p = Entity.Entity()
		c = Entity.Entity()
		c2 = p.Adopt('child', c)
		self.assertIs(c, c2)
		self.assertEqual('child', c.name)

		p = Entity.Entity()
		c = Entity.Entity()
		c2 = p.Adopt(name='child', child=c)
		self.assertIs(c, c2)
		self.assertEqual('child', c.name)

		p = Entity.Entity()
		c = Entity.Entity()
		c2 = p.Adopt(c)
		self.assertIs(c, c2)

	def testOrphan(self):
		p = Entity.Entity()
		c = p.Adopt(Entity.Entity())
		c.Orphan()
		self.assertIsNone(c.parent)

	def testParent(self):
		p = Entity.Entity()
		c = Entity.Entity()
		c.parent = p
		self.assertIsNotNone(c.parent)

	def testRichCompare(self):
		a = Entity.Entity()
		b = Entity.Entity()
		self.assertEqual(a, b)
		a.Adopt(b)
		self.assertNotEqual(a, b)

if __name__ == '__main__':
	unittest.main()
