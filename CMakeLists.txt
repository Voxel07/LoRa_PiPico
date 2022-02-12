cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(PointToPoint C CXX ASM)

set(CNAKE_C_STANDARD 11)
set(CNAKE_CXX_STANDARD 17)
SET(GCC_COVERAGE_COMPILE_FLAGS "-O0")
pico_sdk_init()

add_executable(${PROJECT_NAME}
    main.c
    myLoRa.c
    sx1276.c
    LightChrono.cpp
    pressure.c
    LoraMessage.c
)

pico_add_extra_outputs(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_spi
    hardware_gpio
    pico_unique_id
)

target_include_directories(
     ${PROJECT_NAME} PRIVATE ${SOURCE_DIR}
)

message(${PROJECT_NAME})
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)