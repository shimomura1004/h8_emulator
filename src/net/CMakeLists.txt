cmake_minimum_required(VERSION 3.14)
add_library(net STATIC
    rtl8019as.cc
    tap_device.cc
    rtl8019as_register.cc
)
