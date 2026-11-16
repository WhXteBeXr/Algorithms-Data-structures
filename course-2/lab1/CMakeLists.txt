cmake_minimum_required(VERSION 3.20)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(Course2Lab1 LANGUAGES CXX)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# Библиотека объединяет реализации классов, main.cpp использует её методы.
add_library(c2-lab1-model Employee.cpp Company.cpp)
target_compile_features(c2-lab1-model PUBLIC cxx_std_20)
set_target_properties(c2-lab1-model PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(c2-lab1-model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(c2-lab1 main.cpp)
target_link_libraries(c2-lab1 PRIVATE c2-lab1-model)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(c2-lab1-model PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(c2-lab1 PRIVATE -Wall -Wextra -Wpedantic)
endif()
