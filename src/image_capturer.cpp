#include "image_capturer.hpp"

#include <chrono>
#include <iostream>
#include <thread>

ImageCapturer::ImageCapturer(CameraHal &camera, ActuatorHal &actuator, Mux &mux,
                             CameraType cam_type, const CameraFormat &format,
                             const CameraControls &default_controls,
                             const OutputCommand &default_output)
    : camera_(camera),
      actuator_(actuator),
      mux_(mux),
      camera_type_(cam_type),
      format_(format),
      controls_(default_controls),
      default_output_(default_output),
      configured_(false) {
    std::cout << "[image_capturer] init\n";
}

bool ImageCapturer::start() {
    std::cout << "[image_capturer] starting stream\n";

    if (!configured_) {
        if (!mux_.select(camera_type_)) return false;
        if (!camera_.set_format(format_)) return false;
        if (!camera_.set_controls(controls_)) return false;
        configured_ = true;
    }

    return camera_.start_streaming();
}

bool ImageCapturer::capture(FrameView &frame,
                            const CameraControls *ctrl_override,
                            const OutputCommand *output_override) {
    const CameraControls &ctrl = ctrl_override ? *ctrl_override : controls_;
    const OutputCommand &output =
        output_override ? *output_override : default_output_;

    std::cout << "[image_capturer] turning lights on\n";
    if (output.mode == OutputMode::DIGITAL) {
        actuator_.gpio_write(output.pin, OutputMode::DIGITAL, true);
    } else if (output.mode == OutputMode::ANALOG) {
        actuator_.gpio_write(output.pin, OutputMode::ANALOG, output.current_ma);
    }

    std::cout << "[image_capturer] waiting for lights to be fully on\n";
    if (output.illumination_dwell_ms > 0) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(output.illumination_dwell_ms));
    }

    std::cout << "[image_capturer] capturing image frame\n";
    if (!camera_.set_controls(ctrl)) return false;
    if (!camera_.capture_frame(frame)) return false;

    std::cout << "[image_capturer] turning lights off\n";
    if (output.mode == OutputMode::DIGITAL) {
        actuator_.gpio_write(output.pin, OutputMode::DIGITAL, false);
    } else if (output.mode == OutputMode::ANALOG) {
        actuator_.gpio_write(output.pin, OutputMode::ANALOG, 0.0f);
    }

    return true;
}

void ImageCapturer::release(int frame_index) {
    std::cout << "[image_capturer] releasing frame\n";
    camera_.release_frame(frame_index);
}

bool ImageCapturer::stop() {
    std::cout << "[image_capturer] stopping stream\n";
    return camera_.stop_streaming();
}
