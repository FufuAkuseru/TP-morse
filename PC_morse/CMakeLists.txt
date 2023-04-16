cmake_minimum_required(VERSION 3.14)
set(CMAKE_COLOR_DIAGNOSTICS ON)
# set(CMAKE_C_STANDARD 90)
# set(CMAKE_C_FLAGS "-Wall -Wextra -Wconversion -ansi -pedantic")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra -Wconversion")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

project(computer_morse)

add_executable(morse main.c uart_msg.c uart_msg.h)
