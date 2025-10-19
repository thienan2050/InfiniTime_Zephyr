cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(InfiniTime_Zephyr C)



target_include_directories(app PRIVATE 
    ../include/Screens
) 

target_sources(app PRIVATE
    src/main.c
)