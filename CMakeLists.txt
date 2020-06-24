# MIT License Copyright (c) 2020 Jarrett Wendt

cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME FIEAEngine)

project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message("VERBOSE using python directory \"$ENV{PythonDir}\"")
include_directories($ENV{PythonDir}/include)
link_directories($ENV{PythonDir}/libs)

add_compile_options(-Wno-unknown-attributes -Wno-defaulted-function-deleted -Wno-logical-op-parentheses -Wno-ignored-qualifiers -Wno-ignored-attributes)

# all directories which have a CMakeLists.txt
add_subdirectory(source)
