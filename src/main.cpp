#include <iostream>
#include <memory>

#include "actuator_hal_uart.hpp"
#include "camera_hal_v4l2.hpp"
#include "config.hpp"
#include "focuser.hpp"
#include "i2c.hpp"
#include "image_capturer.hpp"
#include "mux.hpp"

void run_test_harness() {
    std::cout << "[test] running autofocus test harness\n";

    CameraFormat format = {
        .width_px = 640,
        .height_px = 480,
        .pixel_format = 0  // mock value
    };

    CameraControls controls = {.exposure_us = 10000, .gain = 10};

    OutputCommand light = {.pin = Output::WHITE_ILLUMINATION,
                           .mode = OutputMode::DIGITAL,
                           .state = true,
                           .illumination_dwell_ms = 20};

    FocusSweepParams sweep = {.focus_um = {100, 200, 50},
                              .stage_steps = {0, 0, 1},
                              .focus_settle_ms = 10,
                              .stage_settle_ms = 20};

    FocusParams focus_params = {.type = FocusType::SWEEP, .sweep = sweep};

    I2c i2c(MUX_I2C_DEVICE);
    Mux mux(i2c);

    V4l2CameraHal camera(V4L2_CAMERA_DEVICE);
    UartActuatorHal actuator(ACTUATOR_HAL_UART_DEVICE);

    ImageCapturer capturer(camera, actuator, mux, CameraType::MAIN, format,
                           controls, light);

    Focuser focuser(capturer, actuator);
    focuser.run(focus_params);

    std::cout << "[test] autofocus test complete\n";
}

int main() {
    run_test_harness();
    return 0;
}
