cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("imgui_test")

file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/color_shader/color_shader_vs.txt" "${CMAKE_CURRENT_BINARY_DIR}/color_shader_vs.txt" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/color_shader/color_shader_fs.txt" "${CMAKE_CURRENT_BINARY_DIR}/color_shader_fs.txt" COPYONLY)
 

# Add source to this project's executable.
add_executable (imgui_test ${SRC_FILES})


target_include_directories(shader_test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLEW/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/include")

# Add dynamically linked library
add_library(GLFW STATIC IMPORTED)
add_library(GLEW STATIC IMPORTED)
add_library(OpenGL STATIC IMPORTED)


target_compile_definitions(GLEW INTERFACE "GLEW_STATIC")


set_target_properties(GLFW PROPERTIES IMPORTED_LOCATION
"${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/glfw3.lib")

set_target_properties(GLEW PROPERTIES IMPORTED_LOCATION
"${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLEW/lib/Release/x64/glew32s.lib")

set_target_properties(OpenGL PROPERTIES IMPORTED_LOCATION
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/OpenGL32.Lib")

target_link_libraries(shader_test GLFW GLEW OpenGL)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET shader_test PROPERTY CXX_STANDARD 20)
endif()
