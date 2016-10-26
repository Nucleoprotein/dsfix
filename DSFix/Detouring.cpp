

#include "Detouring.h"
#include "Settings.h"
#include "RenderstateManager.h"

bool timingIntroMode = false;

DWORD WINAPI DetouredSleepEx(DWORD dwMilliseconds, BOOL bAlertable)
{
	//SDLOG(12, "T %6lu: Detouring: Sleep for %lu ms", GetCurrentThreadId(), dwMilliseconds);
	//return TrueSleepEx(dwMilliseconds, bAlertable);
	return 0;
}

#include <mmsystem.h>

static DWORD timeIncrease;
DWORD(WINAPI * TrueTimeGetTime)(void) = timeGetTime;
DWORD WINAPI DetouredTimeGetTime()
{
	SDLOG(13, "T %6lu: Detouring: TimeGetTime - real: %10lu, returned: %10lu", GetCurrentThreadId(), TrueTimeGetTime(), TrueTimeGetTime() + timeIncrease);
	//timeIncrease += 16;
	return TrueTimeGetTime() + timeIncrease;
}

static LARGE_INTEGER countsPerSec;
BOOL(WINAPI * TrueQueryPerformanceCounter)(_Out_ LARGE_INTEGER *lpPerformanceCount) = QueryPerformanceCounter;
BOOL WINAPI DetouredQueryPerformanceCounter(_Out_ LARGE_INTEGER *lpPerformanceCount)
{
	BOOL ret = TrueQueryPerformanceCounter(lpPerformanceCount);
	if (timingIntroMode)
	{
		lpPerformanceCount->QuadPart += countsPerSec.QuadPart / 50;
	}
	else
	{
		static size_t count;
		if (count > 10)
			MH_DisableHook(QueryPerformanceCounter);
		++count;
	}
	return ret;
}

typedef HRESULT(WINAPI * D3DXCreateTextureFromFileInMemory_FNType)(_In_ LPDIRECT3DDEVICE9 pDevice, _In_ LPCVOID pSrcData, _In_ UINT SrcDataSize, _Out_ LPDIRECT3DTEXTURE9 *ppTexture);
D3DXCreateTextureFromFileInMemory_FNType TrueD3DXCreateTextureFromFileInMemory = D3DXCreateTextureFromFileInMemory;
HRESULT WINAPI DetouredD3DXCreateTextureFromFileInMemory(_In_ LPDIRECT3DDEVICE9 pDevice, _In_ LPCVOID pSrcData, _In_ UINT SrcDataSize, _Out_ LPDIRECT3DTEXTURE9 *ppTexture)
{
	SDLOG(4, "DetouredD3DXCreateTextureFromFileInMemory");
	HRESULT res = TrueD3DXCreateTextureFromFileInMemory(pDevice, pSrcData, SrcDataSize, ppTexture);
	RSManager::get().registerD3DXCreateTextureFromFileInMemory(pSrcData, SrcDataSize, *ppTexture);
	return res;
}

typedef HRESULT(WINAPI * D3DXCreateTextureFromFileInMemoryEx_FNType)(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture);
D3DXCreateTextureFromFileInMemoryEx_FNType TrueD3DXCreateTextureFromFileInMemoryEx = D3DXCreateTextureFromFileInMemoryEx;
HRESULT WINAPI DetouredD3DXCreateTextureFromFileInMemoryEx(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter,
        DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture)
{
	SDLOG(4, "DetouredD3DXCreateTextureFromFileInMemoryEx");
	HRESULT res = RSManager::get().redirectD3DXCreateTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, Width, Height, MipLevels, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
	RSManager::get().registerD3DXCreateTextureFromFileInMemory(pSrcData, SrcDataSize, *ppTexture);
	return res;
}

typedef HRESULT(WINAPI * D3DXCompileShader_FNType)(_In_ LPCSTR pSrcData, _In_ UINT srcDataLen, _In_ const D3DXMACRO *pDefines, _In_ LPD3DXINCLUDE pInclude, _In_ LPCSTR pFunctionName, _In_ LPCSTR pProfile, _In_ DWORD Flags, _Out_ LPD3DXBUFFER *ppShader, _Out_ LPD3DXBUFFER *ppErrorMsgs, _Out_ LPD3DXCONSTANTTABLE *ppConstantTable);
D3DXCompileShader_FNType TrueD3DXCompileShader = D3DXCompileShader;
HRESULT WINAPI DetouredD3DXCompileShader(_In_ LPCSTR pSrcData, _In_ UINT srcDataLen, _In_ const D3DXMACRO *pDefines, _In_ LPD3DXINCLUDE pInclude, _In_ LPCSTR pFunctionName, _In_ LPCSTR pProfile,
        _In_ DWORD Flags, _Out_ LPD3DXBUFFER *ppShader, _Out_ LPD3DXBUFFER *ppErrorMsgs, _Out_ LPD3DXCONSTANTTABLE *ppConstantTable)
{
	HRESULT res = TrueD3DXCompileShader(pSrcData, srcDataLen, pDefines, pInclude, pFunctionName, pProfile, Flags, ppShader, ppErrorMsgs, ppConstantTable);
	RSManager::get().registerD3DXCompileShader(pSrcData, srcDataLen, pDefines, pInclude, pFunctionName, pProfile, Flags, ppShader, ppErrorMsgs, ppConstantTable);
	return res;
}

static LANGID s_langid;

LANGID(WINAPI* TrueGetUserDefaultLangID)(void);
LANGID WINAPI DetouredGetUserDefaultLangID(void)
{
	if (s_langid)
		return s_langid;
	else
		return TrueGetUserDefaultLangID();
}

void startDetour()
{
	QueryPerformanceFrequency(&countsPerSec);

	MH_Initialize();

	MH_CreateHook(Direct3DCreate9, hkDirect3DCreate9, reinterpret_cast<void**>(&oDirect3DCreate9));

	if (Settings::get().getSkipIntro())
	{
		MH_CreateHook(QueryPerformanceCounter, DetouredQueryPerformanceCounter, reinterpret_cast<void**>(&TrueQueryPerformanceCounter));
	}

	if (Settings::get().getOverrideLanguage().length() >= 2 && Settings::get().getOverrideLanguage().find("none") != 0)
	{
		std::wstring langName = UTF8ToUTF16(Settings::get().getOverrideLanguage());
		LCID lcid = LocaleNameToLCID(langName.c_str(), 0); // Vista+ only, but I do not care about XP
		if (lcid)
		{
			s_langid = LANGIDFROMLCID(lcid);

			MH_CreateHook(GetUserDefaultLangID, DetouredGetUserDefaultLangID, reinterpret_cast<void**>(&TrueGetUserDefaultLangID));
			MH_EnableHook(GetUserDefaultLangID);
		}
	}

	MH_CreateHook(D3DXCreateTextureFromFileInMemory, DetouredD3DXCreateTextureFromFileInMemory, reinterpret_cast<void**>(&TrueD3DXCreateTextureFromFileInMemory));
	MH_CreateHook(D3DXCreateTextureFromFileInMemoryEx, DetouredD3DXCreateTextureFromFileInMemoryEx, reinterpret_cast<void**>(&TrueD3DXCreateTextureFromFileInMemoryEx));

	if (MH_EnableHook(MH_ALL_HOOKS) == MH_OK)
	{
		SDLOG(0, "Detouring: Detoured successfully");
	}
	else
	{
		SDLOG(0, "Detouring: Error detouring");
	}
}

void endDetour()
{
	MH_Uninitialize();
}
