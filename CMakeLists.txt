# MIT License Copyright (c) 2020 Jarrett Wendt

cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME FIEAEngine)

project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message("VERBOSE using python include directory \"$ENV{PYTHON_INCLUDE_DIR}\"")
message("VERBOSE using python library directory \"$ENV{PYTHON_LIBRARY_DIR}\"")
include_directories($ENV{PYTHON_INCLUDE_DIR})
link_directories($ENV{PYTHON_LIBRARY_DIR})

if (WIN32)
	set(PythonCommand py)
else()
	set(PythonCommand python3)
endif()

add_compile_options(-Wno-unknown-attributes -Wno-defaulted-function-deleted -Wno-logical-op-parentheses -Wno-ignored-qualifiers -Wno-ignored-attributes)

# all directories which have a CMakeLists.txt
add_subdirectory(source)
