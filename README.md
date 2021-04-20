h8_emulator
==================================================

## Description
h8_emulator emulates [H8/3069F board](https://akizukidenshi.com/catalog/g/gK-01271/).

## Build
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

## Usage
    $ h8300h kzload.elf

    # on another terminal
    $ sender ser1

## Demo
Start bootloader.

    $ ./h8300h kzload.elf
    Loading segment 0
    Loading segment 1
    Loading segment 2
    Loading segment 3
    Start execution from 0x100

Start serial console.

    $ ./sender ser1
    ?kzload (kozos boot loader) started.
    kzload> load
    (command) send kozos
    Sending [kozos]
    Sending blocks......................................................................................................................................done.

    XMODEM receive succeeded.
    kzload> run
    starting from entry point: 400100
    kozos boot succeed!

    command> MAC: 001122334455
    network ready.

    unknown.
    command> 

## Enable networking
    # Prepare tap device
    $ sudo ip tuntap add dev tap0 mode tap user h8

    # Assign address to the tap device
    $ sudo ip addr add 10.0.0.2/24 dev tap0

    # Link up the tap device
    $ sudo ip link set tun0 up

    # Run your binary
    $ h8300h kzload.elf