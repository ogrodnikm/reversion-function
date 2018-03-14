cmake_minimum_required (VERSION 3.0)

# Add this to project as header only library
add_library(pg_list INTERFACE)
target_include_directories(pg_list INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
