
#include "actuator_hal.hpp"

#include <chrono>
#include <iostream>
#include <thread>

#include "config.hpp"

Response ActuatorHal::reset_watchdog() {
    std::cout << "[actuator_hal] resetting watchdog\n";

    gpio_write(Output::RESET_LINE, OutputMode::DIGITAL, false);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    return gpio_write(Output::RESET_LINE, OutputMode::DIGITAL, true);
}

Response ActuatorHal::move_focus(int32_t microns) {
    std::cout << "[actuator_hal] building command: MOVE_FOCUS\n";

    Command cmd;
    cmd.type = CommandType::MOVE_FOCUS;
    cmd.payload.move_focus.microns = microns;

    return execute(cmd);
}

Response ActuatorHal::move_stage(int32_t steps) {
    std::cout << "[actuator_hal] building command: MOVE_STAGE\n";

    Command cmd;
    cmd.type = CommandType::MOVE_STAGE;
    cmd.payload.move_stage.steps = steps;

    return execute(cmd);
}

Response ActuatorHal::gpio_write(Output pin, OutputMode mode, bool state) {
    std::cout << "[actuator_hal] building command: GPIO_WRITE (digital)\n";

    Command cmd;
    cmd.type = CommandType::GPIO_WRITE;
    cmd.payload.output.pin = pin;
    cmd.payload.output.mode = mode;
    cmd.payload.output.state = state;

    return execute(cmd);
}

Response ActuatorHal::gpio_write(Output pin, OutputMode mode,
                                 float current_ma) {
    std::cout << "[actuator_hal] building command: GPIO_WRITE (analog)\n";

    Command cmd;
    cmd.type = CommandType::GPIO_WRITE;
    cmd.payload.output.pin = pin;
    cmd.payload.output.mode = mode;
    cmd.payload.output.current_ma = current_ma;

    return execute(cmd);
}
