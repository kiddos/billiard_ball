cmake_minimum_required(VERSION 2.8)

project(billiard_ball)

set(CMAKE_REQUIRED_LIBRARIES -lallegro)

add_custom_target(
    Resources ALL
    ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res ${PROJECT_BINARY_DIR}/res SOURCES ${Resources})

add_custom_target(
    Config ALL
    ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/config ${PROJECT_BINARY_DIR}/config SOURCES ${Config})

add_subdirectory(src)
