﻿cmake_minimum_required (VERSION 3.8)

add_executable(Test "reversion_function_test.cpp" "reversion_function_impl_test.cpp" 
                    "pg_list_utils_test.cpp" "pg_list_adapter_test.cpp")
target_link_libraries(Test reversion_function gtest_main)

add_test(NAME MyTest COMMAND Test)

install(TARGETS Test
    RUNTIME DESTINATION "test"
    ARCHIVE DESTINATION "test"
    LIBRARY DESTINATION "lib")
