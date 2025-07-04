# Zenith Engine
ZenithEngine is an open source game engine developed with C++. Zenith Engine involves comprehensive graphics rendering, audio management, input system modules. Developers can create and optimize varying game projects using the Zenith Engine.

## Features
* **Cross Graphics API Support**: Zenith Engine supports both OpenGL 4.6 and Direct3D11 APIs. *(Under development)*
* **Mathematical Operations**: High performance vector, matrix and 3D calculations using GLM library.
* **Audio System**: Audio management with OpenAL.
* **Asset Management**: Managing game resources with a user-friendly asset management system. *(Under Development)*
* **Window Management**: Provides window management and user interaction using Win32 API.

## Dependencies
* **Freetype**: Loads fonts to render text.
* **GLAD**: Loads modern OpenGL functions.
* **GLM**: High-performance math library.
* **ImGui**: Development and debugging Immediate GUI.
* **OpenAL**: Audio backend.
* **sndfile**: Loads audio resource files.
* **stb**: Loads image files to create textures.

## Build Instructions
1. **Clone the repository**:
   ```cmd
   git clone https://github.com/aliefegur/ZenithEngine.git
	```
2. **CMake Build**
   ```cmd
   cd ZenithEngine
   mkdir build
   cd build
   cmake .. -DCMAKE_CONFIGURATION_TYPES=<ConfigType>
   ```
   `<ConfigType>` can be `Debug` or `Release`
3. **Compile The C++ Project**
   ```cmd
   cmake --build .
   ```

After performing these steps; the library file, resources and dll files will be located in the `ZenithEngine/<ConfigType>` directory.
