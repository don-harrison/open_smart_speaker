cmake_minimum_required(VERSION 3.20)

project(teensy40_tflm_hello_world C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------
# Paths
# --------------------------------------------------
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/teensy/imxrt1062.ld)
set(TFLM_DIR ${CMAKE_SOURCE_DIR}/thirdparty/tflite-micro)

# --------------------------------------------------
# MCU flags
# --------------------------------------------------
set(MCU_FLAGS
    -mcpu=cortex-m7
    -mthumb
    -mfpu=fpv5-d16
    -mfloat-abi=hard
)

set(COMMON_FLAGS
    -O2
    -ffunction-sections
    -fdata-sections
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-missing-field-initializers
)

# --------------------------------------------------
# App sources
# --------------------------------------------------
set(APP_SOURCES
    src/main.cpp
    src/model_data.cc
    src/syscalls.c
    src/debug_log.cc
    src/error_reporter_shim.cc
    teensy/startup.c
    teensy/bootdata.c
)

# --------------------------------------------------
# Minimal TFLM sources
# --------------------------------------------------
set(TFLM_SOURCES
    ${TFLM_DIR}/tensorflow/lite/micro/flatbuffer_utils.cc
    ${TFLM_DIR}/tensorflow/lite/kernels/internal/common.cc
    ${TFLM_DIR}/tensorflow/lite/kernels/internal/quantization_util.cc
    ${TFLM_DIR}/tensorflow/lite/kernels/kernel_util.cc
    ${TFLM_DIR}/tensorflow/lite/core/c/common.cc
    ${TFLM_DIR}/tensorflow/lite/core/api/flatbuffer_conversions.cc

    ${TFLM_DIR}/tensorflow/lite/micro/micro_allocator.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_allocation_info.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_context.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_interpreter.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_interpreter_context.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_interpreter_graph.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_log.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_op_resolver.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_profiler.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_resource_variable.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_time.cc
    ${TFLM_DIR}/tensorflow/lite/micro/micro_utils.cc
    ${TFLM_DIR}/tensorflow/lite/micro/memory_helpers.cc
    ${TFLM_DIR}/tensorflow/lite/micro/system_setup.cc

    ${TFLM_DIR}/tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.cc
    ${TFLM_DIR}/tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.cc
    ${TFLM_DIR}/tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.cc

    ${TFLM_DIR}/tensorflow/lite/micro/memory_planner/greedy_memory_planner.cc
    ${TFLM_DIR}/tensorflow/lite/micro/memory_planner/linear_memory_planner.cc
    ${TFLM_DIR}/tensorflow/lite/micro/memory_planner/non_persistent_buffer_planner_shim.cc

    ${TFLM_DIR}/tensorflow/lite/micro/tflite_bridge/flatbuffer_conversions_bridge.cc
    ${TFLM_DIR}/tensorflow/lite/micro/tflite_bridge/micro_error_reporter.cc

    ${TFLM_DIR}/tensorflow/lite/micro/kernels/kernel_util.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/fully_connected.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/fully_connected_common.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/reshape.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/reshape_common.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/logistic.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/logistic_common.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/quantize.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/quantize_common.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/dequantize.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/dequantize_common.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/mul.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/mul_common.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/activations_common.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/elementwise.cc
    ${TFLM_DIR}/tensorflow/lite/micro/kernels/micro_tensor_utils.cc
    ${TFLM_DIR}/tensorflow/compiler/mlir/lite/schema/schema_utils.cc
    ${TFLM_DIR}/tensorflow/lite/kernels/internal/portable_tensor_utils.cc
)

add_executable(firmware.elf
    ${APP_SOURCES}
    ${TFLM_SOURCES}
)

target_compile_options(firmware.elf PRIVATE
    ${MCU_FLAGS}
    ${COMMON_FLAGS}
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-unwind-tables>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-asynchronous-unwind-tables>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
)

# --------------------------------------------------
# Include paths
# --------------------------------------------------
target_include_directories(firmware.elf PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src

    ${TFLM_DIR}
    ${TFLM_DIR}/tensorflow
    ${TFLM_DIR}/tensorflow/lite
    ${TFLM_DIR}/tensorflow/lite/c
    ${TFLM_DIR}/tensorflow/lite/core/api
    ${TFLM_DIR}/tensorflow/lite/micro
    ${TFLM_DIR}/tensorflow/lite/micro/kernels
    ${TFLM_DIR}/tensorflow/lite/micro/tflite_bridge
    ${TFLM_DIR}/tensorflow/lite/schema
    ${CMAKE_SOURCE_DIR}/thirdparty/flatbuffers/include
    ${CMAKE_SOURCE_DIR}/thirdparty/gemmlowp
    ${CMAKE_SOURCE_DIR}/thirdparty/ruy
)

target_compile_definitions(firmware.elf PRIVATE
    TF_LITE_STATIC_MEMORY
    TFLITE_USE_CTIME=0
)

# --------------------------------------------------
# Link
# --------------------------------------------------
target_link_options(firmware.elf PRIVATE
    ${MCU_FLAGS}
    -T${LINKER_SCRIPT}
    -specs=nosys.specs
    -Wl,--gc-sections
    -Wl,-Map=${CMAKE_BINARY_DIR}/firmware.map
)

target_link_libraries(firmware.elf
    m
    c
    gcc
    stdc++
)

# --------------------------------------------------
# Post-build outputs
# --------------------------------------------------
add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:firmware.elf> ${CMAKE_BINARY_DIR}/firmware.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:firmware.elf> ${CMAKE_BINARY_DIR}/firmware.bin
    COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:firmware.elf>
)