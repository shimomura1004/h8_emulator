add_executable(cpu_test
    h83069f_test.cc
)

target_link_libraries(cpu_test
    cpu
    gtest_main
    gmock)

add_test(
    NAME cpu_test
    COMMAND cpu_test
    WORKING_DIRECTORY ${h8emu_SOURCE_DIR}/test
)
