cmake_minimum_required(VERSION 3.20)

project(ZenithEngine VERSION 0.0.1)

# Zenith output directory for deriving projects
if (WIN32)
	set(ZENITH_DIR ${PROJECT_BINARY_DIR}/${CMAKE_CONFIGURATION_TYPES})
elseif (UNIX)
	set(ZENITH_DIR ${PROJECT_BINARY_DIR})
endif ()

# Libraries
add_subdirectory(vendor/glad)
add_subdirectory(vendor/stb)
add_subdirectory(vendor/imgui)
if(UNIX)
	find_package(glfw3 3.4 REQUIRED)
	find_package(OpenAL REQUIRED)
	find_package(Freetype REQUIRED)
	pkg_check_modules(SNDFILE REQUIRED sndfile)
endif()

if (WIN32)
	set(PLATFORM_DEPENDENCIES	opengl32
								d3d11 
								dxguid 
								${PROJECT_SOURCE_DIR}/vendor/freetype/lib/freetype.lib
								${PROJECT_SOURCE_DIR}/vendor/openal/lib/OpenAL32.lib
								${PROJECT_SOURCE_DIR}/vendor/sndfile/lib/sndfile.lib
								)
	set(PLATFORM_INCLUDES	vendor/openal/include
							vendor/sndfile/include
							vendor/freetype/include
							)
elseif(UNIX)
	set(PLATFORM_DEPENDENCIES	glfw
								OpenAL::OpenAL
								Freetype::Freetype
								${SNDFILE_LIBRARIES})
	set(PLATFORM_INCLUDES	${SNDFILE_INCLUDE_DIRS})
endif()

add_library(${PROJECT_NAME})
add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PRIVATE	vendor/glad/include
													vendor/stb/include
													vendor/glm
													vendor/imgui
													src/
													${PLATFORM_INCLUDES}
													)

if (WIN32)
target_link_directories(${PROJECT_NAME} PRIVATE	vendor/freetype/lib
												vendor/openal/lib
												vendor/sndfile/lib
												)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE	glad 
												stb
												imgui												
												${PLATFORM_DEPENDENCIES}
												)

# Post Build Commands
add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/res/
					${ZENITH_DIR}/res/

					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/openal/dll/
					${ZENITH_DIR}/dll/

					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/sndfile/dll/
					${ZENITH_DIR}/dll/
					)

# Preprocessor definitions
if (WIN32)
	target_compile_definitions(${PROJECT_NAME} PUBLIC	"ZENITH_PLATFORM_WINDOWS=true"
														"ZENITH_PLATFORM_LINUX=false")
elseif (UNIX)
	target_compile_definitions(${PROJECT_NAME} PUBLIC	"ZENITH_PLATFORM_WINDOWS=false"
														"ZENITH_PLATFORM_LINUX=true")
endif ()
