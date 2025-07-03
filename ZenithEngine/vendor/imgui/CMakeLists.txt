cmake_minimum_required(VERSION 3.20)
project(imgui VERSION 1.92.0)

# Core Sources
set(IMGUI_SOURCES
	imgui.cpp
	imgui_demo.cpp
	imgui_draw.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
	imgui_impl_opengl3.cpp
	)

# Platform Specific Sources
if(WIN32)
	# For Windows platform, we need to include the Win32 and DirectX 11 implementations
	set(IMGUI_SOURCES 
		${IMGUI_SOURCES} 
		imgui_impl_win32.cpp
		imgui_impl_dx11.cpp)
else()
	# For non-Windows platforms, we include the GLFW implementation
	set(IMGUI_SOURCES ${IMGUI_SOURCES} imgui_impl_glfw.cpp)
endif()

add_library(${PROJECT_NAME} ${IMGUI_SOURCES})
