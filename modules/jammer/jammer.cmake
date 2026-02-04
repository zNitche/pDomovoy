add_executable(jammer
            ${CMAKE_CURRENT_LIST_DIR}/src/main.c
            ${CMAKE_CURRENT_LIST_DIR}/src/callbacks.c
            ${CMAKE_CURRENT_LIST_DIR}/src/utils/button.c
)

target_link_libraries(jammer 
                    pico_stdlib
                    pico_cyw43_arch_none
                    hardware_pwm
                    pico_adxl345)

if (DEBUG)
    target_link_libraries(jammer
        pico_bootsel_via_double_reset
        )
endif()
