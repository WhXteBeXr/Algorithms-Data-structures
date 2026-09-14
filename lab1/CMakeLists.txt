file(GLOB LAB1_TASK_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/taskImplementations/*.cpp)

add_executable(lab1
        main.cpp
        ${LAB1_TASK_SOURCES}
        labFunctions.h
)

target_include_directories(lab1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(lab1 PRIVATE -Wall -Wextra -Wpedantic -O0 -g)