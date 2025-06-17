cmake_minimum_required(VERSION 3.20)
project(imgui VERSION 4.6)

add_library(	${PROJECT_NAME}	imgui.cpp imgui_demo.cpp 
				imgui_draw.cpp imgui_impl_opengl3.cpp 
				imgui_impl_win32.cpp imgui_tables.cpp 
				imgui_widgets.cpp
				)
