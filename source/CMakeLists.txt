# MIT License Copyright (c) 2020 Jarrett Wendt

cmake_minimum_required(VERSION 3.10)

# where the library's source files are located
set(LIBRARY_SOURCE_DIR ${PROJECT_SOURCE_DIR}/source/Library/)

# make Library visible to all projects
include_directories(${LIBRARY_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
# TODO: Ideally all these sub-dirs would get automatically added with some kind of GLOB_RECURSE
# The only dir we want to avoid is python
include_directories(${LIBRARY_SOURCE_DIR}/containers ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${LIBRARY_SOURCE_DIR}/engine ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${LIBRARY_SOURCE_DIR}/events ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${LIBRARY_SOURCE_DIR}/hierarchy ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${LIBRARY_SOURCE_DIR}/math ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${LIBRARY_SOURCE_DIR}/memory ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${LIBRARY_SOURCE_DIR}/reflection ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${LIBRARY_SOURCE_DIR}/util ${CMAKE_CURRENT_BINARY_DIR})

# all directories which have a CMakeLists.txt
add_subdirectory(Library)
add_subdirectory(Tests)
add_subdirectory(Game)
