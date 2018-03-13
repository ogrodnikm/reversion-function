# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("reversion_function")

set(CMAKE_CXX_STANDARD 17)

# This should supress warning, but I don't like setting this globally - need to investigate further
set(CMAKE_CXX_FLAGS "-D_SCL_SECURE_NO_WARNINGS")

add_subdirectory("external")
# add_subdirectory ("prototype")
add_subdirectory("src")
add_subdirectory("unittest")
enable_testing()
