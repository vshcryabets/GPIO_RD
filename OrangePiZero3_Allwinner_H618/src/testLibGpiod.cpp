#include <thread>
#include <chrono>
#include <iostream>
#include <gpiod.h>
#include "gpio.h"

using namespace std::chrono_literals;

int main() {
    std::string chipId = "gpiochip1";  // corresponds to 0x0300B000
    uint16_t pinId = static_cast<uint8_t>(PIN_PORT) * 32 + PIN_ID;
    struct gpiod_chip *chip = gpiod_chip_open_by_name(chipId.c_str());
    if (!chip) {
        std::cerr << "Failed to open " << chipId << std::endl;
        return 1;
    }
    struct gpiod_line *line = gpiod_chip_get_line(chip, pinId);
    if (!line) {
        std::cerr << "Failed to get line " << pinId << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }
    if (gpiod_line_request_output(line, "mmapMethod", 0) < 0) {
        std::cerr << "Failed to request line as output" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Toggle the line state
    for (int i = 0; i < 10; i++) {
        gpiod_line_set_value(line, i % 2);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    gpiod_chip_close(chip);
    return 0;
}