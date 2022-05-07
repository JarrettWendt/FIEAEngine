# Copyright (c) 2019-2022 Jarrett Wendt

cmake_minimum_required(VERSION 3.23)

project(fieaEngine)

include_directories(
	inc
	submodules/Catch2/extras
	submodules/Catch2/src)

set(SUBMODLE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/submodules")

add_subdirectory(src)
add_subdirectory(tests)
