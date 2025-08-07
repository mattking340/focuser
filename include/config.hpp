#pragma once

#include <cstdint>
#include <string>

// Assumed Linux device paths and I2C mux addresses

constexpr char ACTUATOR_HAL_UART_DEVICE[] = "/dev/ttyUSB0";
constexpr char V4L2_CAMERA_DEVICE[] = "/dev/video0";
constexpr char MUX_I2C_DEVICE[] = "/dev/i2c-1";
constexpr uint8_t MUX_I2C_ADDRESS = 0x70;
constexpr uint8_t MUX_CHANNEL_MAIN = 0x01;
constexpr uint8_t MUX_CHANNEL_AUX = 0x02;

// GPIO Control

enum class Output : uint8_t {
    UV_ILLUMINATION,
    WHITE_ILLUMINATION,
    RESET_LINE,
};

enum class OutputMode : uint8_t {
    DIGITAL,
    ANALOG,
};

struct OutputCommand {
    Output pin;
    OutputMode mode;
    union {
        bool state;
        float current_ma;
    };
    uint32_t illumination_dwell_ms;
};

// Camera Config

enum class CameraType {
    MAIN,
    AUX,
};

struct CameraFormat {
    uint32_t width_px;
    uint32_t height_px;
    uint32_t pixel_format;  // V4L2_PIX_FMT_*
};

struct CameraControls {
    uint32_t exposure_us;
    uint32_t gain;
};

struct FrameView {
    const uint8_t *data;
    size_t length;
    int index;
};

// Auto focus Config

struct Range {
    int start;
    int stop;
    int step;
};

struct FocusSweepParams {
    Range focus_um;
    Range stage_steps;
    uint32_t focus_settle_ms;
    uint32_t stage_settle_ms;
};

enum class FocusType {
    SWEEP,
    FIBONACCI,
};

struct FocusParams {
    FocusType type;
    union {
        FocusSweepParams sweep;
        // other
    };
};

// Actuator Protocol (Command/Response)

enum class CommandType : uint8_t {
    MOVE_FOCUS,
    MOVE_STAGE,
    GPIO_WRITE,
};

enum class ResponseType : uint8_t {
    OK,
    ERROR,
    TIMEOUT,
};

struct MoveFocusCommand {
    int32_t microns;
};

struct MoveStageCommand {
    int32_t steps;
};

struct Command {
    CommandType type;
    union {
        MoveFocusCommand move_focus;
        MoveStageCommand move_stage;
        OutputCommand output;
    } payload;
};

struct GpioWriteResponse {
    float measured_current_ma;
};

struct MoveStageResponse {
    int encoder_pos;
    float position_error_step;
};

struct MoveFocusResponse {
    float pwm_duty_cycle;
    float feedback_voltage_mv;
};

struct Response {
    ResponseType type;
    std::string message;

    union {
        GpioWriteResponse gpio_write;
        MoveStageResponse move_stage;
        MoveFocusResponse move_focus;
    } payload;
};
