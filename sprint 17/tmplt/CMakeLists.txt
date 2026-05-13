cmake_minimum_required(VERSION 3.15)

project(BoostAsio LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключаем Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Основной исполняемый файл
add_executable(BoostAsio src/main.cpp)

# Линкуем с Boost (включая boost_system)
target_link_libraries(BoostAsio PRIVATE ${CONAN_LIBS})