cmake_minimum_required(VERSION 3.20)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(Course2Lab4 LANGUAGES CXX)
endif()

# Определения шаблонов подключаются через заголовки, отдельно собираются два .cpp.
add_executable(c2-lab4
    main.cpp
    Benchmark.cpp
    Benchmark.h
    Node.h
    ListIterator.h
    SinglyLinkedList.h
    SinglyLinkedList.tpp
    DoublyLinkedList.h
    DoublyLinkedList.tpp
)
target_compile_features(c2-lab4 PRIVATE cxx_std_20)
set_target_properties(c2-lab4 PROPERTIES CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(c2-lab4 PRIVATE -Wall -Wextra -Wpedantic)
endif()
