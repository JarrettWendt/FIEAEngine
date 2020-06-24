# MIT License Copyright (c) 2020 Jarrett Wendt

cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME FIEAEngine)

project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_TOOLCHAIN_FILE "mnt/c/Dev/vcpkg/scripts/buildsystems/vcpkg.cmake")

# all directories which have a CMakeLists.txt
add_subdirectory(source)
