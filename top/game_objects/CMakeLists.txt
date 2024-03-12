cmake_minimum_required (VERSION 3.8)

project("top")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(game_objects ${SRC_FILES})

if(CMAKE_HOST_SYSTEM MATCHES Windows)
	target_compile_definitions(game_objects PUBLIC "CLIENT_COMPILE")

	target_include_directories(game_objects PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLEW/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/include")

	# Add dynamically linked library
	add_library(GLFW STATIC IMPORTED)
	add_library(GLEW STATIC IMPORTED)
	add_library(OpenGL STATIC IMPORTED)

	target_compile_definitions(GLEW INTERFACE "GLEW_STATIC")


	set_target_properties(GLFW PROPERTIES IMPORTED_LOCATION
	"${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/lib-vc2022/glfw3.lib")

	set_target_properties(GLEW PROPERTIES IMPORTED_LOCATION
	"${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLEW/lib/Release/x64/glew32s.lib")

	set_target_properties(OpenGL PROPERTIES IMPORTED_LOCATION
	"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/OpenGL32.Lib")

	target_link_libraries(game_objects GLFW GLEW OpenGL)
endif()

if (CMAKE_HOST_SYSTEM MATCHES Linux)
	target_compile_definitions(game_objects PUBLIC "SERVER_COMPILE")
	target_include_directories(game_objects PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/include")
endif()