add_executable(c2-lab1
        main.cpp
        Student.cpp
        University.cpp
)

target_compile_options(c2-lab1 PRIVATE -Wall -Wextra -Wpedantic -O0 -g)