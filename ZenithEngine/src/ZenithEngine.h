#include "Zenith/API.h"

#include "Zenith/App/AppManager.h"
#include "Zenith/App/Window.h"
#if ZENITH_PLATFORM_WINDOWS
#include "Zenith/Platform/Windows/Win32Window.h
#elif ZENITH_PLATFORM_LINUX
#include "Zenith/Platform/Linux/LinuxWindow.h"
#endif

#include "Zenith/Audio/AudioClip.h"
#include "Zenith/Audio/AudioDevice.h"
#include "Zenith/Audio/AudioSource.h"

#include "Zenith/Input/Key.h"
#include "Zenith/Input/Input.h"
#include "Zenith/Input/EventListener.h"

#include "Zenith/Renderer/BatchRenderer.h"
#include "Zenith/Renderer/Camera.h"
#include "Zenith/Renderer/Stats.h"

#include "Zenith/Renderer/UI/UIRenderer.h"
#include "Zenith/Renderer/UI/ImGuiManager.h"

#include "Zenith/Utils/Time.h"
#include "Zenith/Utils/File.h"

#include <imgui.h>

#if ZENITH_PLATFORM_WINDOWS
#include <Windows.h>
#define ENTRY_POINT int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
#else
#define ENTRY_POINT int main(int argc, char** argv)
#endif
