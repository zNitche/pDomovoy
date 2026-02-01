cmake_minimum_required(VERSION 3.27)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(picoDoorAlarm)

pico_sdk_init()

include(cmake/jammer.cmake)
include(cmake/station.cmake)

# disable uart stdio & usb stdio depending on -DPROD=1/0
pico_enable_stdio_uart(jammer 0)
pico_enable_stdio_uart(station 0)

if (PROD)
    pico_enable_stdio_usb(station 0)
    pico_enable_stdio_usb(jammer 0)
else()
    pico_enable_stdio_usb(station 1)
    pico_enable_stdio_usb(jammer 1)
endif()

# generate output files
pico_add_extra_outputs(jammer)
pico_add_extra_outputs(station)
