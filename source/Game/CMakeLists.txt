# MIT License Copyright (c) 2020 Jarrett Wendt

cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME Game)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/")

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Add source to this project's executable.
add_executable(${TARGET_NAME} ${SOURCE_FILES} ../Library/main.cpp)

target_link_libraries(${TARGET_NAME} Library)

find_package(Microsoft.GSL CONFIG REQUIRED)
target_link_libraries(Microsoft.GSL::GSL)

add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD COMMAND ${PythonCommand} ${LIBRARY_SOURCE_DIR}/reflection/GenerateAttributes.py ${CMAKE_CURRENT_SOURCE_DIR} ${LIBRARY_SOURCE_DIR})
add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD COMMAND ${PythonCommand} ${LIBRARY_SOURCE_DIR}/reflection/Reflection.py ${CMAKE_CURRENT_SOURCE_DIR} ${LIBRARY_SOURCE_DIR})
