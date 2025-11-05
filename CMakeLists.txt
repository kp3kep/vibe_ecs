cmake_minimum_required(VERSION 4.0)
project(Engine_ECS)

set(CMAKE_CXX_STANDARD 26)

set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SFML-3.0.2/lib/cmake/SFML")

find_package(SFML 3.0.2 REQUIRED COMPONENTS System Window Graphics)

include_directories(include)
file(GLOB_RECURSE ENGINE
        Engine/Source/*.c++
        Game/*.c++
)

add_executable(Engine_ECS ${ENGINE} Game/Source/main.c++)

target_include_directories(Engine_ECS PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/Source"
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/Source/Core"
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/Source/ECS"
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine/Source/Systems"
        "${CMAKE_CURRENT_SOURCE_DIR}/Game/Source"
        "${CMAKE_CURRENT_SOURCE_DIR}/Game/Systems"
)

target_link_libraries(Engine_ECS PUBLIC
        SFML::Graphics
)