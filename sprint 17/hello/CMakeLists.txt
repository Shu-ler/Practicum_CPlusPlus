cmake_minimum_required(VERSION 3.11)

project(Hello CXX)
set(CMAKE_CXX_STANDARD 20)

# Подключаем скрипт от Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Поддержка потоков (на всякий случай)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Основной исполняемый файл
add_executable(hello src/main.cpp)

# Подключаем библиотеки: потоки + все от Conan (включая Boost)
target_link_libraries(hello PRIVATE Threads::Threads ${CONAN_LIBS})