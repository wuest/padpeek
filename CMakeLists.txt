cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

set(TARGET "padpeek")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(padpeek)

# Initialize the SDK
pico_sdk_init()

add_executable(padpeek)

pico_generate_pio_header(padpeek ${CMAKE_CURRENT_LIST_DIR}/pico/shiftregister.pio)

target_sources(padpeek PRIVATE pico/padpeek.c)

target_include_directories(padpeek PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(padpeek PRIVATE
	pico_stdlib
	pico_multicore
	hardware_pio
	)

pico_enable_stdio_usb(padpeek 1)
pico_enable_stdio_uart(padpeek 0)

# Required for U2F output
pico_add_extra_outputs(padpeek)

# add url via pico_set_program_url
pico_set_program_name(${TARGET} "padpeek") 
pico_set_program_description(${TARGET} "SNES input display and input recording")
pico_set_program_url(${TARGET} "https://github.com/wuest/padpeek")
