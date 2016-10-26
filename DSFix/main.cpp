

#include <windows.h>
#include <fstream>
#include <ostream>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <strsafe.h>

#include "main.h"
#include "d3d9.h"
#include "d3dutil.h"
#include "Settings.h"
#include "KeyActions.h"
#include "Detouring.h"
#include "SaveManager.h"
#include "FPS.h"

// globals
tDirectInput8Create oDirectInput8Create;
char dlldir[MAX_PATH];
char fileName[MAX_PATH];

BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, PVOID pvReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		GetModuleFileName(NULL, fileName, MAX_PATH);
		DisableThreadLibraryCalls(hDll);
		GetModuleFileName(hDll, dlldir, MAX_PATH);
		for (int i = strlen(dlldir); i > 0; i--)
		{
			if (dlldir[i] == '\\')
			{
				dlldir[i + 1] = 0;
				break;
			}
		}
#ifndef RELEASE_VER
		LogFile(GetDirectoryFile("DSfix.log"));
#endif
		SDLOG(0, "===== start DSfix %s = fn: %s", VERSION, fileName);

		// load settings
		Settings::get().load();
		KeyActions::get().load();

#ifndef RELEASE_VER
		Settings::get().report();
		KeyActions::get().report();
#endif

		startDetour();

		if (Settings::get().getUnlockFPS())
			applyFPSPatch();

		break;
	case DLL_PROCESS_DETACH:
		endDetour();
		SDLOG(0, "===== end = fn: %s", fileName);
		break;
	}

	return TRUE;
}

const char *GetDirectoryFile(char *filename)
{
	static char path[MAX_PATH];
	strcpy_s(path, dlldir);
	strcat_s(path, filename);
	return path;
}
