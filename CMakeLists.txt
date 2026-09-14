cmake_minimum_required(VERSION 3.20)
project(Algorithms-DataStructures LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Общие флаги компиляции для всех лаб — задаём один раз через функцию/переменную
set(COMMON_WARNINGS -Wall -Wextra -Wpedantic)

add_subdirectory(lab1)
add_subdirectory(lab2)
add_subdirectory(lab3)
add_subdirectory(lab4)