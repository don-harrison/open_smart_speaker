#include "imxrt1062.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "model_data.h"

extern "C" void delay_cycles(volatile uint32_t count) {
    while (count--) {
        __asm__ volatile("nop");
    }
}

// TFLM headers
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/tflite_bridge/micro_error_reporter.h"
#include "tensorflow/lite/schema/schema_generated.h"

// You will generate/add these from the hello_world example model
#include "model_data.h"

namespace {
tflite::MicroErrorReporter micro_error_reporter;
tflite::ErrorReporter* error_reporter = &micro_error_reporter;

constexpr int kTensorArenaSize = 8 * 1024;
alignas(16) uint8_t tensor_arena[kTensorArenaSize];
}

static void led_init() {
    IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_03 = 5;
    IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_03 = IOMUXC_PAD_DSE(7);
    IOMUXC_GPR_GPR27 = 0xFFFFFFFF;
    GPIO7_GDIR |= (1 << 3);
}

static void led_on() {
    GPIO7_DR_SET = (1 << 3);
}

static void led_off() {
    GPIO7_DR_CLEAR = (1 << 3);
}

extern "C" int main() {
    led_init();

    const tflite::Model* model = tflite::GetModel(g_model_data);
    if (model->version() != 3) {
        while (1) {
            led_on();  delay_cycles(2000000);
            led_off(); delay_cycles(2000000);
        }
    }

    tflite::MicroMutableOpResolver<6> resolver;
    resolver.AddFullyConnected();
    resolver.AddReshape();
    resolver.AddLogistic();
    resolver.AddQuantize();
    resolver.AddDequantize();
    resolver.AddMul();

    tflite::MicroInterpreter interpreter(
        model,
        resolver,
        tensor_arena,
        kTensorArenaSize,
        nullptr,
        nullptr,
        false);
        
    if (interpreter.AllocateTensors() != kTfLiteOk) {
        while (1) {
            led_on();  delay_cycles(500000);
            led_off(); delay_cycles(500000);
        }
    }

    TfLiteTensor* input = interpreter.input(0);
    TfLiteTensor* output = interpreter.output(0);

    float x = 0.0f;
    while (1) {
        input->data.f[0] = x;

        if (interpreter.Invoke() != kTfLiteOk) {
            led_on(); delay_cycles(100000);
            led_off(); delay_cycles(100000);
            continue;
        }

        float y = output->data.f[0];

        // map y roughly from [-1,1] to a delay range
        float normalized = (y + 1.0f) * 0.5f;
        uint32_t on_delay  = 100000 + (uint32_t)(normalized * 1500000.0f);
        uint32_t off_delay = 100000 + (uint32_t)((1.0f - normalized) * 1500000.0f);

        led_on();
        delay_cycles(on_delay);
        led_off();
        delay_cycles(off_delay);

        x += 0.1f;
        if (x > 6.28f) x = 0.0f;
    }
}