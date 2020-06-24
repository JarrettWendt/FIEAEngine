# MIT License Copyright (c) 2020 Jarrett Wendt

cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME Tests)

# Add source to this project's executable.
add_executable(${TARGET_NAME} "main.cpp" "UtilTests.cpp" "pch.h" "pch.cpp" "TestUtils.h" "TestUtils.cpp" "MemLeakUtils.h")

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE Catch2::Catch2 Library)
