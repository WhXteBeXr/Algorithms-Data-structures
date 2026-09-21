file(GLOB LAB1_TASK_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/taskImplementations/*.cpp)

add_executable(c1-lab1
        main.cpp
        ${LAB1_TASK_SOURCES}
        labFunctions.h
)

target_include_directories(c1-lab1 PRIVATE .)
target_compile_options(c1-lab1 PRIVATE -Wall -Wextra -Wpedantic -O0 -g)