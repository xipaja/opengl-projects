cmake_minimum_required(VERSION 3.30) 

project(opengl_projects)

include_directories(include)
file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.c")

find_package(SDL3 REQUIRED CONFIG)
find_package(assimp REQUIRED CONFIG)

add_executable(output ${SRC_FILES})

target_link_libraries(output SDL3::SDL3 assimp::assimp) 