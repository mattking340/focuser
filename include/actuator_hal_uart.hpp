#pragma once

#include <string>

#include "actuator_hal.hpp"
#include "config.hpp"
#include "uart.hpp"

class UartActuatorHal : public ActuatorHal {
public:
    UartActuatorHal(const std::string &device_path);
    ~UartActuatorHal();

    Response execute(const Command &cmd) override;

private:
    Uart uart_;
};