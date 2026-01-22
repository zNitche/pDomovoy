add_executable(client src/client.c)

target_link_libraries(client pico_stdlib)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(client pico_cyw43_arch_none)
endif()

pico_enable_stdio_usb(client 1)
pico_enable_stdio_uart(client 0)

pico_add_extra_outputs(client)
