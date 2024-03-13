cmake_minimum_required (VERSION 3.12)

project("imgui")

file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(imgui ${SRC_FILES})
target_include_directories(imgui "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")