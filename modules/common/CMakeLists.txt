file(GLOB PDOMOVOY_COMMON_EXECS ${CMAKE_CURRENT_LIST_DIR}/src/*.c)

cmake_minimum_required(VERSION 3.27)

add_library(pdomovoy_common INTERFACE)

target_link_libraries(pdomovoy_common INTERFACE
  pico_stdlib
  hardware_i2c
)

target_include_directories(pdomovoy_common INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")
target_sources(pdomovoy_common INTERFACE ${PDOMOVOY_COMMON_EXECS})
