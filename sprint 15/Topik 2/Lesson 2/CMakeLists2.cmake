cmake_minimum_required(VERSION 3.11)

project(Negate CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(negate  
            img_lib.h img_lib.cpp
            ppm_image.h ppm_image.cpp
            main.cpp)

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()

target_link_libraries(negate ${SYSTEM_LIBS})