add_executable(data_transfer_test
    mov_register_direct_test.cc
    mov_immediate_test.cc
    mov_absolute_address_test.cc
    mov_register_indirect_test.cc
    mov_register_indirect_with_displacement_test.cc
    mov_register_indirect_with_displacement_prepost_test.cc
)
target_link_libraries(data_transfer_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME data_transfer_test
    COMMAND data_transfer_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(arithmetic_ops_test mulxs_test.cc divxs_test.cc adds_test.cc cmp_immediate_test.cc cmp_register_direct_test.cc sub_test.cc subx_test.cc)
target_link_libraries(arithmetic_ops_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME arithmetic_ops_test
    COMMAND arithmetic_ops_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(logical_ops_test and_test.cc or_immediate_test.cc xor_test.cc not_test.cc)
target_link_libraries(logical_ops_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME logical_ops_test
    COMMAND logical_ops_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

# shift ops
add_executable(shift_ops_test shll_test.cc)
target_link_libraries(shift_ops_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME shift_ops_test
    COMMAND shift_ops_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(bit_manipulation_test bset_test.cc bclr_test.cc btst_test.cc bld_test.cc)
target_link_libraries(bit_manipulation_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME bit_manipulation_test
    COMMAND bit_manipulation_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(branching_test bcc_test.cc jmp_test.cc jsr_test.cc rts_test.cc)
target_link_libraries(branching_test cpu mcu dram board operation_map instructions gtest_main gmock)
add_test(
    NAME branching_test
    COMMAND branching_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)

add_executable(system_control_test trapa_test.cc rte_test.cc sleep_test.cc andc_test.cc orc_test.cc nop_test.cc)
target_link_libraries(system_control_test cpu mcu dram interrupt board operation_map instructions gtest_main gmock)
add_test(
    NAME system_control_test
    COMMAND system_control_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)
