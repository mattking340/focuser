#pragma once

#include "camera_hal.hpp"
#include "config.hpp"
#include <string>

class V4l2CameraHal : public CameraHal {
public:
  V4l2CameraHal(const std::string &device_path);
  ~V4l2CameraHal();

  bool set_format(const CameraFormat &format) override;
  bool set_controls(const CameraControls &controls) override;

  bool start_streaming() override;
  bool stop_streaming() override;

  bool capture_frame(FrameView &view) override;
  void release_frame(int index) override;

private:
  int fd_;
};
