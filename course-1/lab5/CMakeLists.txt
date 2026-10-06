add_executable(c1-lab5
        main.cpp
        SparseMatrix.cpp
)

target_compile_options(c1-lab5 PRIVATE -Wall -Wextra -Wpedantic -O0 -g)