#!/usr/bin/python

import os, sys
from pathlib import Path

# TODO:
# Don't assume the filename is the class name.
# Do some syntax checking with the KEYWORDS instead of just ignoring them.
# Come up with a cleaner way to encode the indentation, perhaps at the cost of double-parsing the string.
# Use a library for real C++ parsing.

# The characters that could denote the end of a declaration.
DELIMS = ['[', '{', '=', ';']
KEYWORDS = ['inline', 'const', 'constexpr', 'static', 'class']
ACCESS_MODIFIERS = ['public', 'private', 'protected']

# Gets the prefix before any occurrences of the passed strings.
def trimTrailing(string, strings):
	return string if len(strings) == 0 else trimTrailing(string.split(strings[0])[0], strings[1:])

# Gets postfix after any occurrences of the passed strings.
def trimLeading(string, strings):
	return string if len(strings) == 0 else trimLeading(string.split(strings[0])[-1], strings[1:])

# Returns whether or not the line of code is commented out.
def isCommentedOut(line):
	line = line.lstrip()
	return line.startswith('//') or line.startswith('/*')

# Returns the substring between the prefix and postfix. For example:
# substringBetween('[blah]', '[', ']') -> 'blah'
def substringBetween(string, prefix, postfix):
	return string.split(prefix)[1].split(postfix)[0]

def containsAll(string, strings):
	return True if len(strings) == 0 else strings[0] in string and containsAll(string, strings[1:])

# A single Attribute within an Attributed class.
class Attribute:
	def __init__(self, attributed, string):
		self.attributed = attributed
		self.__tokenize(string)

	def __str__(self):
		ret = '{ '
		ret += '"' + self.name + '", '
		ret += str(self.count) + ', '
		ret += 'offsetof(' + self.attributed.scopeResolvedClass() + ', ' + self.name + '), ' if self.isMember else '0, '
		ret += 'Datum::TypeOf<' + self.type + '>, '
		return ret + ' }'

	def __tokenize(self, string):
		# Case where this is an Attribute without a data member.
		if '[[Attribute' in string:
			self.isMember = self.isPointer = False;
			tokens = [token.strip() for token in string.split('(')[1].split(')')[0].split(',')]
			self.type = tokens[0]
			self.name = substringBetween(tokens[1], '"', '"')
			# This could be an array.
			if '[' in self.type:
				self.count = int(substringBetween(self.type, '[', ']'))
				self.type = trimTrailing(self.type, '[')
			else:
				self.count = 0
			return tokens
		
		# Case where this is an Attribute with a data member.
		self.isMember = True;

		# Get the string before any of the delims.
		# Get rid of trailing and leading whitespace.
		# Split on whitespace.
		# Get rid of occurrences of the ignored keywords.
		tokens = list(filter(lambda token: token not in KEYWORDS, trimTrailing(string, DELIMS).strip().split()))
		self.isPointer = '*' in tokens[0]
		self.type = trimTrailing(tokens[0], '*')
		self.name = tokens[1]
		# See if this is an array and get the count.
		self.count = substringBetween(string, '[', ']') if '[' in string else 1

# One instance represents one class inheriting from Attributed
class Attributed:
	def __init__(self, path = ''):
		# A list of every Attribute within this class.
		self.attributes = []
		self.namespace = ''
		self.path = path
		self.className = path.name.split('.')[0]
		self.parentName = ''
		self.parent = None

	def __str__(self):
		ret = '\t\t{\n\t\t\t' + self.scopeResolvedClass() + '::typeID,\n\t\t\t{\n\t\t\t\t'
		ret += (self.parent.scopeResolvedClass() if self.parent is not None else 'Attributed') + '::typeID,\n\t\t\t\t{\n'
		if len(self.attributes) > 0:
			for attribute in self.attributes:
				ret += '\t\t\t\t\t' + str(attribute) + ',\n'
		else:
			ret += '\t\t\t\t\tArray<Attribute>()\n'
		return ret + '\t\t\t\t}\n\t\t\t}\n\t\t}'
	
	def __eq__(self, other):
		# All Attributeds should have a path and they should all be unique.
		return other is not None and self.path == other.path
	
	def addAttribute(self, string):
		self.attributes.append(Attribute(self, string))
	
	# Is only valid if it all of its members have data.
	def isValid(self):
		return self.namespace != '' and self.className != '' and self.parentName != '' and (self.parentName == 'Attributed' or self.parent != None)
	
	# Get the scope resolved class. Ex: 'MyNamespace::MyClass'
	def scopeResolvedClass(self):
		return self.namespace + '::' + self.className

rootPaths = [os.path.abspath(arg) for arg in sys.argv[1:]] if len(sys.argv) > 1 else [os.path.abspath('..')]
destFile = rootPaths[0] + '/.generated/Registry.generated.cpp'

print("Generating Reflectable Attributes for Attributeds under paths " + str(rootPaths))
print("Will export to " + str(destFile))

attributeds = []

# Keep going through everything if there's more attributeds to discover.
moreAttributeds = True
while moreAttributeds:
	moreAttributeds = False

	# Recursively go through all header files.
	for rootPath in rootPaths:
		for extension in ['*.h', "*.hh", "*.hpp"]:
			for path in Path(rootPath).rglob(extension):
				with open(path, 'r') as file:
					attributed = Attributed(path)

					# Skip if we've already seen this one.
					if attributed in attributeds:
						continue
					
					prevline = '';
					for line in file:
						# I'm pretty sure this is is a comprehensive enough check to find the line that declares the class and its parent.
						if 'class' in line and ':' in line and attributed.className in line and attributed.parentName == '':
							# Grab the first thing in the inheritance list.
							attributed.parentName = trimLeading(line.split(',')[0].split(':')[-1], ACCESS_MODIFIERS).strip()
							
							# See if the previous line contains a variadic template. If so, let's assume only empty variadics of this are being used by children.
							if containsAll(prevline, ['template', '<', '...', '>']):
								attributed.className += '<>'

							# Go through all existing Attributeds to see if its parent is there.
							# If not, then either this isn't an Attributed or we'll catch it on another pass.
							if attributed.parentName != 'Attributed':
								for other in attributeds:
									if other.className == attributed.parentName:
										attributed.parent = other
										break
							# We can skip reading the rest of this file if the parent wasn't set.
							if attributed.parent == None and not attributed.parentName == 'Attributed':
								break

						# We assume the entire nested namespace of this Attributed is on one line.
						if 'namespace' in line:
							attributed.namespace = line.split()[-1]

						if '[[Attribute' in line and not isCommentedOut(line):
							if '(' in line:
								attributed.addAttribute(line)
							else:
								attributed.addAttribute(next(file))

						prevline = line
					
					if attributed.isValid():
						attributeds.append(attributed)
						moreAttributeds = True

# Finally, generate the string we'll write to the file.
string = """#include "pch.h"
#include "Registry.h"

"""

for attributed in attributeds:
	string += '#include "' +  attributed.path.as_posix() + '"\n'

string += """
namespace Library
{
	const HashMap<RTTI::IDType, Registry::Attributes> Registry::registry =
	{
"""

for attributed in attributeds:
	string += "" + str(attributed) + ',\n'

string += """	};
}
"""

# make the directory if it doesn't already exist
os.makedirs(os.path.dirname(destFile), exist_ok=True)
with open(destFile, 'w') as file:
	file.write(string)
	file.close()
