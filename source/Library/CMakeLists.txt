# MIT License Copyright (c) 2020 Jarrett Wendt

cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME Library)

file(GLOB SOURCE_FILES "${PROJECT_SOURCE_DIR}/source/${TARGET_NAME}/*.cpp")

# Add source to this project's executable.
add_library(${TARGET_NAME} STATIC ${SOURCE_FILES})
