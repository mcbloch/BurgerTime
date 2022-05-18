cmake_minimum_required(VERSION 3.19)
project(BurgerTime)

add_definitions("-std=c++20")

# Add conan. It is initialized by running 'conan install ..' from the build folder
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# TODO Don't build on W4 warnings
# Add targets and ci for windows x86, x64 and unix x64

add_compile_options(
		# -Wall
		# -Wextra
		#		-pedantic
		#		-Wconversion
		#		-Wsign-conversion
		#		-O3 -march=native -mtune=native -flto -static
		#		$<$<CONFIG:DEBUG>:-g3>
		#		$<$<CONFIG:DEBUG>:-Og>
		#		$<$<CONFIG:RELEASE>:-O3>
)
#
#add_compile_definitions(
#		$<$<CONFIG:DEBUG>:DEBUG>
#)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)
#include_directories(inc)
include_directories(src)
#include_directories(src/template)

file( GLOB SRCS Minigin/*.cpp Minigin/*.h )

# Main executable for own use
add_executable(burgertime ${SRCS})

target_link_libraries(burgertime ${CONAN_LIBS})

message(CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR})
message(CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR})

#add_custom_command(
#		TARGET burgertime POST_BUILD
#		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/res)
#add_custom_command(
#		TARGET aoc2021_cpp POST_BUILD
#		COMMAND ${CMAKE_COMMAND} -E copy
#		${CMAKE_SOURCE_DIR}/res/*
#		${CMAKE_CURRENT_BINARY_DIR}/res/)

