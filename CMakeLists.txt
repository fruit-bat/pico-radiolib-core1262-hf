cmake_minimum_required(VERSION 3.18)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico-sx1262-hx C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(
    -Wall
    -Wno-format          
    -Wno-unused-function
)

set(RADIOLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../RadioLib" ) 
add_subdirectory("${RADIOLIB_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/RadioLib")

set(SOURCES
    "${RADIOLIB_DIR}/src/hal/RPiPico/PicoHal.cpp"
    setup.cpp
)

add_executable(${PROJECT_NAME}-tx
    main-tx.cpp
    ${SOURCES}
)

add_executable(${PROJECT_NAME}-rx
    main-rx.cpp
    ${SOURCES}
)

set(TARGETS ${PROJECT_NAME}-tx ${PROJECT_NAME}-rx)

foreach(proj ${TARGETS})
    target_compile_definitions(${proj} PUBLIC RADIOLIB_BUILD_RPI_PICO)

    # Pull in common dependencies
    target_link_libraries(${proj} pico_stdlib hardware_spi hardware_gpio hardware_timer pico_multicore hardware_pwm RadioLib)

    pico_enable_stdio_usb(${proj} 1)
    pico_enable_stdio_uart(${proj} 0)

    # Create map/bin/hex file etc.
    pico_add_extra_outputs(${proj})    
endforeach(proj in ${TARGETS})


