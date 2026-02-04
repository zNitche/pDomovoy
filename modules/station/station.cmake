add_executable(station ${CMAKE_CURRENT_LIST_DIR}/src/main.c)

target_link_libraries(station 
                    pico_stdlib
                    pico_cyw43_arch_none)
