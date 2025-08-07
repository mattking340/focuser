#include "actuator_hal_uart.hpp"

#include <cstdint>
#include <iostream>

#include "config.hpp"
#include "uart.hpp"

std::string command_type_to_string(CommandType type) {
    switch (type) {
        case CommandType::MOVE_FOCUS:
            return "MOVE_FOCUS";
        case CommandType::MOVE_STAGE:
            return "MOVE_STAGE";
        case CommandType::GPIO_WRITE:
            return "GPIO_WRITE";
    }
    return "UNKNOWN";
}

UartActuatorHal::UartActuatorHal(const std::string &device_path)
    : uart_(device_path) {
    std::cout << "[actuator_hal_uart] init\n";
}

UartActuatorHal::~UartActuatorHal() {
    std::cout << "[actuator_hal_uart] destroying\n";
}

Response UartActuatorHal::execute(const Command &cmd) {
    std::cout << "[actuator_hal_uart] executing command: "
              << command_type_to_string(cmd.type) << "\n";

    // serialize Command into Protobuf
    //   - proto::Command proto_cmd;
    //   - map fields from `cmd` into `proto_cmd`
    //   - proto_cmd.SerializeToArray(tx_buffer, tx_length);

    uint8_t tx_buffer[Uart::MAX_FRAME_SIZE] = {0};
    size_t tx_length = 16;

    // send Protobuf frame over UART
    uart_.send(tx_buffer, tx_length);

    // receive Protobuf response frame over UART
    uint8_t rx_buffer[Uart::MAX_FRAME_SIZE] = {0};
    size_t rx_length = 0;
    uart_.receive(rx_buffer, sizeof(rx_buffer), rx_length);

    // deserialize into Response struct
    //   - proto::Response proto_resp;
    //   - proto_resp.ParseFromArray(rx_buffer, rx_length);
    //   - map proto_resp fields into resp fields

    Response resp;
    resp.type = ResponseType::OK;
    resp.message = "totally just moved the focus actuator!";

    // mock response payload
    resp.payload.move_focus = {.pwm_duty_cycle = 0.42f,
                               .feedback_voltage_mv = 1234.5f};

    return resp;
}