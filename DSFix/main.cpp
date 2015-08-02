#include "stdafx.h"

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
char dlldir[320];
TCHAR fileName[512];

BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, PVOID pvReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        GetModuleFileName(NULL, fileName, 512);
        DisableThreadLibraryCalls(hDll);
        GetModuleFileName(hDll, dlldir, 512);
        for (int i = strlen(dlldir); i > 0; i--) { if (dlldir[i] == '\\') { dlldir[i + 1] = 0; break; } }
        LogFile(GetDirectoryFile("DSfix.log"));
        SDLOG(0, "===== start DSfix %s = fn: %s\n", VERSION, fileName);

        // load settings
        Settings::get().load();
        Settings::get().report();

        KeyActions::get().load();
        KeyActions::get().report();

        SaveManager::get().init();

        earlyDetour();

        if (Settings::get().getUnlockFPS()) applyFPSPatch();
        break;
    case DLL_PROCESS_DETACH:
        Settings::get().shutdown();
        endDetour();
        SDLOG(0, "===== end = fn: %s\n", fileName);
        break;
    }

    return TRUE;
}

const char *GetDirectoryFile(char *filename)
{
	static char path[320];
	strcpy_s(path, dlldir);
	strcat_s(path, filename);
	return path;
}
