# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)


project(Graduation_Project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${PROJECT_NAME}
    blink.c
    calibration.c
    encoder.c
    lcd.c
    main.c
    motor.c
    uart.c
    
)


# Hedef kaynaklarını tanımla

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})


target_link_libraries(${PROJECT_NAME} 
    pico_cyw43_arch_lwip_threadsafe_background 
    pico_stdlib
    hardware_gpio   # GPIO donanım kütüphanesi
    hardware_uart   # Uart donanım kütüphanesi
    hardware_timer
    pico_multicore
    hardware_i2c
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})


