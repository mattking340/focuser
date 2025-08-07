#include "camera_hal_v4l2.hpp"
#include "camera_hal.hpp"
#include "config.hpp"

#include <iostream>
#include <string>

V4l2CameraHal::V4l2CameraHal(const std::string &device_path) : fd_(-1) {
  std::cout << "[v4l2] initializing\n";

  // open(device_path, O_RDWR)
  // store result in fd_
  // VIDIOC_QUERYCAP to confirm V4L2 support
}

V4l2CameraHal::~V4l2CameraHal() {
  std::cout << "[v4l2] destroying\n";

  // if (fd_ >= 0)
  //     close(fd_)
}

bool V4l2CameraHal::set_format(const CameraFormat &format) {
  std::cout << "[v4l2] setting format\n";

  // prepare v4l2_format struct
  // use VIDIOC_S_FMT to apply:
  //   - width
  //   - height
  //   - pixel format (e.g., V4L2_PIX_FMT_YUYV)

  return true;
}

bool V4l2CameraHal::set_controls(const CameraControls &controls) {
  std::cout << "[v4l2] setting controls\n";

  // use VIDIOC_S_CTRL or VIDIOC_S_EXT_CTRLS to set:
  //   - V4L2_CID_EXPOSURE
  //   - V4L2_CID_GAIN

  return true;
}

bool V4l2CameraHal::start_streaming() {
  std::cout << "[v4l2] starting stream\n";

  // request buffers via VIDIOC_REQBUFS
  // mmap buffers into userspace
  // queue all buffers using VIDIOC_QBUF
  // start stream with VIDIOC_STREAMON

  return true;
}

bool V4l2CameraHal::stop_streaming() {
  std::cout << "[v4l2] stopping stream\n";

  // stop stream with VIDIOC_STREAMOFF
  // unmap buffers if needed

  return true;
}

bool V4l2CameraHal::capture_frame(FrameView &view) {
  std::cout << "[v4l2] capturing frame\n";

  // dequeue buffer via VIDIOC_DQBUF
  // populate view:
  //   - view.data = mmap'd buffer start
  //   - view.length = v4l2_buffer.bytesused
  //   - view.index = v4l2_buffer.index

  return true;
}

void V4l2CameraHal::release_frame(int index) {
  std::cout << "[v4l2] releasing frame\n";

  // requeue buffer via VIDIOC_QBUF with buffer index
}
