cmake_minimum_required (VERSION 3.8)

set(SOURCE "reversion_function.cpp")

add_library(reversion_function ${SOURCE})

target_link_libraries(reversion_function pg_list)
target_include_directories(reversion_function PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS reversion_function
    RUNTIME DESTINATION "bin"
    ARCHIVE DESTINATION "lib"
    LIBRARY DESTINATION "lib")

install(FILES "reversion_function.h" "reversion_function_impl.h" DESTINATION "include")
install(DIRECTORY "adapters" DESTINATION "include")
