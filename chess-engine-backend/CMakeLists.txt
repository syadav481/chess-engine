cmake_minimum_required(VERSION 3.25.1)
project(chess-engine-backend)

add_executable(engine_backend 
    main.cpp 
)

find_package(nlohmann_json 3.11.3 REQUIRED)
target_link_libraries(engine_backend PRIVATE nlohmann_json::nlohmann_json)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)
