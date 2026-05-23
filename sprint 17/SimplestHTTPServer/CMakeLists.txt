cmake_minimum_required(VERSION 3.15)

project(SimplestHTTPServer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключаем Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Основной исполняемый файл
add_executable(SimplestHTTPServer src/main.cpp)

# Линкуем с Boost (включая boost_system)
target_link_libraries(SimplestHTTPServer PRIVATE ${CONAN_LIBS})