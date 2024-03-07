cmake_minimum_required (VERSION 3.8)

project("top")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_subdirectory(${PROJECT_SOURCE_DIR}/netutils)

function(add_instance instance_name)  
  file(GLOB SERV_SRC_FILES "${PROJECT_SOURCE_DIR}/${instance_name}/src/*.cpp")
  
  add_executable(${instance_name} ${SERV_SRC_FILES})

  target_include_directories(${instance_name} PUBLIC
    "${PROJECT_SOURCE_DIR}/netutils" "${PROJECT_SOURCE_DIR}/${instance_name}/include"
  )

  target_link_libraries(${instance_name} netutils)
endfunction()

if(CMAKE_HOST_SYSTEM MATCHES Windows)
  add_instance(client)
endif()

if (CMAKE_HOST_SYSTEM MATCHES Linux)
  add_instance(server)
endif()