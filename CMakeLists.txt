cmake_minimum_required(VERSION 4.1)

project(Algorithms-DataStructures
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Включаем Debug по умолчанию
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Общие файлы, которые могут использоваться всеми лабораторными
set(GLOBAL_SOURCES
        lab1/taskImplementations/taskOne.cpp
        lab1/taskImplementations/taskTwo.cpp
        lab1/taskImplementations/taskThree.cpp
        lab1/taskImplementations/taskFour.cpp
        lab1/taskImplementations/TaskSix.cpp
        lab1/taskImplementations/TaskFive.cpp
        lab1/labFunctions.h)

# Найти все main.cpp ровно на один уровень глубже корня
file(GLOB LAB_SOURCES CONFIGURE_DEPENDS */main.cpp)

foreach(LAB_MAIN IN LISTS LAB_SOURCES)
    # Папка лабораторной
    get_filename_component(LAB_DIR ${LAB_MAIN} DIRECTORY)
    # Имя папки = имя исполняемого файла
    get_filename_component(LAB_NAME ${LAB_DIR} NAME)

    add_executable(${LAB_NAME} ${LAB_MAIN} ${GLOBAL_SOURCES})

    # Общие предупреждения и флаги для отладки
    target_compile_options(${LAB_NAME} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -O0      # отключаем оптимизации для отладки
            -g       # включаем отладочную информацию
    )
endforeach()