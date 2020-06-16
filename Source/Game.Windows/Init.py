# MIT License Copyright (c) 2020 Jarrett Wendt

from time import time, ctime
print('Today is', ctime(time()))

from FIEAEngine import Entity

class SquarePrinter(Entity.Entity):
	def _Update(self):
		super().Update()
		print('.---.')
		print('|   |')
		print('|   |')
		print("'---'")


