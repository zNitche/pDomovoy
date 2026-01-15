cmake_minimum_required(VERSION 3.13...3.27)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

project(picoDoorAlarm)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

add_executable(hello_world
            hello_world.c
            )

target_link_libraries(hello_world pico_stdlib)

pico_enable_stdio_usb(hello_world 1)
pico_enable_stdio_uart(hello_world 0)

pico_add_extra_outputs(hello_world)
