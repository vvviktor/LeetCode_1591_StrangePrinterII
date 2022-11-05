cmake_minimum_required(VERSION 3.10)

project(strange_printer_II)
set(CMAKE_CXX_STANDARD 17)

add_executable(strange_printer_II StrangePrinterII.cpp my_assert.h my_assert.cpp)