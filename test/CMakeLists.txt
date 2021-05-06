cmake_minimum_required(VERSION 3.14)

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
    instructions/mov_immediate_test.cc
    cpu/h83069f_test.cc
    # todo: これらはライブラリにして、src のファイルを直接参照するのをやめる
    ../src/h8300h_mcu.cc
    ../src/h8board.cc
    ../src/elf_loader.cc
)
target_link_libraries(example
    cpu
    dram
    ioport
    bus
    operation_map
    instructions
    gtest_main
    gmock)
add_test(NAME example_test COMMAND example)
