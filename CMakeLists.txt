cmake_minimum_required(VERSION 4.0)
project(Engine_ECS)

set(CMAKE_CXX_STANDARD 26)

# Убрали set(SFML_DIR...)
# Убрали версию "3.0.2"
find_package(SFML REQUIRED COMPONENTS system window graphics)

# Сказали, где лежат .h++
include_directories(${SFML_INCLUDE_DIR})

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

# ----------------- ГЛАВНОЕ ИЗМЕНЕНИЕ -----------------
# Линкуем не ${SFML_GRAPHICS_LIBRARY} и т.д.,
# а ОДНУ "общую" переменную, которую заполнил FindSFML
target_link_libraries(Engine_ECS PUBLIC
        sfml-graphics
        sfml-window
        sfml-system)