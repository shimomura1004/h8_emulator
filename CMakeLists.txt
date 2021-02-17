cmake_minimum_required(VERSION 3.1)

project(h8300h CXX)
include_directories(${h8300h_SOURCE_DIR})

add_definitions("-Wall -I. -std=c++11 -pthread -g")

add_subdirectory(sci)
add_subdirectory(instructions)
add_subdirectory(operation_map)
add_subdirectory(timer)
add_subdirectory(dram)
add_subdirectory(ioport)
add_subdirectory(interrupt)

add_executable(h8300h
    elf_loader.cc
    mcu.cc
    h8300h.cc
    runner.cc
    main.cc
)

add_executable(sender
    sender.cc
)

target_link_libraries(h8300h sci instructions operation_map timer dram ioport interrupt)
