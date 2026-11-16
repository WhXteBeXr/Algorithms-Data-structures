cmake_minimum_required(VERSION 3.20)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(Course2Lab3 LANGUAGES CXX)
endif()

# Реализации шаблонов подключаются через Vector.h, отдельно компилируются два .cpp.
add_executable(c2-lab3
    main.cpp
    AmortizedAnalysis.cpp
    AmortizedAnalysis.h
    Vector.h
    Vector.tpp
    VectorBuffer.h
    ReverseIterator.h
)
target_compile_features(c2-lab3 PRIVATE cxx_std_20)
set_target_properties(c2-lab3 PROPERTIES CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(c2-lab3 PRIVATE -Wall -Wextra -Wpedantic)
endif()
