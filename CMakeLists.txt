cmake_minimum_required(VERSION 3.12)
project(mages C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_JOB_COUNT 10)

# Specify the directory containing libraylib.a
link_directories(${CMAKE_SOURCE_DIR})

# Enable compiler optimizations and parallel build options
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native -flto")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
endif()

# Enable parallel builds if using Ninja or similar
set(CMAKE_JOB_POOLS "compile=${CMAKE_JOB_COUNT}")
set(CMAKE_JOB_POOL_COMPILE "compile")


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