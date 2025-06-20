cmake_minimum_required(VERSION 3.20)

project(ZenithEngine VERSION 0.0.1)

# Libraries
add_subdirectory(vendor/glad)
add_subdirectory(vendor/stb)
add_subdirectory(vendor/imgui)

# Source files
file (GLOB ENGINE_PROJECT_SOURCES	src/**.h
									src/Zenith/App/**.h
									src/Zenith/App/**.cpp
									src/Zenith/Audio/**.h
									src/Zenith/Audio/**.cpp
									src/Zenith/Renderer/**.h
									src/Zenith/Renderer/**.cpp
									src/Zenith/Renderer/UI/**.h
									src/Zenith/Renderer/UI/**.cpp
									src/Zenith/Utils/**.h
									src/Zenith/Utils/**.cpp
									src/Zenith/Input/**.h
									src/Zenith/Input/**.cpp
									src/Zenith/Platform/OpenGL/**.h
									src/Zenith/Platform/OpenGL/**.cpp
									src/Zenith/Platform/DirectX/**.h
									src/Zenith/Platform/DirectX/**.cpp)

add_library(${PROJECT_NAME} ${ENGINE_PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE	vendor/glad/include
													vendor/stb/include
													vendor/glm
													vendor/freetype/include
													vendor/imgui
													vendor/openal/include
													vendor/sndfile/include
													src/)

target_link_directories(${PROJECT_NAME} PRIVATE	vendor/freetype/lib
												vendor/openal/lib
												vendor/sndfile/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE	glad 
												stb
												opengl32
												imgui
												d3d11
												dxguid
												${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/freetype/lib/freetype.lib
												${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/openal/lib/OpenAL32.lib
												${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/sndfile/lib/sndfile.lib)
												
target_precompile_headers(${PROJECT_NAME} PRIVATE src/zenithpch.h)

# Post Build Commands
add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/res/
					${CMAKE_BINARY_DIR}/${PROJECT_NAME}/res/

					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/openal/dll/
					${CMAKE_BINARY_DIR}/${PROJECT_NAME}/dll/

					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/sndfile/dll/
					${CMAKE_BINARY_DIR}/${PROJECT_NAME}/dll/
					)

# Build HLSL Shaders
file (GLOB HLSL_VERTEX_SHADER_SOURCES	src/Zenith/Platform/DirectX/Shaders/**VS.hlsl)
file (GLOB HLSL_PIXEL_SHADER_SOURCES	src/Zenith/Platform/DirectX/Shaders/**PS.hlsl)
set(HLSL_SHADER_SOURCES	${HLSL_VERTEX_SHADER_SOURCES} ${HLSL_PIXEL_SHADER_SOURCES})

set_source_files_properties(${HLSL_VERTEX_SHADER_SOURCES} PROPERTIES ShaderType "vs")
set_source_files_properties(${HLSL_PIXEL_SHADER_SOURCES} PROPERTIES ShaderType "ps")
set_source_files_properties(${HLSL_SHADER_SOURCES} PROPERTIES ShaderModel "4_0")

set(COMPILED_SHADERS)

message(${CMAKE_BINARY_DIR})

foreach(SHADER ${HLSL_SHADER_SOURCES})
	get_filename_component(FILE_WE ${SHADER} NAME_WE)
	get_source_file_property(shadertype ${SHADER} ShaderType)
	get_source_file_property(shadermodel ${SHADER} ShaderModel)

	set(CSO_OUTPUT ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/res/shaders/${FILE_WE}.cso)
    set(PDB_OUTPUT ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/res/shaders/${FILE_WE}.pdb)
	
	add_custom_command(	OUTPUT ${CSO_OUTPUT}
						COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${CSO_OUTPUT} /Fd ${PDB_OUTPUT} ${SHADER}
						DEPENDS ${SHADER}
						COMMENT "Compiling HLSL Shader: ${SHADER}"
						VERBATIM)

	list(APPEND COMPILED_SHADERS ${CSO_OUTPUT})
endforeach(SHADER)

add_custom_target(ZenithShaders ALL DEPENDS ${COMPILED_SHADERS})
