cmake_minimum_required(VERSION 3.20)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(Course2Lab13 LANGUAGES CXX)
endif()

add_executable(c2-lab13 main.cpp Graph.cpp Comparison.cpp)
target_compile_features(c2-lab13 PRIVATE cxx_std_20)
set_target_properties(c2-lab13 PROPERTIES CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(c2-lab13 PRIVATE -Wall -Wextra -Wpedantic)
endif()
