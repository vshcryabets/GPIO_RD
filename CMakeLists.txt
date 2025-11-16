cmake_minimum_required(VERSION 3.22)

project(GpioRd LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

option(USE_LIBGPIOD2 "Use libgpiod v2 for GPIO access where applicable" ON)

add_subdirectory(OrangePiZero3_Allwinner_H618)