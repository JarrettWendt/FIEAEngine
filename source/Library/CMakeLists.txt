# MIT License Copyright (c) 2020 Jarrett Wendt

cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME Library)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Add source to this project's executable.
add_library(${TARGET_NAME} STATIC ${SOURCE_FILES} "memory/WeakPtr.h"  "memory/SmartPtr.h")

# Linking External Libraries
find_package(Microsoft.GSL CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} PUBLIC Microsoft.GSL::GSL)
