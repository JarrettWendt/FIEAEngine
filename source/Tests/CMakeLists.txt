# MIT License Copyright (c) 2020 Jarrett Wendt

cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME Tests)

macro(SUBDIRLIST result curdir)
	file(GLOB children RELATIVE ${curdir} ${curdir}/*)
	set(dirlist "")
	foreach(child ${children})
		if(IS_DIRECTORY ${curdir}/${child})
			list(APPEND dirlist ${child})
		endif()
	endforeach()
	set(${result} ${dirlist})
endmacro()

SUBDIRLIST(SUB_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

message("DEBUG ${CMAKE_CURRENT_SOURCE_DIR}")
message("DEBUG ${SUB_DIRS}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/")

# Add source to this project's executable.
add_executable(${TARGET_NAME} ${SOURCE_FILES})

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE Catch2::Catch2 Library)
