/*	Direct3D9 Interface */



#include <windows.h>
#include "main.h"
#include "d3d9.h"
#include "d3dutil.h"

#include "Settings.h"
#include "RenderstateManager.h"

HRESULT APIENTRY hkIDirect3D9::QueryInterface(REFIID riid, void **ppvObj)
{
	SDLOG(1, "hkIDirect3D9::QueryInterface");
	return m_pD3Dint->QueryInterface(riid, ppvObj);
}

ULONG APIENTRY hkIDirect3D9::AddRef()
{
	return m_pD3Dint->AddRef();
}

HRESULT APIENTRY hkIDirect3D9::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
	return m_pD3Dint->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT APIENTRY hkIDirect3D9::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
	return m_pD3Dint->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT APIENTRY hkIDirect3D9::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
{
	return m_pD3Dint->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
}

HRESULT APIENTRY hkIDirect3D9::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels)
{
	return m_pD3Dint->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
}

HRESULT APIENTRY hkIDirect3D9::CheckDeviceType(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed)
{
	return m_pD3Dint->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed);
}

HRESULT APIENTRY hkIDirect3D9::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice9 **ppReturnedDeviceInterface)
{
	SDLOG(0, "CreateDevice ------ Adapter %u", Adapter);

	if (Settings::get().getD3DAdapterOverride() >= 0)
	{
		SDLOG(0, " - Adapter override to %d", Settings::get().getD3DAdapterOverride());
		Adapter = Settings::get().getD3DAdapterOverride();
	}

	// BehaviorFlags -> D3DCREATE_DISABLE_DRIVER_MANAGEMENT_EX D3DCREATE_HARDWARE_VERTEXPROCESSING D3DCREATE_MULTITHREADED
	if (BehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING && !(BehaviorFlags & D3DCREATE_PUREDEVICE))
	{
		BehaviorFlags |= D3DCREATE_PUREDEVICE;
	}

	if (!pPresentationParameters)
	{
		return m_pD3Dint->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
	}

	D3DPRESENT_PARAMETERS adjusted = RSManager::get().adjustPresentationParameters(pPresentationParameters);
	HRESULT hRet;

	hRet = m_pD3Dint->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, &adjusted, ppReturnedDeviceInterface);

	if (SUCCEEDED(hRet) && ppReturnedDeviceInterface)
	{
		new hkIDirect3DDevice9(ppReturnedDeviceInterface, &adjusted, this);
	}
	return hRet;
}

HRESULT APIENTRY hkIDirect3D9::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode)
{
	return m_pD3Dint->EnumAdapterModes(Adapter, Format, Mode, pMode);
}

UINT APIENTRY hkIDirect3D9::GetAdapterCount()
{
	return m_pD3Dint->GetAdapterCount();
}

HRESULT APIENTRY hkIDirect3D9::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE *pMode)
{
	return m_pD3Dint->GetAdapterDisplayMode(Adapter, pMode);
}

HRESULT APIENTRY hkIDirect3D9::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9 *pIdentifier)
{
	return m_pD3Dint->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

UINT APIENTRY hkIDirect3D9::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
{
	return m_pD3Dint->GetAdapterModeCount(Adapter, Format);
}

HMONITOR APIENTRY hkIDirect3D9::GetAdapterMonitor(UINT Adapter)
{
	return m_pD3Dint->GetAdapterMonitor(Adapter);
}

HRESULT APIENTRY hkIDirect3D9::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9 *pCaps)
{
	return m_pD3Dint->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HRESULT APIENTRY hkIDirect3D9::RegisterSoftwareDevice(void *pInitializeFunction)
{
	return m_pD3Dint->RegisterSoftwareDevice(pInitializeFunction);
}

ULONG APIENTRY hkIDirect3D9::Release()
{
	ULONG refs = m_pD3Dint->Release();
	if (!refs)
		delete this;
	return refs;
}