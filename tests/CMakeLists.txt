# Copyright (c) 2019-2022 Jarrett Wendt

file(GLOB_RECURSE SOURCE_FILES *.cc)

add_executable(fieaEngineUnitTests
	${SOURCE_FILES}
	# I had trouble linking to Catch2 as a static library, so instead we're going with the amalgamated files
	../submodules/Catch2/extras/catch_amalgamated.cpp)

set_target_properties(fieaEngineUnitTests PROPERTIES
	CXX_STANDARD 			20
	CXX_STANDARD_REQUIRED 	yes
	CXX_EXTENSIONS 			no)

target_link_libraries(fieaEngineUnitTests PRIVATE fieaEngine)
