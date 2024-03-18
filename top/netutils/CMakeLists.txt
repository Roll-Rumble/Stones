cmake_minimum_required (VERSION 3.8)

project("netutils")

add_library(netutils STATIC netutils.cpp)


if(CMAKE_HOST_SYSTEM MATCHES Windows)
    target_compile_definitions(netutils PUBLIC "CLIENT_COMPILE")
endif()

if (CMAKE_HOST_SYSTEM MATCHES Linux)
	target_compile_definitions(netutils PUBLIC "SERVER_COMPILE")
endif()