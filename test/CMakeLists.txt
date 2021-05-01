cmake_minimum_required(VERSION 3.14)

include_directories(
    ${h8300h_SOURCE_DIR}/src
    ${GTEST_INCLUDE_PATH}
    ${GMOCK_INCLUDE_PATH}
)

add_definitions("-Wall -I. -std=c++11 -pthread -g")
# add_definitions("-Wconversion")

# test cases in static library IS NOT linked because there is no references from main object
add_executable(example
    example.cc
    instructions/mov_immediate_test.cc
)
target_link_libraries(example
    gtest_main
    gmock)
add_test(NAME example_test COMMAND example)
