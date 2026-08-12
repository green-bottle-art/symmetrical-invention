cmake_minimum_required(VERSION 3.16)

project(Game)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

add_executable(game main.cpp)

target_link_libraries(game PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)