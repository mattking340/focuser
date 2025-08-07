#include "i2c.hpp"

#include <iostream>

I2c::I2c(const std::string &device_path) : fd_(-1) {
    std::cout << "[i2c] initializing\n";

    // open I2C device (e.g., /dev/i2c-X)
    // store file descriptor in fd_
}

I2c::~I2c() {
    std::cout << "[i2c] destroying\n";

    // close fd_ if open
}

bool I2c::write(uint8_t device_addr, const uint8_t *data, size_t length) {
    std::cout << "[i2c] write\n";

    // use ioctl(fd_, I2C_SLAVE, device_addr)
    // write data buffer to fd_

    return true;
}

bool I2c::read(uint8_t device_addr, uint8_t *buffer, size_t length) {
    std::cout << "[i2c] read\n";

    // use ioctl(fd_, I2C_SLAVE, device_addr)
    // read data into buffer from fd_

    return true;
}
