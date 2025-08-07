#include "uart.hpp"
#include <cstdint>
#include <iostream>
#include <string>

Uart::Uart(const std::string &device_path) : fd_(-1) {
  std::cout << "[uart] initializing\n";

  // open UART device (e.g., /dev/ttyS1)
  // configure termios settings (baud rate, 8N1, no flow control)
  // store file descriptor in fd_
}

Uart::~Uart() {
  std::cout << "[uart] destroying\n";

  // close fd_ if open
}

bool Uart::send(const uint8_t *payload, size_t length) {
  std::cout << "[uart] sending\n";

  // check length doesn’t exceed MAX_FRAME_SIZE
  // build frame: [LENGTH][CRC][PAYLOAD]
  // - LENGTH = payload size (1 byte)
  // - CRC = XOR checksum or CRC-8-ATM
  // - append payload
  // write frame to fd_

  return true;
}

bool Uart::receive(uint8_t *payload_buffer, size_t max_length,
                   size_t &received_length) {
  std::cout << "[uart] receiving\n";

  // read frame from fd_: [LENGTH][CRC][PAYLOAD]
  // check LENGTH <= max_length
  // verify CRC
  // copy payload into payload_buffer
  // set received_length

  return true;
}

uint8_t Uart::compute_crc(const uint8_t *data, size_t length) {
  std::cout << "[uart] computing crc\n";

  // compute XOR/CRC-8-ATM checksum

  return 0x00;
}
