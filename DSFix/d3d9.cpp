#include "stdafx.h"

#include <windows.h>

#include "d3d9.h"
#include "main.h"
#include "Settings.h"

tDirect3DCreate9 oDirect3DCreate9;
IDirect3D9 *APIENTRY hkDirect3DCreate9(UINT SDKVersion)
{
    PrintLog("hkDirect3DCreate9\n");

    IDirect3D9 *d3dint = NULL;
    d3dint = oDirect3DCreate9(SDKVersion);

    if (d3dint) {       
        new hkIDirect3D9(&d3dint);
    }
    return d3dint;
}
