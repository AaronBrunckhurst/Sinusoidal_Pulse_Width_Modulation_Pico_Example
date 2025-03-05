cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_EXTRAS_PATH}/external/pico_extras_import.cmake)

# Set project name and C standard
project(SpwmPico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
pico_sdk_init()

# Add the executable
add_executable(spwm_demo
    main.c
    spwm.c
)

# Link with the Pico standard libraries
target_link_libraries(spwm_demo
    pico_stdlib
    hardware_pwm
    hardware_timer
)

# Enable USB output, UART output, or both
pico_enable_stdio_usb(spwm_demo 1)
pico_enable_stdio_uart(spwm_demo 1)

# Create output files (UF2, ELF, BIN)
pico_add_extra_outputs(spwm_demo)
