add_executable(jammer src/jammer.c)

target_link_libraries(jammer pico_stdlib)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(jammer pico_cyw43_arch_none)
endif()

if (DEBUG)
    target_link_libraries(jammer
        pico_bootsel_via_double_reset
        )
endif()
