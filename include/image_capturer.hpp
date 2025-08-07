#pragma once

#include "actuator_hal.hpp"
#include "camera_hal.hpp"
#include "config.hpp"
#include "i2c.hpp"
#include "mux.hpp"

class ImageCapturer {
public:
    ImageCapturer(CameraHal &camera, ActuatorHal &actuator, Mux &mux,
                  CameraType camera_type, const CameraFormat &format,
                  const CameraControls &default_controls,
                  const OutputCommand &default_output);

    bool start();
    bool capture(FrameView &frame,
                 const CameraControls *ctrl_override = nullptr,
                 const OutputCommand *output_override = nullptr);
    void release(int frame_index);
    bool stop();

private:
    CameraHal &camera_;
    ActuatorHal &actuator_;
    Mux &mux_;

    CameraType camera_type_;

    CameraFormat format_;
    CameraControls controls_;
    OutputCommand default_output_;

    bool configured_;
};
