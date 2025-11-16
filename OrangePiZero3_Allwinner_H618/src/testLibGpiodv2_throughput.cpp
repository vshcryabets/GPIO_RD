#include <thread>
#include <chrono>
#include <iostream>
#include <gpiod.h>
#include "gpio.h"

using namespace std::chrono_literals;

int main()
{
    std::string chipId = "gpiochip1"; // corresponds to 0x0300B000
    unsigned int pinId = static_cast<uint8_t>(PIN_PORT) * 32 + PIN_ID;
    gpiod_request_config *req_config = nullptr;
    gpiod_chip *chip = nullptr;
    gpiod_line_request *request = nullptr;
    gpiod_line_config *line_config = nullptr;
    gpiod_line_settings *line_settings = nullptr;

    do
    {
        chip = gpiod_chip_open(("/dev/" + chipId).c_str());
        if (!chip)
        {
            std::cerr << "Failed to open " << chipId << std::endl;
            break;
        }
        line_settings = gpiod_line_settings_new();
        if (!line_settings)
        {
            std::cerr << "Failed to create line settings for " << chipId << std::endl;
            break;
        }
        gpiod_line_settings_set_direction(line_settings, GPIOD_LINE_DIRECTION_OUTPUT);
        line_config = gpiod_line_config_new();
        if (!line_config)
        {
            std::cerr << "Failed to create line config for " << chipId << std::endl;
            break;
        }
        gpiod_line_config_add_line_settings(line_config, &pinId, 1, line_settings);

        // Створюємо конфіг для запиту
        req_config = gpiod_request_config_new();
        if (!req_config)
        {
            std::cerr << "Failed to request config for " << chipId << std::endl;
            break;
        }
        gpiod_request_config_set_consumer(req_config, "my-blink-app");

        // Запитуємо лінію(ї)
        request = gpiod_chip_request_lines(chip, req_config, line_config);
        // Звільняємо конфіг запиту, він більше не потрібен
        gpiod_request_config_free(req_config);
        if (!request)
        {
            std::cerr << "Failed to request line for " << chipId << std::endl;
            break;
        }

        // Toggle the line state
	constexpr uint64_t max = 10000000;
	std::cout << "Send " << max << " ticks" << std::endl;
        for (uint64_t i = 0; i < max; i++)
        {
            gpiod_line_value value = (i % 2) ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
            gpiod_line_request_set_value(request, pinId, value);
        }
    } while (false);

    if (request != nullptr)
        gpiod_line_request_release(request);

    if (line_config != nullptr)
        gpiod_line_config_free(line_config);

    if (line_settings != nullptr)
        gpiod_line_settings_free(line_settings);

    if (chip != nullptr)
        gpiod_chip_close(chip);
    return 0;
}
