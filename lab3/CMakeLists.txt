add_executable(lab3
        main.cpp
        banknote.cpp
        date.cpp
)

target_include_directories(lab3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(lab3 PRIVATE -Wall -Wextra -Wpedantic -O0 -g)