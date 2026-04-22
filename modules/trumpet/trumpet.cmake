file(GLOB TRUMPET_CORE_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/*.c)
file(GLOB TRUMPET_MP_CORES_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/cores/*.c)
file(GLOB TRUMPET_PAGE_HANDLERS_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/page_handlers/*.c)

add_executable(trumpet 
            ${TRUMPET_CORE_EXECS}
            ${TRUMPET_MP_CORES_EXECS}
            ${TRUMPET_PAGE_HANDLERS_EXECS}
)

target_link_libraries(trumpet 
                    pico_stdlib
                    pdomovoy_common
                    pico_cyw43_arch_lwip_threadsafe_background
                    hardware_pwm
                    hardware_adc
                    pico_multicore
                    pico_ssd1306
                    pico_aht20)
