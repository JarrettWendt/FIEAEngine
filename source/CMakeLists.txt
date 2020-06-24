# MIT License Copyright (c) 2020 Jarrett Wendt

cmake_minimum_required(VERSION 3.10)

# where the library's source files are located
set(LIBRARY_SOURCE_DIR ${PROJECT_SOURCE_DIR}/source/Library/)

# make Library visible to all projects
include_directories(${LIBRARY_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# all directories which have a CMakeLists.txt
add_subdirectory(Library)
add_subdirectory(Tests)
add_subdirectory(Game)
