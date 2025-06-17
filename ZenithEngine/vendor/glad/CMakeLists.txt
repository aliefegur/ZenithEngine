cmake_minimum_required(VERSION 3.20)
project(glad VERSION 4.6)

add_library(${PROJECT_NAME} src/glad.c src/glad_wgl.c)
target_include_directories(${PROJECT_NAME} PRIVATE "include")
