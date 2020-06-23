# MIT License Copyright (c) 2020 Jarrett Wendt

cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME Game)

# Add source to this project's executable.
add_executable(${TARGET_NAME} "main.cpp")

target_link_libraries(${TARGET_NAME} Library)
