#pragma once

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxerr.lib")

#include <d3d9.h>
#include <d3dx9.h>
#include "d3d9int.h"
#include "d3d9dev.h"

IDirect3D9 *APIENTRY hkDirect3DCreate9(UINT SDKVersion);

typedef IDirect3D9 *(APIENTRY *tDirect3DCreate9)(UINT);
extern tDirect3DCreate9 oDirect3DCreate9;
