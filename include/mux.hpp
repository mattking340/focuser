#pragma once

#include "config.hpp"
#include "i2c.hpp"

class Mux {
public:
  Mux(I2c &i2c);

  bool select(CameraType cam_type);

private:
  I2c &i2c_;
};