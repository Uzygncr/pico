cmake_minimum_required(VERSION 3.13)
# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(encoder C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Pico SDK'yi başlat
pico_sdk_init()

# Kaynak dosyanızı ekleyin
add_executable(${PROJECT_NAME}
    main.c
)

# Gerekli Pico SDK kütüphanelerini linkleyin
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_gpio)

# Standart giriş/çıkışı etkinleştirin
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# ELF, BIN, HEX çıktıları oluşturun
pico_add_extra_outputs(${PROJECT_NAME})
