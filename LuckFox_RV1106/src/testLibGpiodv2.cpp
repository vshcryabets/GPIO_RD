#include <gpiod.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "unistd.h"

#include "gpio.h"

int main() {
    std::cout << "Testing GPIO pin 124" << std::endl;
    std::string chipId = "gpiochip1";
    unsigned int pinId = static_cast<uint8_t>(PIN_PORT) * 8 + PIN_ID;
    // 1. Open the chip
    struct gpiod_chip *chip = gpiod_chip_open_by_name(chipId.c_str());
    if (!chip) {
        std::cerr << "Error: failed to open " << chipId << std::endl;
        return 1;
    }

    // 2. Get the line (GPIO1_B2 = 10)
    struct gpiod_line *line = gpiod_chip_get_line(chip, pinId);
    if (!line) {
        std::cerr << "Error: failed to find line " << pinId << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }
    if (gpiod_line_request_output(line, "test-app", 0) < 0) {
        std::cerr << "Error: failed to configure line as output" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }
    for (int i = 0; i < 100; ++i) {
        int val = i % 2;
        gpiod_line_set_value(line, val);
        int current_val = gpiod_line_get_value(line); 
        std::cout << "Set: " << val << " | Real: " << current_val << std::endl;
        sleep(2);
    }
    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return 0;
}
