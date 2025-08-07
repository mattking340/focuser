#pragma once

#include <cstdint>
#include <string>

class I2c {
public:
    I2c(const std::string &device_path);
    ~I2c();

    bool write(uint8_t device_addr, const uint8_t *data, size_t length);
    bool read(uint8_t device_addr, uint8_t *buffer, size_t length);

private:
    int fd_;
};