cmake_minimum_required(VERSION 3.20)
project(glad VERSION 4.6)

if (WIN32)
	add_library(${PROJECT_NAME} src/glad.c src/glad_wgl.c)
elseif (UNIX)
	add_library(${PROJECT_NAME} src/glad.c)
endif ()
target_include_directories(${PROJECT_NAME} PRIVATE "include")
