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

	def __init__(self, speed, idx):
		self.speed = speed
		self.localTransform.translation.y = idx
		self.idx = idx
		
	def _Update(self):
		colour, attr = SubParticle.attrs[int(self.idx)]
		ScreenManager.screen.print_at(choice(ascii_letters), int(self.worldTransform.translation.x), int(self.worldTransform.translation.y), colour, attr)
		#print('Particle' + self.parent.name + 'Sub' + self.name + ' (' + str(self.localTransform.translation.x) + ', ' + str(self.localTransform.translation.y) + ')')
		super()._Update()

# A ParticleSystem is made up of these
# Manages SubParticles
class Particle(Entity.Entity):
	maxChildren = len(SubParticle.attrs)

	def __init__(self):
		self.localTransform.translation.x = randint(0, ScreenManager.screen.width)
		self.localTransform.translation.y = -Particle.maxChildren
		self.speed = 5 * random()

		# add all of our sub-particles
		childCounter = 0
		while childCounter < Particle.maxChildren:
			self.Adopt(str(childCounter), SubParticle(self.speed, childCounter))
			childCounter += 1

	def _Update(self):
		self.localTransform.translation.y += self.speed * Time.Delta()
		# destroy this particle when we reach far enough below the screen
		if self.localTransform.translation.y - Particle.maxChildren > ScreenManager.screen.height:
			self.Orphan()
		super()._Update()
		#print('Particle (' + str(self.localTransform.translation.x) + ', ' + str(self.localTransform.translation.y) + ')\n')

# Manages Particles
class ParticleSystem(Entity.Entity):
	maxChildren = 100

	def __init__(self):
		self.name = 'ParticleSystem'
		self.childCounter = 0

	def _Update(self):
		if self.numChildren < ParticleSystem.maxChildren:
			self.childCounter += 1
			self.Adopt(str(self.childCounter), Particle())
		super()._Update()
		#print('ParticleSystem ' + str(self.childCounter))

Engine.World().Adopt(ScreenManager())
Engine.World().Adopt(ParticleSystem())
