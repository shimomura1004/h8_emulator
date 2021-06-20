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

add_executable(xor_test xor_test.cc)
target_link_libraries(xor_test
    cpu
    mcu
    dram
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME xor_test
    COMMAND xor_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(and_test and_test.cc)
target_link_libraries(and_test
    cpu
    mcu
    dram
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME and_test
    COMMAND and_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(bcc_test bcc_test.cc)
target_link_libraries(bcc_test
    cpu
    mcu
    dram
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME bcc_test
    COMMAND bcc_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(bset_test bset_test.cc)
target_link_libraries(bset_test
    cpu
    mcu
    dram
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME bset_test
    COMMAND bset_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(bclr_test bclr_test.cc)
target_link_libraries(bclr_test
    cpu
    mcu
    dram
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME bclr_test
    COMMAND bclr_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(nop_test nop_test.cc)
target_link_libraries(nop_test
    cpu
    mcu
    dram
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME nop_test
    COMMAND nop_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(sleep_test sleep_test.cc)
target_link_libraries(sleep_test
    cpu
    mcu
    dram
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME sleep_test
    COMMAND sleep_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(rts_test rts_test.cc)
target_link_libraries(rts_test
    cpu
    mcu
    dram
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME rts_test
    COMMAND rts_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(rte_test rte_test.cc)
target_link_libraries(rte_test
    cpu
    mcu
    dram
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME rte_test
    COMMAND rte_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(jmp_test jmp_test.cc)
target_link_libraries(jmp_test
    cpu
    mcu
    dram
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME jmp_test
    COMMAND jmp_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(jsr_test jsr_test.cc)
target_link_libraries(jsr_test
    cpu
    mcu
    dram
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME jsr_test
    COMMAND jsr_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(trapa_test trapa_test.cc)
target_link_libraries(trapa_test
    cpu
    mcu
    dram
    interrupt
    board
    operation_map
    instructions
    gtest_main
    gmock)
add_test(
    NAME trapa_test
    COMMAND trapa_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)
