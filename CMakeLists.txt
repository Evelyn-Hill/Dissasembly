cmake_minimum_required(VERSION 4.0)

# == OVERVIEW OF FILE == Eve Plum 11/24/25
# To facilitate easy unit testing for game logic the source is split into two executables and a library.
# The "game" executable only contains the main file.
# The "game_lib" library contains everything to actually startup and run the game along with the game logic.
# The "test" executable links agains the game library to facilitate easy testing.
#
# == Build Outcome ==
# Two executables, game and test. Game runs the game, test runs the unit tests.
# One library that contains all the game source code.
#
# == FUTURE? ==
# This could make it very easy to build hot reloading because we're building the game
# code as a library already, not doing that shit rn tho! 

project(DissasemblyLine)

# == COMPILER FLAGS ==
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# == SUBMODULES == 
add_subdirectory(vendor/raylib)
add_subdirectory(vendor/gtest)

# == INCLUDES == 
include_directories(include/)

# == SRCS == (ADD GAME SOURCE FILES HERE)
set(SRCS
	src/Game.cpp
    src/AssetManager.cpp
)

# == MAIN == 
add_executable(game ${PROJECT_SOURCE_DIR}/src/Main.cpp)

# == GAME LIB ==
add_library(game_lib ${SRCS})

# Show all warnings, treat warnings as errors.
target_compile_options(game_lib PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

# == LINK RAYLIB AND GAME == 
target_link_libraries(game_lib raylib)

# == LINK MAIN AND GAME ==
target_link_libraries(game game_lib)

# == TESTING ==
enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# == WINDOWS SPECIFIC CACHING == 
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# = TEST SRCS == (ADD TEST SOURCE FILES HERE)
set(TEST_SRCS
	test/Generic.cpp
)

add_executable(tests ${TEST_SRCS})

# == LINK GTEST AND GAME LIB ==
target_link_libraries(tests gtest gtest_main)
target_link_libraries(tests game_lib)

include(GoogleTest)
gtest_discover_tests(tests)

add_test(UnitTests tests)
