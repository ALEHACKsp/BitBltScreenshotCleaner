#include "pch.h"
#include <iostream>
#include <windows.h>
#include "hook.h"
#include "SshotCleaner.h"
#include "Log.h"

//Overlay
SShotCleaner sshotCleaner(L"Star Wars Battlefront II", L"classname", L"windowname");

//Internal drawing, pass in your hack bDrawEnabled
//SShotCleaner sshotCleaner(L"Star Wars Battlefront II", &bDraw);

DWORD WINAPI HackThread(HMODULE hModule)
{
	if (sshotCleaner.Init())
	{
		sshotCleaner.Enable();
	}

	while (true)
	{
		Sleep(10);

		#ifdef _DEBUG

		//Turn the sshot cleaner off and on
		if (GetAsyncKeyState(VK_HOME) & 1)
		{
			sshotCleaner.Toggle();
		}

		//Test cleaning the screenshots
		if (GetAsyncKeyState(VK_NUMPAD9) & 1)
		{   
			sshotCleaner.SaveTestScreenshot(FindWindow(NULL, sshotCleaner.gameWindowTitle.c_str()));
		}
		#endif

		//disable & eject DLL
		if (GetAsyncKeyState(VK_END) & 1)
			break;
	}

	sshotCleaner.Disable();
	Sleep(500);
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}