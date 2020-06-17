# MIT License Copyright (c) 2020 Jarrett Wendt

from collections import namedtuple
from random import randint, random, choice
from string import ascii_letters
from asciimatics.screen import Screen
from FIEAEngine import Engine, Entity, Time

# Owner of the Screen
class ScreenManager(Entity.Entity):
	screen = None

	def __init__(self):
		ScreenManager.screen = Screen.open()
	
	def __del__(self):
		screen.close()

	def _Update(self):
		ScreenManager.screen.refresh()
		super()._Update()

# A Particle is made up of these
class SubParticle(Entity.Entity):
	Attr = namedtuple("Attr", ['colour', 'attr'])
	attrs =	[
		Attr(Screen.COLOUR_BLACK, Screen.A_NORMAL),
		Attr(Screen.COLOUR_GREEN, Screen.A_NORMAL),
		Attr(Screen.COLOUR_GREEN, Screen.A_NORMAL),
		Attr(Screen.COLOUR_GREEN, Screen.A_NORMAL),
		Attr(Screen.COLOUR_GREEN, Screen.A_NORMAL),
		Attr(Screen.COLOUR_GREEN, Screen.A_BOLD),
		Attr(Screen.COLOUR_GREEN, Screen.A_BOLD),
		Attr(Screen.COLOUR_GREEN, Screen.A_BOLD),
		Attr(Screen.COLOUR_WHITE, Screen.A_BOLD),
		Attr(Screen.COLOUR_WHITE, Screen.A_REVERSE),
	]

	def __init__(self, speed, x, y):
		# TODO: Get parent's speed in Draw
		self.speed = speed
		self.x = x
		self.y = y
		self.idx = len(SubParticle.attrs)
		
	def Draw(self):
		colour, attr = SubParticle.attrs[int(self.idx)]
		ScreenManager.screen.print_at(choice(ascii_letters), self.x, int(self.y), colour, attr)

	def _Update(self):
		self.idx -= self.speed * Time.Delta()
		if self.idx < 0:
			self.Orphan()
		else:
			self.Draw()
		super()._Update()

# A ParticleSystem is made up of these
# Manages SubParticles
class Particle(Entity.Entity):
	maxChildren = len(SubParticle.attrs)

	def __init__(self):
		self.x = randint(0, ScreenManager.screen.width)
		self.y = 0
		self.speed = 5 * random()
	
	def _Update(self):
		newY = self.y + self.speed * Time.Delta()
		# if we reached a new index, add a new sub-particle
		if int(self.y) < int(newY) and self.numChildren < Particle.maxChildren:
			self.Adopt(self.name + 'SubParticle' + str(self.numChildren), SubParticle(self.speed, self.x, self.y))
		self.y = newY
		# destroy this particle when we reach far enough below the screen
		if self.y - Particle.maxChildren > ScreenManager.screen.height:
			self.Orphan()
		super()._Update()

# Manages Particles
class ParticleSystem(Entity.Entity):
	def _Update(self):
		if self.numChildren < 100:
			self.Adopt('Particle' + str(self.numChildren), Particle())
		super()._Update()

Engine.World().Adopt('ScreenManager', ScreenManager())
Engine.World().Adopt(ParticleSystem())
