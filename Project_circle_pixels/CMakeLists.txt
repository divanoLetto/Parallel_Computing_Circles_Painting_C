cmake_minimum_required(VERSION 3.15)
project(Project_circle_pixels)

set(CMAKE_CXX_STANDARD 14)

add_executable(Project_circle_pixels main.cpp MyCircle.h pixel_parallel.h pixel_parallel_layers2.h pixel_seq.h)

include_directories("/usr/include/SFML")
target_link_libraries(Project_circle_pixels "/usr/lib/x86_64-linux-gnu/libsfml-graphics.so" )
target_link_libraries(Project_circle_pixels "/usr/lib/x86_64-linux-gnu/libsfml-system.so" )
target_link_libraries(Project_circle_pixels "/usr/lib/x86_64-linux-gnu/libsfml-window.so" )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Project_circle_pixels Threads::Threads)