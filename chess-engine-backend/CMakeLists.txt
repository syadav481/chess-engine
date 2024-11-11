cmake_minimum_required(VERSION 3.25.1)
project(chess-engine-backend)

add_executable(engine_backend 
    main.cpp 
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)
