# MIT License Copyright (c) 2020 Jarrett Wendt

from time import time, ctime
print('Today is', ctime(time()))

from FIEAEngine import Engine, Entity

class SquarePrinter(Entity.Entity):
	def _Update(self):
		print(' ___ ')
		print('|   |')
		print('|___|')
		super()._Update()

Engine.World().Adopt(SquarePrinter())
