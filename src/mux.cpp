#include "mux.hpp"

Mux::Mux(I2c &i2c) : i2c_(i2c) {}

bool Mux::select(CameraType cam_type) {
  uint8_t channel =
      (cam_type == CameraType::MAIN) ? MUX_CHANNEL_MAIN : MUX_CHANNEL_AUX;

  return i2c_.write(MUX_I2C_ADDRESS, &channel, 1);
}