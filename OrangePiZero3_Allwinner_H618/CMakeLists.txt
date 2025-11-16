cmake_minimum_required(VERSION 3.22)

include_directories(include)

add_executable(TestMmapGPIOAccess
    src/testMmapGpio.cpp)

add_executable(TestMmapGPIOAccess_throughput
    src/testMmapGpio_throughput.cpp)

add_executable(TestSysClassGPIOAccess
    src/testSysClassGpio.cpp)

if (USE_LIBGPIOD2)
    message(STATUS "Building with libgpiod v2 support")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GPIOD REQUIRED libgpiod)
    include_directories(${GPIOD_INCLUDE_DIRS})
    link_directories(${GPIOD_LIBRARY_DIRS})
    add_definitions(${GPIOD_CFLAGS_OTHER})

    add_executable(TestLibGpiodv2
        src/testLibGpiodv2.cpp)
    target_link_libraries(TestLibGpiodv2 gpiod)

    add_executable(TestLibGpiodv2_throughput
        src/testLibGpiodv2_throughput.cpp)
    target_link_libraries(TestLibGpiodv2_throughput gpiod)

else()
    message(STATUS "Building with libgpiod v1 support")
    add_executable(TestLibGpiod
        src/testLibGpiod.cpp)
    target_link_libraries(TestLibGpiod gpiod)
endif()