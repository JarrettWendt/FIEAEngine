# MIT License Copyright (c) 2020 Jarrett Wendt

import unittest
from FIEAEngine import Entity

class EntityTests(unittest.TestCase):

	def testCtor(self):
		e = Entity.Entity('hi')
		self.assertEqual('hi', e.GetName())
		e = Entity.Entity(name='hello')
		self.assertEqual('hello', e.GetName())

	def testGetName(self):
		e = Entity.Entity()
		self.assertEqual('Entity', e.GetName())

if __name__ == '__main__':
	unittest.main()
