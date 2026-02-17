cmake_minimum_required(VERSION 3.15)
project(Viperfish LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Core library
file(GLOB CORE_SOURCES "src/*.cpp")

add_library(viper_core OBJECT ${CORE_SOURCES})

target_include_directories(viper_core PUBLIC src)

# Test suites
option(GENERAL "Build the general test suite" ON)
option(PERFT "Build the PERFT suite." ON)
option(UNIT_TESTS "Build the unit tests suite." ON)

if(GENERAL)
    file(GLOB GENERAL_SOURCES "tests/general/*.cpp")
    add_executable(general ${GENERAL_SOURCES})
    target_link_libraries(general PRIVATE viper_core)
endif()

if(PERFT)
    file(GLOB PERFT_SOURCES "tests/perft/*.cpp")
    add_executable(perft ${PERFT_SOURCES})
    target_link_libraries(perft PRIVATE viper_core)
endif()

if(UNIT_TESTS)
    file(GLOB UNIT_TESTS_SOURCES "tests/unit_tests/*.cpp")
    add_executable(unit_tests ${UNIT_TESTS_SOURCES})
    target_link_libraries(unit_tests PRIVATE viper_core)
endif()
