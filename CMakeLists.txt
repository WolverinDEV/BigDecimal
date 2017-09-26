cmake_minimum_required(VERSION 3.6)
project(BigDecimal)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp include/BigDecimal.h src/BigDecimal.cpp)
add_executable(BigDecimal ${SOURCE_FILES})