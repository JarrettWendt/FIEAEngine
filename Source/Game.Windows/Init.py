# MIT License Copyright (c) 2020 Jarrett Wendt

from FIEAEngine import Engine, Entity

class RunningMan(Entity.Entity):
	def Print(self):
		print('bleh')
	
	def _Update(self):
		self.Print()
		super()._Update()

Engine.World().Adopt(RunningMan())
