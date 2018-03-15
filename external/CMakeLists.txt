cmake_minimum_required (VERSION 3.8)

# Add this to project as header only library
add_library(pg_list INTERFACE)
target_include_directories(pg_list INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS pg_list
    RUNTIME DESTINATION "bin"
    ARCHIVE DESTINATION "lib"
    LIBRARY DESTINATION "lib")

install(FILES "node_tags.h" "nodes.h" "pg_list.h" "pg_list_utils.h" DESTINATION "include")
