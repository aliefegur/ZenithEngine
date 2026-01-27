file (GLOB HLSL_VERTEX_SHADER_SOURCES	./**VS.hlsl)
file (GLOB HLSL_PIXEL_SHADER_SOURCES	./**PS.hlsl)
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
