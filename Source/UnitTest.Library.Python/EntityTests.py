# MIT License Copyright (c) 2020 Jarrett Wendt

import unittest
from FIEAEngine import Entity

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

if __name__ == '__main__':
	unittest.main()
