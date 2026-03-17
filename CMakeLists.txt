cmake_minimum_required(VERSION 4.1)

project(Algorithms-DataStructures
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Найти все main.cpp ровно на один уровень глубже корня
file(GLOB LAB_SOURCES CONFIGURE_DEPENDS */main.cpp)

foreach(LAB_MAIN IN LISTS LAB_SOURCES)
    # Папка лабораторной
    get_filename_component(LAB_DIR ${LAB_MAIN} DIRECTORY)
    # Имя папки = имя исполняемого файла
    get_filename_component(LAB_NAME ${LAB_DIR} NAME)

    add_executable(${LAB_NAME} ${LAB_MAIN})

    # Общие предупреждения (по желанию)
    target_compile_options(${LAB_NAME} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endforeach()
