#pragma once

#include <cstdint>

enum class GPIOPort: uint8_t {
    PA = 0,
    PB = 1,
    PC = 2,
    PD = 3,
};

constexpr uint8_t PIN_ID = 7;
constexpr GPIOPort PIN_PORT = GPIOPort::PC;