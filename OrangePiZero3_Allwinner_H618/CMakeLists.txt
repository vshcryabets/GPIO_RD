cmake_minimum_required(VERSION 3.22)

project(TestMmapGPIOAccess LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(include)

add_executable(TestMmapGPIOAccess
    src/testMmapGpio.cpp)

add_executable(TestSysClassGPIOAccess
    src/testSysClassGpio.cpp)

# add_executable(TestLibGpiod
#     src/testLibGpiod.cpp)

add_executable(TestLibGpiodv2
    src/testLibGpiodv2.cpp)

target_link_libraries(TestLibGpiodv2 gpiod)