cmake_minimum_required (VERSION 3.8)

project("top")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_subdirectory(${PROJECT_SOURCE_DIR}/netutils)

function(add_instance)  
  file(GLOB SERV_SRC_FILES "${PROJECT_SOURCE_DIR}/${ARGV0}/src/*.cpp")
  
  add_executable(${ARGV0} ${SERV_SRC_FILES})

  target_include_directories(${ARGV0} PUBLIC
    "${PROJECT_SOURCE_DIR}/netutils" "${PROJECT_SOURCE_DIR}/${ARGV0}/include"
  )

  target_link_libraries(${ARGV0} netutils)
endfunction()



add_instance(server)