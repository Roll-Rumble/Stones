cmake_minimum_required (VERSION 3.8)

project("server")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(server src/Ball.cpp src/networking.cpp src/server.cpp)
target_include_directories(server PUBLIC ${PROJECT_SOURCE_DIR}/include)