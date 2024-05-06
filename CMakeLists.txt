cmake_minimum_required(VERSION 3.27)
project(mages C)

set(CMAKE_C_STANDARD 17)

# Specify the directory containing libraylib.a
link_directories(${CMAKE_SOURCE_DIR})

add_executable(mages main.c)

target_link_libraries(mages PRIVATE
        ${CMAKE_DL_LIBS}
        m
        pthread
        GL
        X11
        rt
        raylib
)