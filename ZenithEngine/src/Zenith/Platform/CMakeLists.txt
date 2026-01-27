add_subdirectory(OpenGL)

if(WIN32)
	add_subdirectory(Windows)
	add_subdirectory(DirectX)
elseif(UNIX)
	add_subdirectory(Linux)
endif()
