#pragma once

#include "actuator_hal.hpp"
#include "config.hpp"
#include "image_capturer.hpp"

class Focuser {
public:
    Focuser(ImageCapturer &capturer, ActuatorHal &actuator);

    void run(const FocusParams &params);
    void handoff_frame(const FrameView &frame);

private:
    enum class State {
        INIT,
        MOVE_STAGE,
        MOVE_FOCUS,
        CAPTURE,
        EVALUATE,
        COMPLETE
    };

    State state_;
    ImageCapturer &capturer_;
    ActuatorHal &actuator_;

    float evaluate_focus(const FrameView &frame);
};
