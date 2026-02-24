file(GLOB WARDEN_CORE_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/*.c)
file(GLOB WARDEN_MP_CORES_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/cores/*.c)
file(GLOB WARDEN_UTILS_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/utils/*.c)

add_executable(warden 
            ${WARDEN_CORE_EXECS}
            ${WARDEN_MP_CORES_EXECS}
            ${WARDEN_UTILS_EXECS}
)

target_link_libraries(warden 
                    pico_stdlib
                    pico_cyw43_arch_lwip_threadsafe_background
                    hardware_pwm
                    hardware_adc
                    pico_multicore
                    pico_adxl345)
