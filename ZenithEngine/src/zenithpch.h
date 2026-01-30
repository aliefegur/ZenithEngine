// Standard libs
#include <cstdlib>
#include <cstdint>
#include <chrono>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <unordered_map>
#include <memory>

// Windows
#if ZENITH_PLATFORM_WINDOWS
#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#endif

// GLAD
#include <glad/glad.h>
#if ZENITH_PLATFORM_WINDOWS
#include <glad/glad_wgl.h>
#endif

// GLFW
#if ZENITH_PLATFORM_LINUX
#include <GLFW/glfw3.h>
#endif

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ImGui
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#if ZENITH_PLATFORM_WINDOWS
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#else
#include <imgui_impl_glfw.h>
#endif

#include <stb/stb_image.h>
