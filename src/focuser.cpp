#include "focuser.hpp"

#include <chrono>
#include <iostream>
#include <thread>

#include "focus_metric.hpp"

Focuser::Focuser(ImageCapturer &capturer, ActuatorHal &actuator)
    : capturer_(capturer), actuator_(actuator) {
    std::cout << "[focuser] init\n";
}

void Focuser::run(const FocusParams &p) {
    std::cout << "[focuser] run\n";
    if (p.type != FocusType::SWEEP) return;

    state_ = State::INIT;

    struct {
        int stage;
        int focus;
    } current = {p.sweep.stage_steps.start, p.sweep.focus_um.start};

    struct {
        int stage;
        int focus;
        float score;
    } best = {current.stage, current.focus, -1.0f};

    FrameView frame;

    while (state_ != State::COMPLETE) {
        switch (state_) {
            case State::INIT:
                std::cout << "[focuser] State: INIT\n";
                actuator_.reset_watchdog();
                capturer_.start();
                state_ = State::MOVE_STAGE;
                break;

            case State::MOVE_STAGE:
                std::cout << "[focuser] State: MOVE_STAGE " << current.stage
                          << "\n";
                actuator_.move_stage(current.stage);
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(p.sweep.stage_settle_ms));
                current.focus = p.sweep.focus_um.start;
                state_ = State::MOVE_FOCUS;
                break;

            case State::MOVE_FOCUS:
                std::cout << "[focuser] State: MOVE_FOCUS " << current.focus
                          << "\n";
                actuator_.move_focus(current.focus);
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(p.sweep.focus_settle_ms));
                state_ = State::CAPTURE;
                break;

            case State::CAPTURE:
                std::cout << "[focuser] State: CAPTURE\n";
                capturer_.capture(frame);  // assume success
                state_ = State::EVALUATE;
                break;

            case State::EVALUATE: {
                std::cout << "[focuser] State: EVALUATE\n";
                float score = evaluate_focus(frame);
                std::cout << "[focuser] focus score " << score << '\n';

                if (score > best.score) {
                    best.score = score;
                    best.stage = current.stage;
                    best.focus = current.focus;
                }

                capturer_.release(frame.index);

                current.focus += p.sweep.focus_um.step;
                if (current.focus <= p.sweep.focus_um.stop) {
                    state_ = State::MOVE_FOCUS;
                } else {
                    current.stage += p.sweep.stage_steps.step;
                    state_ = (current.stage <= p.sweep.stage_steps.stop)
                                 ? State::MOVE_STAGE
                                 : State::COMPLETE;
                }
                break;
            }

            case State::COMPLETE: {
                break;
            }
        }
    }

    std::cout << "[focuser] State: COMPLETE\n";
    std::cout << "[focuser] best stage = " << best.stage
              << ", best focus = " << best.focus
              << ", best score: " << best.score << "\n";

    actuator_.move_focus(best.focus);
    actuator_.move_stage(best.stage);
    std::this_thread::sleep_for(
        std::chrono::milliseconds(p.sweep.stage_settle_ms));

    // grab result and handoff
    capturer_.capture(frame);
    handoff_frame(frame);
    capturer_.release(frame.index);
    capturer_.stop();
}

float Focuser::evaluate_focus(const FrameView &frame) {
    // place holder metric
    // focus_metric namespace can hold various focus algorithms
    return focus_metric::random_score(frame);
}

void Focuser::handoff_frame(const FrameView &frame) {
    std::cout << "[focuser] handing off frame to another process\n";

    // use shm_open + mmap for zero-copy frame sharing
    // signal readiness via named pipe (mkfifo) or lock file
}
