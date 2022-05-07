# Copyright (c) 2019-2022 Jarrett Wendt

file(GLOB_RECURSE SOURCE_FILES *.cc)
add_library(fieaEngine STATIC ${SOURCE_FILES})

set_target_properties(fieaEngine PROPERTIES
	CXX_STANDARD 			20
	CXX_STANDARD_REQUIRED 	yes
	CXX_EXTENSIONS 			no)

# target_include_directories(fieaEngine PRIVATE ../inc)
