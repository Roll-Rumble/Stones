cmake_minimum_required (VERSION 3.8)

project("menu")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB MENU_SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jtag_atlantic/lib/jtag_atlantic.dll" "${CMAKE_CURRENT_BINARY_DIR}/jtag_atlantic.dll" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jtag_atlantic/lib/jtag_client.dll" "${CMAKE_CURRENT_BINARY_DIR}/jtag_client.dll" COPYONLY)

# Add source to this project's executable.
add_executable (menu ${MENU_SRC_FILES})

#set_target_properties(menu PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(menu PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jtag_atlantic/include"
)

# Add dynamically linked library
add_library(jtag_atlantic SHARED IMPORTED)
add_library(jtag_client SHARED IMPORTED)

set_target_properties(jtag_atlantic PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jtag_atlantic/lib/jtag_atlantic.dll"
    IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jtag_atlantic/lib/jtag_atlantic.lib")

set_target_properties(jtag_client PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jtag_atlantic/lib/jtag_client.dll"
    IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jtag_atlantic/lib/jtag_client.lib")

target_link_libraries(menu jtag_atlantic)
