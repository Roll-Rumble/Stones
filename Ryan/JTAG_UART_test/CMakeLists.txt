# CMakeList.txt : CMake project for JTAG_UART_test, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("JTAG_UART_test")

# Add source to this project's executable.
add_executable (JTAG_UART_test "JTAG_UART_test.cpp" "jtag_atlantic.h" "controller.cpp")

set(this_DIR "C:\\dev\\JTAG_UART_test")


link_directories( ${this_DIR} ) #add library path
include_directories( ${this_DIR} ) #include headers
target_link_libraries( ${PROJECT_NAME} ${this_DIR} )

# Add dynamically linked library
add_library(jtag_atlantic STATIC IMPORTED GLOBAL)
add_library(jtag_client STATIC IMPORTED GLOBAL)
set_target_properties(jtag_atlantic PROPERTIES IMPORTED_LOCATION "C:\\dev\\JTAG_UART_test\\jtag_atlantic.lib")
set_target_properties(jtag_client PROPERTIES IMPORTED_LOCATION "C:\\dev\\JTAG_UART_test\\jtag_client.lib")
target_link_libraries(JTAG_UART_test jtag_atlantic)
target_link_libraries(JTAG_UART_test jtag_client)



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET JTAG_UART_test PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
