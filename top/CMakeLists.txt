cmake_minimum_required (VERSION 3.8)

project("top")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_subdirectory(${PROJECT_SOURCE_DIR}/netutils)


if(CMAKE_HOST_SYSTEM MATCHES Windows)
	file(GLOB CLIENT_SRC_FILES "${PROJECT_SOURCE_DIR}/client/src/*.cpp")
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jtag_atlantic/lib/jtag_atlantic.dll" "${CMAKE_CURRENT_BINARY_DIR}/jtag_atlantic.dll" COPYONLY)
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jtag_atlantic/lib/jtag_client.dll" "${CMAKE_CURRENT_BINARY_DIR}/jtag_client.dll" COPYONLY)

	# Add source to this project's executable.
	add_executable (client ${CLIENT_SRC_FILES})


	target_include_directories(client PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLEW/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jtag_atlantic/include"
		"${PROJECT_SOURCE_DIR}/netutils"
		"${PROJECT_SOURCE_DIR}/client/include")

	# Add dynamically linked library
	add_library(jtag_atlantic SHARED IMPORTED)
	add_library(jtag_client SHARED IMPORTED)
	add_library(GLFW STATIC IMPORTED)
	add_library(GLEW STATIC IMPORTED)
	add_library(OpenGL STATIC IMPORTED)
	add_library(WinSock STATIC IMPORTED)

	target_compile_definitions(GLEW INTERFACE "GLEW_STATIC")

	set_target_properties(jtag_atlantic PROPERTIES
		IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jtag_atlantic/lib/jtag_atlantic.dll"
		IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jtag_atlantic/lib/jtag_atlantic.lib")

	set_target_properties(jtag_client PROPERTIES
		IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jtag_atlantic/lib/jtag_client.dll"
		IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jtag_atlantic/lib/jtag_client.lib")

	set_target_properties(GLFW PROPERTIES IMPORTED_LOCATION
	"${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/lib-vc2022/glfw3.lib")

	set_target_properties(GLEW PROPERTIES IMPORTED_LOCATION
	"${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLEW/lib/Release/x64/glew32s.lib")

	set_target_properties(OpenGL PROPERTIES IMPORTED_LOCATION
	"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/OpenGL32.Lib")

	set_target_properties(WinSock PROPERTIES IMPORTED_LOCATION
	"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/WS2_32.Lib")

	target_link_libraries(client netutils jtag_atlantic jtag_client GLFW GLEW OpenGL WinSock)
endif()

if (CMAKE_HOST_SYSTEM MATCHES Linux)
	file(GLOB SERV_SRC_FILES "${PROJECT_SOURCE_DIR}/server/src/*.cpp")
  
	add_executable(server ${SERV_SRC_FILES})

	target_include_directories(server PUBLIC
	"${PROJECT_SOURCE_DIR}/netutils" "${PROJECT_SOURCE_DIR}/server/include"
	)

	target_link_libraries(server netutils)
endif()