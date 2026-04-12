add_executable(trumpet ${CMAKE_CURRENT_LIST_DIR}/src/main.c)

target_link_libraries(trumpet 
                    pico_stdlib
                    pico_cyw43_arch_lwip_threadsafe_background
                    hardware_pwm
                    hardware_adc
                    pico_multicore
                    pico_ssd1306)
