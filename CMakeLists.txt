cmake_minimum_required(VERSION 3.27)

set(PICO_BOARD "pico_w")

if (DEBUG)
    set(CMAKE_BUILD_TYPE "Debug")
else()
    set(CMAKE_BUILD_TYPE "Release")
endif()

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pDomovoy VERSION 0.0.1)

pico_sdk_init()

# add libs
add_subdirectory(libs/pico-c-adxl345/lib)

# add modules
include(modules/warden/warden.cmake)
include(modules/trumpet/trumpet.cmake)

target_include_directories(warden PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/modules/common/includes
)

target_include_directories(trumpet PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/modules/common/includes
)

# disable uart stdio & usb stdio depending on -DPROD=1/0
pico_enable_stdio_uart(warden 0)
pico_enable_stdio_uart(trumpet 0)

if (DEBUG)
    message("[pD] configuring for Debug...")

    pico_enable_stdio_usb(trumpet 1)
    pico_enable_stdio_usb(warden 1)
    
    target_compile_definitions(warden PUBLIC DEBUG=1)
    target_compile_definitions(trumpet PUBLIC DEBUG=1)

    target_link_libraries(warden
        pico_bootsel_via_double_reset
    )

    target_link_libraries(trumpet
        pico_bootsel_via_double_reset
    )
else()
    message("[pD] configuring for Release...")

    pico_enable_stdio_usb(trumpet 0)
    pico_enable_stdio_usb(warden 0)
endif()

# generate output files
pico_add_extra_outputs(warden)
pico_add_extra_outputs(trumpet)
