cmake_minimum_required(VERSION 3.14)

find_package(Threads)
include_directories(${h8300h_SOURCE_DIR}/src)

add_definitions("-Wall -I. -std=c++11 -pthread -g")
# add_definitions("-Wconversion")

add_subdirectory(cpu)
add_subdirectory(sci)
add_subdirectory(instructions)
add_subdirectory(operation_map)
add_subdirectory(timer)
add_subdirectory(dram)
add_subdirectory(ioport)
add_subdirectory(bus)
add_subdirectory(interrupt)
add_subdirectory(net)

add_executable(h8emu
    elf_loader.cc
    mcu.cc
    h8board.cc
    runner.cc
    main.cc
)

target_link_libraries(
    h8emu
    cpu
    sci
    operation_map
    instructions
    timer
    dram
    ioport
    bus
    interrupt
    net
    ${CMAKE_THREAD_LIBS_INIT}
)

add_executable(sender
    sender.cc
)
