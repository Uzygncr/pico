cmake_minimum_required(VERSION 3.13)
# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(lcd_example C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(lcd_example
    lcd.cpp
)

target_link_libraries(lcd_example pico_stdlib hardware_i2c)

pico_enable_stdio_usb(lcd_example 1)
pico_enable_stdio_uart(lcd_example 0)

pico_add_extra_outputs(lcd_example)
