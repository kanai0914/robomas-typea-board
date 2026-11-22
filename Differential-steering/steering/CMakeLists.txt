cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(steering STATIC
    src/app.cpp
)

target_include_directories(steering PUBLIC
    include     
)
target_link_libraries(steering PRIVATE stm32cubemx)

