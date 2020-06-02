import os, sys
from pathlib import Path

# TODO:
# Don't assume the filename is the class name.
# Do some syntax checking with the KEYWORDS instead of just ignoring them.
# Come up with a cleaner way to encode the indentation, perhaps at the cost of double-parsing the string.
# Use a library for real C++ parsing.
# Make this work for nested types and for when the namespace declaration isn't all on one line.

# Returns whether or not the line of code is commented out.
def isCommentedOut(line):
	line = line.lstrip()
	return line.startswith('//') or line.startswith('/*')

# 
class ClassInfo:
	def __init__(self, path = ''):
		self.namespace = ''
		self.path = path;
		self.className = ''

	def __str__(self):
		return '{ "' + self.className + '", [] { return std::make_shared<' + self.scopeResolvedClass() + '>(); } }'
	
	# Get the scope resolved class. Ex: 'MyNamespace::MyClass'
	def scopeResolvedClass(self):
		return self.namespace + '::' + self.className

rootPaths = [os.path.abspath(arg) for arg in sys.argv[1:]] if len(sys.argv) > 1 else [os.path.abspath('..')]
destFile = rootPaths[0] + '\Reflection.generated.cpp'

classInfos = []

# Recursively go through all header files.
for rootPath in rootPaths:
	for extension in ['*.h', "*.hh", "*.hpp"]:
		for path in Path(rootPath).rglob(extension):
			with open(path, 'r') as file:
				classInfo = ClassInfo(path)

				for line in file:
					if 'namespace' in line :
						classInfo.namespace = line.split()[-1]
					if not isCommentedOut(line):
						if '[[Reflectable]]' in line:
							try:
								classInfo.className = next(file).split()[1]
								classInfos.append(classInfo)
							except Exception:
								pass

# Finally, generate the string we'll write to the file.
string = """#include "pch.h"
#include "Reflection.h"

"""

for classInfo in classInfos:
	string += '#include "' +  classInfo.path.as_posix() + '"\n'

string += """
namespace Library
{
	const HashMap<std::string, std::function<std::shared_ptr<void>()>> Reflection::constructors =
	{
"""

for classInfo in classInfos:
	string += '\t\t' + str(classInfo) + ',\n'

string += """	};
}
"""

with open(destFile, 'w') as file:
	file.write(string)
	file.close()
