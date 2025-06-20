#include <Windows.h>
#include <ZenithEngine.h>

#include "TestApp.h"

int APIENTRY WinMain(_In_		HINSTANCE	hInstance,
	_In_opt_	HINSTANCE	hPrevInstance,
	_In_		LPSTR		lpCmdLine,
	_In_		int			nShowCmd)
{
	try
	{
		Zenith::ZenithApp* app = new TestApp();
		Zenith::AppManager::RunApplication(app);
		delete app;
	}
	catch (const Zenith::ZenithException& ex)
	{
		MessageBoxA(NULL, ex.what(), ex.GetType(), MB_ICONERROR);
	}
	catch (const Zenith::ZenithWException& ex)
	{
		MessageBoxW(NULL, ex.what_unicode(), ex.GetType(), MB_ICONERROR);
	}
	catch (const std::exception& ex)
	{
		MessageBoxA(NULL, ex.what(), "Standard Library Exception", MB_ICONERROR);
	}
	catch (...)
	{
		MessageBoxA(NULL, "No details", "Undefined Exception", MB_ICONERROR);
	}

	return Zenith::AppManager::GetReturnValue();
}
