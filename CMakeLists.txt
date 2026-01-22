cmake_minimum_required(VERSION 3.27)

# initialize the SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(picoDoorAlarm)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

include(cmake/server.cmake)
include(cmake/client.cmake)
