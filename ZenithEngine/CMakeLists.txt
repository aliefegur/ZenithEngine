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
	set(PLATFORM_DEPENDENCIES	d3d11 
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

# Source files
file (GLOB ENGINE_PROJECT_SOURCES	src/**.h
									src/Zenith/App/**.h
									src/Zenith/App/**.cpp
									src/Zenith/Audio/**.h
									src/Zenith/Audio/**.cpp
									src/Zenith/Renderer/**.h
									src/Zenith/Renderer/**.cpp
									#src/Zenith/Renderer/UI/**.h
									#src/Zenith/Renderer/UI/**.cpp
									src/Zenith/Utils/**.h
									src/Zenith/Utils/**.cpp
									src/Zenith/Input/**.h
									src/Zenith/Input/**.cpp
									src/Zenith/Platform/OpenGL/**.h
									src/Zenith/Platform/OpenGL/**.cpp
									)
if(WIN32)
	file (GLOB PLATFORM_SOURCES	src/Zenith/Platform/DirectX/**.h
								src/Zenith/Platform/DirectX/**.cpp
								src/Zenith/Platform/Windows/**.h
								src/Zenith/Platform/Windows/**.cpp
								)
elseif(UNIX)
	file (GLOB PLATFORM_SOURCES	src/Zenith/Platform/Linux/**.h
								src/Zenith/Platform/Linux/**.cpp
								)
endif()

add_library(${PROJECT_NAME} ${ENGINE_PROJECT_SOURCES} ${PLATFORM_SOURCES})

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
												opengl32
												imgui												
												${PLATFORM_DEPENDENCIES}
												)
												
target_precompile_headers(${PROJECT_NAME} PRIVATE src/zenithpch.h)

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

# Build HLSL Shaders
if (WIN32)
	file (GLOB HLSL_VERTEX_SHADER_SOURCES	src/Zenith/Platform/DirectX/Shaders/**VS.hlsl)
	file (GLOB HLSL_PIXEL_SHADER_SOURCES	src/Zenith/Platform/DirectX/Shaders/**PS.hlsl)
	set(HLSL_SHADER_SOURCES	${HLSL_VERTEX_SHADER_SOURCES} ${HLSL_PIXEL_SHADER_SOURCES})

	set_source_files_properties(${HLSL_VERTEX_SHADER_SOURCES} PROPERTIES ShaderType "vs")
	set_source_files_properties(${HLSL_PIXEL_SHADER_SOURCES} PROPERTIES ShaderType "ps")
	set_source_files_properties(${HLSL_SHADER_SOURCES} PROPERTIES ShaderModel "4_0")

	set(COMPILED_SHADERS)
	foreach(SHADER ${HLSL_SHADER_SOURCES})
		get_filename_component(FILE_WE ${SHADER} NAME_WE)
		get_source_file_property(shadertype ${SHADER} ShaderType)
		get_source_file_property(shadermodel ${SHADER} ShaderModel)

		set(CSO_OUTPUT ${ZENITH_DIR}/res/shaders/${FILE_WE}.cso)
		set(PDB_OUTPUT ${ZENITH_DIR}/res/shaders/${FILE_WE}.pdb)
		
		add_custom_command(	OUTPUT ${CSO_OUTPUT}
							COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${CSO_OUTPUT} /Fd ${PDB_OUTPUT} ${SHADER}
							DEPENDS ${SHADER}
							COMMENT "Compiling HLSL Shader: ${SHADER}"
							VERBATIM)

		list(APPEND COMPILED_SHADERS ${CSO_OUTPUT})
	endforeach(SHADER)

	add_custom_target(ZenithShaders ALL DEPENDS ${COMPILED_SHADERS})
	add_dependencies(${PROJECT_NAME} ZenithShaders)
endif()

# Preprocessor definitions
if (WIN32)
	target_compile_definitions(${PROJECT_NAME} PRIVATE	"ZENITH_PLATFORM_WINDOWS=true"
														"ZENITH_PLATFORM_LINUX=false")
elseif (UNIX)
	target_compile_definitions(${PROJECT_NAME} PRIVATE	"ZENITH_PLATFORM_WINDOWS=false"
														"ZENITH_PLATFORM_LINUX=true")
endif ()
