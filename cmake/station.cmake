add_executable(station src/station/main.c)

target_link_libraries(station pico_stdlib)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(station pico_cyw43_arch_none)
endif()

if (DEBUG)
    target_link_libraries(station
        pico_bootsel_via_double_reset
        )
endif()
