add_executable(mov_test
    mov_register_direct_test.cc
    mov_immediate_test.cc
    mov_absolute_address_test.cc
    mov_register_indirect_test.cc
    mov_register_indirect_with_displacement_test.cc
    mov_register_indirect_with_displacement_prepost_test.cc
)
target_link_libraries(mov_test
    cpu
    mcu
    dram
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME mov_test
    COMMAND mov_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(or_test or_immediate_test.cc)
target_link_libraries(or_test
    cpu
    mcu
    dram
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME or_test
    COMMAND or_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(cmp_test
    cmp_immediate_test.cc
    cmp_register_direct_test.cc
)
target_link_libraries(cmp_test
    cpu
    mcu
    dram
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME cmp_test
    COMMAND cmp_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)
