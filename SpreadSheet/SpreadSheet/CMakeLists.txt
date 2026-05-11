cmake_minimum_required(VERSION 3.15)

# Название проекта
project(SpreadSheet CXX)

# Устанавливаем стандарт C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем исполняемый файл из исходных кодов
add_executable(SpreadSheet
    main.cpp
    structures.cpp
    common.h
    test_runner_p.h
)
