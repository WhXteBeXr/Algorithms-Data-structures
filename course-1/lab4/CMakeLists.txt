add_executable(c1-lab4
        main.cpp
        IntVector.cpp
)

target_compile_options(c1-lab4 PRIVATE -Wall -Wextra -Wpedantic -O0 -g)