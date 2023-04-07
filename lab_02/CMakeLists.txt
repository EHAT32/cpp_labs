cmake_minimum_required(VERSION 3.0.0)

project(lab_2)

set(SOURCE_FILES src/main.cpp)

add_executable(main ${SOURCE_FILES})

target_include_directories(main PRIVATE include)