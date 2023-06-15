cmake_minimum_required(VERSION 3.25)
project(E3_Objetos)

set(CMAKE_CXX_STANDARD 17)
include_directories(./../../../include/)
set(CMAKE_CXX_FLAGS -I/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_executable(E3_Objetos main.cpp glut_ply.cpp Objeto.cpp ./../../../include/glad.c BoundingVolume.h)

target_link_libraries(E3_Objetos glfw.3)