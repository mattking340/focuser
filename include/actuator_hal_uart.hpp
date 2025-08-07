#pragma once

#include "actuator_hal.hpp"
#include "config.hpp"
#include "uart.hpp"
#include <string>

class UartActuatorHal : public ActuatorHal {
public:
  UartActuatorHal(const std::string &device_path);
  ~UartActuatorHal();

  Response execute(const Command &cmd) override;

private:
  Uart uart_;
};