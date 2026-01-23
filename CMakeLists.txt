cmake_minimum_required(VERSION 3.27)

# initialize the SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(picoDoorAlarm)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# server & client cmake setup
include(cmake/server.cmake)
include(cmake/client.cmake)

# disable uart stdio & usb stdio depending on -DPROD=1/0
pico_enable_stdio_uart(server 0)
pico_enable_stdio_uart(client 0)

if (PROD)
    pico_enable_stdio_usb(client 0)
    pico_enable_stdio_usb(server 0)
else()
    pico_enable_stdio_usb(client 1)
    pico_enable_stdio_usb(server 1)
endif()

# generate output files
pico_add_extra_outputs(server)
pico_add_extra_outputs(client)
