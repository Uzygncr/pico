cmake_minimum_required(VERSION 3.22)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)


include(pico_sdk_import.cmake)
project(w_blink_test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(test
    w_blink.c
)

pico_add_extra_outputs(test)
target_include_directories(test PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(test pico_cyw43_arch_lwip_threadsafe_background pico_stdlib)