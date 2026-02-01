add_executable(station src/station.c)

target_link_libraries(station pico_stdlib)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(station pico_cyw43_arch_none)
endif()

if (DEBUG)
    target_compile_definitions(example PUBLIC DEBUG=1)

    target_link_libraries(example
        pico_bootsel_via_double_reset
        )
endif()
