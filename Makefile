cmake_minimum_required(VERSION 3.20)
project(teensy40_tflm C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MCU_FLAGS
    -mcpu=cortex-m7
    -mthumb
    -mfpu=fpv5-d16
    -mfloat-abi=hard
)

set(COMMON_FLAGS
    ${MCU_FLAGS}
    -O2
    -ffunction-sections
    -fdata-sections
    -fno-exceptions
    -fno-rtti
    -fno-threadsafe-statics
    -fno-use-cxa-atexit
    -fno-unwind-tables
    -fno-asynchronous-unwind-tables
    -Wall
    -Wextra
)

add_compile_options(${COMMON_FLAGS})

add_link_options(
    ${MCU_FLAGS}
    -T${CMAKE_SOURCE_DIR}/imxrt1062.ld
    -nostdlib
    -Wl,--gc-sections
    -Wl,-Map=${CMAKE_BINARY_DIR}/firmware.map
)

set(TFLM_DIR ${CMAKE_SOURCE_DIR}/third_party/tflite-micro)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${TFLM_DIR}
    ${TFLM_DIR}/tensorflow/lite/micro
    ${TFLM_DIR}/tensorflow/lite/schema
    ${TFLM_DIR}/tensorflow/lite/core/api
    ${TFLM_DIR}/tensorflow
)

set(APP_SOURCES
    src/main.cpp
    src/model_data.cc
    src/syscalls.c
    src/debug_log.cc
    src/error_reporter_shim.cc
    teensy/startup.c
    teensy/bootdata.c
)

set(TFLM_SOURCES
    ${TFLM_DIR}/tensorflow/lite/micro/micro_allocator.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_error_reporter.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_interpreter.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_mutable_op_resolver.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_resource_variables.cc
    ${TFLM_DIR}/tensorflow/lite/micro/system_setup.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/fully_connected.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/quantize.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/dequantize.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/add.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/mul.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/reshape.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/logistic.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/softmax.cc
)

add_executable(firmware.elf
    ${APP_SOURCES}
    ${TFLM_SOURCES}
)

target_include_directories(firmware.elf PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${TFLM_DIR}
)

target_link_libraries(firmware.elf
    gcc
    stdc++
    m
    c
)

add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex $<TARGET_FILE:firmware.elf> ${CMAKE_BINARY_DIR}/firmware.hex
    COMMAND arm-none-eabi-objcopy -O binary $<TARGET_FILE:firmware.elf> ${CMAKE_BINARY_DIR}/firmware.bin
)