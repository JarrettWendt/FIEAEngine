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
		self.name = 'ScreenManager'
	
	def __del__(self):
		screen.close()

	def _Update(self):
		ScreenManager.screen.refresh()
		super()._Update()

# A Particle is made up of these
class SubParticle(Entity.Entity):
	color = Screen.COLOUR_GREEN
	Attr = namedtuple("Attr", ['colour', 'attr'])
	attrs =	[
		Attr(Screen.COLOUR_BLACK, Screen.A_NORMAL),
		Attr(color, Screen.A_NORMAL),
		Attr(color, Screen.A_NORMAL),
		Attr(color, Screen.A_NORMAL),
		Attr(color, Screen.A_NORMAL),
		Attr(color, Screen.A_BOLD),
		Attr(color, Screen.A_BOLD),
		Attr(color, Screen.A_BOLD),
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
		#print('Draw (' + str(self.x) + ', ' + str(self.y) + ')')

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
		self.childCounter = 0
	
	def _Update(self):
		newY = self.y + self.speed * Time.Delta()
		# if we reached a new index, add a new sub-particle
		if int(self.y) < int(newY) and self.numChildren < Particle.maxChildren:
			self.childCounter += 1
			self.Adopt(self.name + 'SubParticle' + str(self.childCounter), SubParticle(self.speed, self.x, self.y))
		self.y = newY
		# destroy this particle when we reach far enough below the screen
		if self.y - Particle.maxChildren > ScreenManager.screen.height:
			self.Orphan()
		super()._Update()
		#print('Particle (' + str(self.x) + ', ' + str(self.y) + ')\n')

# Manages Particles
class ParticleSystem(Entity.Entity):
	maxChildren = 100

	def __init__(self):
		self.name = 'ParticleSystem'
		self.childCounter = 0

	def _Update(self):
		if self.numChildren < ParticleSystem.maxChildren:
			self.childCounter += 1
			self.Adopt('Particle' + str(self.childCounter), Particle())
		super()._Update()
		#print('ParticleSystem ' + str(self.childCounter))

Engine.World().Adopt(ScreenManager())
Engine.World().Adopt(ParticleSystem())
