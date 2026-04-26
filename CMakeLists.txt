cmake_minimum_required(VERSION 3.27)

set(PICO_BOARD "pico_w")

if (DEBUG)
    set(CMAKE_BUILD_TYPE "Debug")
else()
    set(CMAKE_BUILD_TYPE "Release")
endif()

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pDomovoy VERSION 0.5.0)

pico_sdk_init()

include(config.cmake)

# add libs
add_subdirectory(libs/pico-c-adxl345/lib)
add_subdirectory(libs/pico-c-ssd1306/lib)
add_subdirectory(libs/pico-c-aht20/lib)

# add commons
add_subdirectory(modules/common)

# add modules
include(modules/warden/warden.cmake)
include(modules/trumpet/trumpet.cmake)

target_include_directories(warden PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/modules/common/includes
        ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_include_directories(trumpet PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/modules/common/includes
        ${CMAKE_CURRENT_BINARY_DIR}/generated
)

# disable uart stdio & usb stdio depending on -DPROD=1/0
pico_enable_stdio_uart(warden 0)
pico_enable_stdio_uart(trumpet 0)

target_compile_definitions(warden PUBLIC DEBUG="${DEBUG}")
target_compile_definitions(trumpet PUBLIC DEBUG="${DEBUG}")

string(REPLACE ";" "," PD_CLIENT_BT_MAC_STRING "${PD_CLIENT_BT_MAC}")
string(REPLACE ";" "," PD_SERVER_BT_MAC_STRING "${PD_SERVER_BT_MAC}")
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/configuration_files/pd_common_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/pd_common_config.h
)

if (DEBUG)
    message("[pD] configuring for Debug...")

    pico_enable_stdio_usb(trumpet 1)
    pico_enable_stdio_usb(warden 1)

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
