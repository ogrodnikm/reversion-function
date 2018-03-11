cmake_minimum_required (VERSION 3.8)

set(SOURCE "reversion_function.cpp" "reversion_function.h" "pg_list_utils.h")

add_library(reversion_function ${SOURCE})

target_link_libraries(reversion_function pg_list)
target_include_directories(reversion_function PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
