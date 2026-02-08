cmake_minimum_required(VERSION 3.12)
project(Viperfish LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp" "test/*.cpp")

add_executable(viper_test ${SOURCES})

target_include_directories(viper_test PRIVATE src)