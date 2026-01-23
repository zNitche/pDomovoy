add_executable(client src/client.c)

target_link_libraries(client pico_stdlib)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(client pico_cyw43_arch_none)
endif()
