cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("fpga_move")


# Add source to this project's executable.
add_executable (fpga_move "src/main.cpp" "src/controller.cpp")


target_include_directories(fpga_move PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLEW/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/include" "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/jtag_atlantic/include")

# Add dynamically linked library
add_library(jtag_atlantic SHARED IMPORTED)
add_library(jtag_client SHARED IMPORTED)
add_library(GLFW STATIC IMPORTED)
add_library(GLEW STATIC IMPORTED)
add_library(OpenGL STATIC IMPORTED)


target_compile_definitions(GLEW INTERFACE "GLEW_STATIC")

set_target_properties(jtag_atlantic PROPERTIES
	IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/jtag_atlantic/lib/jtag_atlantic.dll"
	IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/jtag_atlantic/lib/jtag_atlantic.lib")

set_target_properties(jtag_client PROPERTIES
	IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/jtag_atlantic/lib/jtag_client.dll"
	IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/jtag_atlantic/lib/jtag_client.lib")

set_target_properties(GLFW PROPERTIES IMPORTED_LOCATION
"${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/glfw3.lib")

set_target_properties(GLEW PROPERTIES IMPORTED_LOCATION
"${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLEW/lib/Release/x64/glew32s.lib")

set_target_properties(OpenGL PROPERTIES IMPORTED_LOCATION
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/OpenGL32.Lib")

target_link_libraries(fpga_move jtag_atlantic jtag_client GLFW GLEW OpenGL)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET fpga_move PROPERTY CXX_STANDARD 20)
endif()
