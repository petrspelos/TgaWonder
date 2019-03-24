cmake_minimum_required(VERSION 3.14)
project(TgaWonder C)

set(CMAKE_C_STANDARD 11)

add_executable(TgaWonder main.c)

target_link_libraries(TgaWonder m)
