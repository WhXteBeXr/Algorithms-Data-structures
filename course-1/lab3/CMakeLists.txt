add_executable(c1-lab3
        main.cpp
        banknote.cpp
        date.cpp
)
target_compile_options(c1-lab3 PRIVATE -Wall -Wextra -Wpedantic -O0 -g)