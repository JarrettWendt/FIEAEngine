# MIT License Copyright (c) 2020 Jarrett Wendt

import unittest
from FIEAEngine import Math

class MathTests(unittest.TestCase):

	def testIsPrime(self):
		self.assertTrue(Math.IsPrime(51))
		self.assertFalse(Math.IsPrime(112))

	def testNextPrime(self):
		self.assertEqual(3, Math.NextPrime(2))
		self.assertEqual(7, Math.NextPrime(6))

	def testReMap(self):
		self.assertEqual(127, Math.ReMap(511, 0, 1023, 0, 255))
		self.assertAlmostEqual(127.5, Math.ReMap(511.5, 0.0, 1023.0, 0.0, 255.0), delta=0.5)

	def testReMap01(self):
		self.assertEqual(0.5, Math.ReMap01(50, 0, 100))

if __name__ == '__main__':
	unittest.main()
