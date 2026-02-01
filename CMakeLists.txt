cmake_minimum_required(VERSION 3.27)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_door_alarm VERSION 0.0.1)

pico_sdk_init()

# add libs
add_subdirectory(libs/pico-c-adxl345/lib)

# add sub cmakes
include(cmake/jammer.cmake)
include(cmake/station.cmake)

# disable uart stdio & usb stdio depending on -DPROD=1/0
pico_enable_stdio_uart(jammer 0)
pico_enable_stdio_uart(station 0)

if (DEBUG)
    pico_enable_stdio_usb(station 1)
    pico_enable_stdio_usb(jammer 1)
    
    target_compile_definitions(jammer PUBLIC DEBUG=1)
    target_compile_definitions(station PUBLIC DEBUG=1)
else()
    pico_enable_stdio_usb(station 0)
    pico_enable_stdio_usb(jammer 0)
endif()

# generate output files
pico_add_extra_outputs(jammer)
pico_add_extra_outputs(station)
