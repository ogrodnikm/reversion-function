cmake_minimum_required (VERSION 3.8)

set(SOURCE "reversion_function_prototype.cpp")

add_executable(reversion_function_prototype ${SOURCE})

target_link_libraries(reversion_function_prototype reversion_function pg_list)

install(TARGETS reversion_function_prototype
    RUNTIME DESTINATION "bin"
    ARCHIVE DESTINATION "bin"
    LIBRARY DESTINATION "lib")
