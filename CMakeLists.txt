cmake_minimum_required(VERSION 4.0)

project(game)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(vendor/raylib)

set(SRCS
	src/Main.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} raylib)
