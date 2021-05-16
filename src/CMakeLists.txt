
find_package(Threads)
include_directories(${h8emu_SOURCE_DIR}/src)

add_definitions("-Wall -I. -std=c++11 -pthread -g")
# add_definitions("-Wconversion")

add_subdirectory(cpu)
add_subdirectory(mcu)
add_subdirectory(sci)
add_subdirectory(instructions)
add_subdirectory(operation_map)
add_subdirectory(timer)
add_subdirectory(dram)
add_subdirectory(ioport)
add_subdirectory(bus)
add_subdirectory(interrupt)
add_subdirectory(net)
add_subdirectory(debugger)
add_subdirectory(board)

add_executable(h8emu
    main.cc
)

target_link_libraries(
    h8emu
    debugger
    cpu
    mcu
    sci
    operation_map
    instructions
    timer
    dram
    ioport
    bus
    interrupt
    net
    board
    ${CMAKE_THREAD_LIBS_INIT}
)

add_executable(sender
    sender.cc
)
