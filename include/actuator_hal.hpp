#pragma once

#include "config.hpp"
#include <cstdint>

class ActuatorHal {
public:
  virtual ~ActuatorHal() = default;

  virtual Response execute(const Command &cmd) = 0;

  Response reset_watchdog();
  Response move_focus(int32_t microns);
  Response move_stage(int32_t steps);
  Response gpio_write(Output pin, OutputMode mode, bool state);
  Response gpio_write(Output pin, OutputMode mode, float current_ma);
};