cmake_minimum_required(VERSION 3.20)
project(anatomy_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSION OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(anatomy_cpp
  src/main.cpp
  src/compute.cpp
)

target_compile_options(anatomy_cpp PRIVATE
  -Wall
  -Wextra
  -Wpedantic
  -Wshadow
  -Wconversion
  -Wsign-conversion
)
