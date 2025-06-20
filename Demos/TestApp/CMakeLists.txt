cmake_minimum_required(VERSION 3.20)

project(TestApp VERSION 1.0.0)

file(GLOB TEST_APP_SRC	src/**.h
						src/**.cpp
						src/**.rc)

add_executable(${PROJECT_NAME} WIN32 ${TEST_APP_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE	${CMAKE_SOURCE_DIR}/ZenithEngine/src
													${CMAKE_SOURCE_DIR}/ZenithEngine/vendor/glm
													${CMAKE_SOURCE_DIR}/ZenithEngine/vendor/imgui
													${CMAKE_SOURCE_DIR}/ZenithEngine/vendor/openal/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ZenithEngine opengl32 d3dcompiler)

# Post Build Commands
add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/Demos/${PROJECT_NAME}/res/
					${CMAKE_BINARY_DIR}/Demos/${PROJECT_NAME}/res/

					TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_BINARY_DIR}/ZenithEngine/res/
					${CMAKE_BINARY_DIR}/Demos/${PROJECT_NAME}/res/

					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_BINARY_DIR}/ZenithEngine/dll/
					${CMAKE_BINARY_DIR}/Demos/${PROJECT_NAME}/
					)
