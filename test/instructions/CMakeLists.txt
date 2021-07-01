add_executable(mov_test
    mov_register_direct_test.cc
    mov_immediate_test.cc
    mov_absolute_address_test.cc
    mov_register_indirect_test.cc
    mov_register_indirect_with_displacement_test.cc
    mov_register_indirect_with_displacement_prepost_test.cc
)
target_link_libraries(mov_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME mov_test
    COMMAND mov_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(logical_instructions_test and_test.cc or_immediate_test.cc xor_test.cc)
target_link_libraries(logical_instructions_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME logical_instructions_test
    COMMAND logical_instructions_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(cmp_test cmp_immediate_test.cc cmp_register_direct_test.cc)
target_link_libraries(cmp_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME cmp_test
    COMMAND cmp_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(bcc_test bcc_test.cc)
target_link_libraries(bcc_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME bcc_test
    COMMAND bcc_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(bset_test bset_test.cc)
target_link_libraries(bset_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME bset_test
    COMMAND bset_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(bclr_test bclr_test.cc)
target_link_libraries(bclr_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME bclr_test
    COMMAND bclr_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(nop_test nop_test.cc)
target_link_libraries(nop_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME nop_test
    COMMAND nop_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(sleep_test sleep_test.cc)
target_link_libraries(sleep_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME sleep_test
    COMMAND sleep_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(jmp_test jmp_test.cc jsr_test.cc trapa_test.cc rts_test.cc rte_test.cc)
target_link_libraries(jmp_test cpu mcu dram interrupt board operation_map instructions gtest_main gmock)
add_test(
    NAME jmp_test
    COMMAND jmp_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(arithmetic_test mulxs_test.cc divxs_test.cc)
target_link_libraries(arithmetic_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME arithmetic_test
    COMMAND arithmetic_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(adds_test adds_test.cc)
target_link_libraries(adds_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME adds_test
    COMMAND adds_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(ccr_test andc_test.cc orc_test.cc)
target_link_libraries(ccr_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME ccr_test
    COMMAND ccr_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)
