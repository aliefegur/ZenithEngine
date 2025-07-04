#include "Zenith/API.h"

#include <cstdlib>
#include <cstdint>
#include <chrono>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <memory>
#include <random>

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include <glad/glad.h>
#include <glad/glad_wgl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#ifdef ZENITH_PLATFORM_WINDOWS
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#else
#include <imgui_impl_glfw.h>
#endif

#include <stb/stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H
