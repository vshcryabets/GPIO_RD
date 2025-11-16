#include <chrono>
#include <thread>
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include "gpio.h"

using namespace std::chrono_literals;

int main() {
    int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);    
    if (mem_fd < 0) {
        std::cerr << "Can't open /dev/mem: " << strerror(errno) << std::endl;
        return 1;
    }

    constexpr off_t PIO_BASE_ADDR = 0x300B000;
    constexpr size_t PAGE_SIZE = 4096;

    void* map_base = mmap(
        nullptr,
        PAGE_SIZE,
        PROT_READ | PROT_WRITE,  // нам потрібен доступ на читання і запис
        MAP_SHARED,          
        mem_fd,              
        PIO_BASE_ADDR // фізична адреса (має бути вирівняна по розміру сторінки 4Kb)
    );
    close(mem_fd); // дескриптор файлу більше не потрібен після mmap

    if (map_base == MAP_FAILED) {
        std::cerr << "mmap error: " << strerror(errno) << std::endl;
        return 1;
    }
    
    // volatile щоб компілятор не кешував значення регістру і завжди 
    // виконував реальне читання/запис.
    volatile uint32_t* reg32_ptr = (volatile uint32_t*)map_base;

    uint32_t pin_port = static_cast<uint8_t>(PIN_PORT);

    uint32_t cfg0_off = pin_port * 0x24 + 0x00; // CFG0 register offset
    uint32_t dat_off = pin_port * 0x24 + 0x10; // DAT register offset

    uint32_t current_value = *(reg32_ptr + cfg0_off/4);
    current_value &= ~(0xF << PIN_ID * 4); // clear bits
    current_value |= (0x1 << PIN_ID * 4);  // set bits
    *(reg32_ptr + cfg0_off/4) = current_value;

    // Toggle the line state
    constexpr uint64_t max = 10000000;
    std::cout << "Send " << max << " ticks" << std::endl;
    for(uint64_t i = 0; i < max; i++) {
        current_value = *(reg32_ptr + dat_off/4);
        if (!(current_value & (1 << PIN_ID))) {
            current_value |= (1 << PIN_ID);  // set bit to 1
        } else {
            current_value &= ~(1 << PIN_ID);  // clear bit
        }
        *(reg32_ptr + dat_off/4) = current_value;
    }
    if (munmap(map_base, PAGE_SIZE) == -1) {
        std::cerr << "munmap error: " << strerror(errno) << std::endl;
    }
    return 0;
}