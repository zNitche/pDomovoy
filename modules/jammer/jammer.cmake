file(GLOB JAMMER_CORE_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/*.c)
file(GLOB JAMMER_MP_CORES_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/cores/*.c)
file(GLOB JAMMER_UTILS_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/utils/*.c)

add_executable(jammer 
            ${JAMMER_CORE_EXECS}
            ${JAMMER_MP_CORES_EXECS}
            ${JAMMER_UTILS_EXECS}
)

target_link_libraries(jammer 
                    pico_stdlib
                    pico_cyw43_arch_lwip_threadsafe_background
                    hardware_pwm
                    hardware_adc
                    pico_multicore
                    pico_adxl345)
