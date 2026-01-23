add_executable(server src/server.c)

target_link_libraries(server pico_stdlib)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(server pico_cyw43_arch_none)
endif()
