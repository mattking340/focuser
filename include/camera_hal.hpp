#pragma once

#include "config.hpp"

class CameraHal {
public:
    virtual ~CameraHal() = default;

    virtual bool set_format(const CameraFormat &format) = 0;
    virtual bool set_controls(const CameraControls &controls) = 0;

    virtual bool start_streaming() = 0;
    virtual bool stop_streaming() = 0;

    virtual bool capture_frame(FrameView &view) = 0;
    virtual void release_frame(int index) = 0;
};
