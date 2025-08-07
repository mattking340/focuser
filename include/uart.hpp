#pragma once

#include <cstdint>
#include <string>

class Uart {
public:
    static constexpr size_t MAX_FRAME_SIZE = 256;

    Uart(const std::string &device_path);
    ~Uart();

    bool send(const uint8_t *payload, size_t length);
    bool receive(uint8_t *payload_buffer, size_t max_length,
                 size_t &received_length);

private:
    int fd_;

    uint8_t compute_crc(const uint8_t *data, size_t length);
};
