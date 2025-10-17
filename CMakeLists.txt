cmake_minimum_required(VERSION 4.0)
project(Engine_ECS)

set(CMAKE_CXX_STANDARD 26)

include_directories(include)
file(GLOB_RECURSE ENGINE Source/*.c++)

add_executable(Engine_ECS ${ENGINE} main.c++)

target_include_directories(Engine_ECS PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/Source"
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/Source/Systems"
        "${CMAKE_CURRENT_SOURCE_DIR}/Game"
)