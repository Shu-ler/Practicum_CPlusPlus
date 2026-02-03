cmake_minimum_required(VERSION 3.11)

project(Negate CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Файлы библиотеки изображений
set(IMGLIB_FILES
    img_lib.h
    img_lib.cpp
    ppm_image.h
    ppm_image.cpp
)

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()

# Цель: hmirror
add_executable(hmirror
    hmirr.cpp
    ${IMGLIB_FILES}
)
target_link_libraries(hmirror ${SYSTEM_LIBS})

# Цель: vmirror
add_executable(vmirror
    vmirr.cpp
    ${IMGLIB_FILES}
)
target_link_libraries(vmirror ${SYSTEM_LIBS})

# Цель: sobel
add_executable(sobel
    sobel.cpp
    ${IMGLIB_FILES}
)
target_link_libraries(sobel ${SYSTEM_LIBS})