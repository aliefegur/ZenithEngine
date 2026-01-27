target_sources(${PROJECT_NAME} PRIVATE	DxgiInfoManager.h
										DxgiInfoManager.cpp
										D3D11BatchRenderer.h
										D3D11BatchRenderer.cpp
										D3D11Bindable.h
										D3D11Bindable.cpp
										D3D11Graphics.h
										D3D11Graphics.cpp
										D3D11Renderer.h
										D3D11Renderer.cpp
										D3D11Shader.h
										D3D11Shader.cpp
										D3D11Texture2D.h
										D3D11Texture2D.cpp
										)

add_subdirectory(Shaders)

