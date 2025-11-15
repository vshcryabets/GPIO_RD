#pragma once

#include <cstdint>

enum class GPIOPort: uint8_t {
    PC = 2,
    PF = 5,
    PG = 6,
    PH = 7,
    PI = 8
};

constexpr uint8_t PIN_ID = 3;
constexpr GPIOPort PIN_PORT = GPIOPort::PI;