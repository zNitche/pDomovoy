file(GLOB WARDEN_CORE_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/*.c)
file(GLOB WARDEN_MP_CORES_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/cores/*.c)
file(GLOB WARDEN_UTILS_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/utils/*.c)

add_executable(warden 
            ${WARDEN_CORE_EXECS}
            ${WARDEN_MP_CORES_EXECS}
            ${WARDEN_UTILS_EXECS}
)

target_compile_definitions(warden PRIVATE
    RUNNING_AS_CLIENT=1
    PICO_FLASH_ASSUME_CORE0_SAFE=1
    PICO_FLASH_ASSUME_CORE1_SAFE=1
)

target_link_libraries(warden 
                    pico_stdlib
                    pdomovoy_common
                    pico_multicore
                    pico_cyw43_arch_none
                    pico_btstack_ble
                    pico_btstack_cyw43
                    hardware_pwm
                    hardware_adc
                    pico_adxl345)
