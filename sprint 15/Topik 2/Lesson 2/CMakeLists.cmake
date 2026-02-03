cmake_minimum_required(VERSION 3.14)

project(Negate VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(negate
    img_lib.h
    img_lib.cpp
    ppm_image.h
    ppm_image.cpp
    main.cpp
)