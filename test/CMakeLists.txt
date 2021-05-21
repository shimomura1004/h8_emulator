include_directories(
    ${h8emu_SOURCE_DIR}/src
    ${h8emu_SOURCE_DIR}/test
    ${GTEST_INCLUDE_PATH}
    ${GMOCK_INCLUDE_PATH}
)

add_definitions("-Wall -I. -std=c++11 -pthread -g")
# add_definitions("-Wconversion")

add_subdirectory(cpu)
add_subdirectory(instructions)

# test cases in static library IS NOT linked because there is no references from main object
