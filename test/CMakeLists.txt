
include_directories(
    ${h8emu_SOURCE_DIR}/src
    ${h8emu_SOURCE_DIR}/test
    ${GTEST_INCLUDE_PATH}
    ${GMOCK_INCLUDE_PATH}
)

add_definitions("-Wall -I. -std=c++11 -pthread -g")
# add_definitions("-Wconversion")

# test cases in static library IS NOT linked because there is no references from main object
# todo: リンク時間を短くするためにライブラリを分けたほうがいい
add_executable(example
    example.cc
    instructions/mov_register_direct_test.cc
    instructions/mov_immediate_test.cc
    instructions/mov_absolute_address_test.cc
    instructions/mov_register_indirect_test.cc
    instructions/mov_register_indirect_with_displacement_test.cc
    instructions/mov_register_indirect_with_displacement_prepost_test.cc
    instructions/or_immediate_test.cc
    cpu/h83069f_test.cc
)

target_link_libraries(example
    cpu
    mcu
    dram
    ioport
    board
    bus
    operation_map
    instructions
    gtest_main
    gmock)

add_test(
    NAME example_test
    COMMAND example
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)
