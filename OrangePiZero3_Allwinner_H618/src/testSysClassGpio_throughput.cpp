#include <thread>
#include <chrono>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include "gpio.h"

using namespace std::chrono_literals;

int main() {
    uint16_t pinId = static_cast<uint8_t>(PIN_PORT) * 32 + PIN_ID;
    std::string pinStr = std::to_string(pinId);

    // export pin
    int exportFd = open("/sys/class/gpio/export", O_WRONLY);
    if (exportFd < 0) {
        std::cerr << "Can't open /sys/class/gpio/export: " << strerror(errno) << std::endl;
        return 1;
    }
    write(exportFd, pinStr.c_str(), pinStr.size());
    close(exportFd);

    // configure pin direction
    std::string gpioCtrlPath = "/sys/class/gpio/gpio" + pinStr;
    std::string directionPath = gpioCtrlPath + "/direction";
    int directionFd = open(directionPath.c_str(), O_WRONLY);
    if (directionFd < 0) {
        std::cerr << "Can't open " << directionPath << ": " << strerror(errno) << std::endl;
        return 1;
    }
    write(directionFd, "out", 3);
    close(directionFd);

    // toggle pin value
    std::string valuePath = gpioCtrlPath + "/value";
    int valueFd = open(valuePath.c_str(), O_RDWR);
    if (valueFd < 0) {
        std::cerr << "Can't open " << valuePath << ": " << strerror(errno) << std::endl;
        return 1;
    }

    char valueBuf;
    // Toggle the line state
    constexpr uint64_t max = 10000000;
    std::cout << "Send " << max << " ticks" << std::endl;    
    for(uint64_t i = 0; i < max; i++) {
        valueBuf = (i % 2) ? '1' : '0';
        if (write(valueFd, &valueBuf, 1) != 1) {
            std::cerr << "Error writing value: " << strerror(errno) << std::endl;
            close(valueFd);
            return 1;
        }
    }
    close(valueFd);

    // unexport pin
    int unexportFd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (unexportFd < 0) {
        std::cerr << "Can't open /sys/class/gpio/unexport: " << strerror(errno) << std::endl;
        return 1;
    }
    write(unexportFd, pinStr.c_str(), pinStr.size());
    close(unexportFd);
    return 0;
}