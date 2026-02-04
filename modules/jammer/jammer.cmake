file(GLOB JAMMER_CORE_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/*.c)
file(GLOB JAMMER_UTILS_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/utils/*.c)

add_executable(jammer 
            ${JAMMER_CORE_EXECS}
            ${JAMMER_UTILS_EXECS}
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
