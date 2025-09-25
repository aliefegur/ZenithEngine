#include <ZenithEngine.h>

#include "TestApp.h"

ENTRY_POINT
{
#if ZENITH_PLATFORM_WINDOWS
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
#else
	Zenith::ZenithApp* app = new TestApp();
	Zenith::AppManager::RunApplication(app);
	delete app;
#endif

	return Zenith::AppManager::GetReturnValue();
}
