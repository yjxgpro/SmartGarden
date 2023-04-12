cmake_minimum_required(VERSION 3.0)
project(l298n_test)

set(CMAKE_CXX_STANDARD 11)

find_library(PIGPIO_LIBRARY NAMES pigpio)

add_executable(l298n_test main.cpp)

target_link_libraries(l298n_test ${PIGPIO_LIBRARY})
