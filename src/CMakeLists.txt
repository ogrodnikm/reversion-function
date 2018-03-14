cmake_minimum_required (VERSION 3.0)

set(SOURCE "reversion_function.cpp")

add_library(reversion_function ${SOURCE})

target_link_libraries(reversion_function pg_list)
target_include_directories(reversion_function PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
