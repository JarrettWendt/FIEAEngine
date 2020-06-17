# MIT License Copyright (c) 2020 Jarrett Wendt

from asciimatics.screen import Screen
from FIEAEngine import Engine, Entity

class ScreenManager(Entity.Entity):
	screen = None

	def __init__(self):
		ScreenManager.screen = Screen.open()
	
	def __del__(self):
		screen.close()

	def Refresh(self):
		ScreenManager.screen.refresh()

	def _Update(self):
		self.Refresh()
		super()._Update()

class Particle(Entity.Entity):
	def Draw(self):
		ScreenManager.screen.print_at('X', 0, 0)
	
	def _Update(self):
		self.Draw()
		super()._Update()

Engine.World().Adopt('ScreenManager', ScreenManager())
Engine.World().Adopt(Particle())
