

#include <windows.h>
#include <fstream>
#include <ostream>
#include <iostream>
#include <list>
#include "main.h"
#include "d3dutil.h"
#include "RenderstateManager.h"

hkIDirect3DDevice9::hkIDirect3DDevice9(IDirect3DDevice9 **ppReturnedDeviceInterface, D3DPRESENT_PARAMETERS *pPresentParam, IDirect3D9 *pIDirect3D9)
{
	SDLOG(0, "hkIDirect3DDevice9");
	m_pD3Ddev = *ppReturnedDeviceInterface;
	m_pD3Dint = pIDirect3D9;
	RSManager::get().setD3DDevice(m_pD3Ddev);
	RSManager::get().initResources();

	*ppReturnedDeviceInterface = this;
}

HRESULT APIENTRY hkIDirect3DDevice9::Present(CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion)
{
	SDLOG(3, "!!!!!!!!!!!!!!!!!!!!!!! Present !!!!!!!!!!!!!!!!!!");
	return RSManager::get().redirectPresent(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	return m_pD3Ddev->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
	if (RenderTargetIndex != 0) return D3D_OK; // rendertargets > 0 are not actually used by the game - this makes the log shorter
	SDLOG(3, "SetRenderTarget %5d, %p", RenderTargetIndex, pRenderTarget);
	return RSManager::get().redirectSetRenderTarget(RenderTargetIndex, pRenderTarget);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9* pvShader)
{
	SDLOG(7, "SetVertexShader: %p", pvShader);
	return m_pD3Ddev->SetVertexShader(pvShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9 *pViewport)
{
	SDLOG(6, "SetViewport X / Y - W x H : %4lu / %4lu  -  %4lu x %4lu", pViewport->X, pViewport->Y, pViewport->Width, pViewport->Height);
	RSManager::get().setViewport(*pViewport);
	return m_pD3Ddev->SetViewport(pViewport);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	SDLOG(9, "DrawIndexedPrimitive");
	return m_pD3Ddev->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT PrimitiveCount, CONST void *pIndexData, D3DFORMAT IndexDataFormat, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	SDLOG(9, "DrawIndexedPrimitiveUP(%d, %u, %u, %u, %u, %p, %d, %p, %d)", PrimitiveType, MinIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
	return RSManager::get().redirectDrawIndexedPrimitiveUP(PrimitiveType, MinIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	SDLOG(9, "DrawPrimitive");
	return m_pD3Ddev->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	SDLOG(9, "DrawPrimitiveUP(%d, %u, %u, %u, %u, %p, %d, %p, %d)", PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
	return RSManager::get().redirectDrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawRectPatch(UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo)
{
	SDLOG(9, "DrawRectPatch");
	return m_pD3Ddev->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawTriPatch(UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo)
{
	SDLOG(9, "DrawTriPatch");
	return m_pD3Ddev->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	return m_pD3Ddev->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

HRESULT APIENTRY hkIDirect3DDevice9::EndScene()
{
	SDLOG(7, "EndScene");
	return m_pD3Ddev->EndScene();
}

HRESULT APIENTRY hkIDirect3DDevice9::QueryInterface(REFIID riid, LPVOID *ppvObj)
{
	return m_pD3Ddev->QueryInterface(riid, ppvObj);
}

ULONG APIENTRY hkIDirect3DDevice9::AddRef()
{
	return m_pD3Ddev->AddRef();
}

HRESULT APIENTRY hkIDirect3DDevice9::BeginScene()
{
	SDLOG(7, "BeginScene");
	return m_pD3Ddev->BeginScene();
}

HRESULT APIENTRY hkIDirect3DDevice9::BeginStateBlock()
{
	return m_pD3Ddev->BeginStateBlock();
}

HRESULT APIENTRY hkIDirect3DDevice9::Clear(DWORD Count, CONST D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
	return m_pD3Ddev->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT APIENTRY hkIDirect3DDevice9::ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{
	return m_pD3Ddev->ColorFill(pSurface, pRect, color);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DSwapChain9 **ppSwapChain)
{
	return m_pD3Ddev->CreateAdditionalSwapChain(pPresentationParameters, ppSwapChain);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
{
	return m_pD3Ddev->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	SDLOG(4, "CreateDepthStencilSurface w/h: %4u/%4u  format: %s", Width, Height, D3DFormatToString(Format));
	if (Width == 1024 && Height == 720)
	{
		SDLOG(4, " - OVERRIDE to %4u/%4u!", Settings::get().getRenderWidth(), Settings::get().getRenderHeight());
		return m_pD3Ddev->CreateDepthStencilSurface(Settings::get().getRenderWidth(), Settings::get().getRenderHeight(), Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
	}
	return m_pD3Ddev->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
	return m_pD3Ddev->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	return m_pD3Ddev->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
	HRESULT res = m_pD3Ddev->CreatePixelShader(pFunction, ppShader);
	if (Settings::get().getLogLevel() > 12)
	{
		SDLOG(12, "CreatePixelShader data: %p : shader: %p", pFunction, *ppShader);
		LPD3DXBUFFER buffer;
		D3DXDisassembleShader(pFunction, false, NULL, &buffer);
		SDLOG(12, "===== disassembly:\n%s\n==============", buffer->GetBufferPointer());
	}
	return res;
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
	return m_pD3Ddev->CreateQuery(Type, ppQuery);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	SDLOG(1, "CreateRenderTarget w/h: %4u/%4u  format: %s", Width, Height, D3DFormatToString(Format));
	if (Width == 1024 && Height == 720)
	{
		SDLOG(1, " - OVERRIDE to %4u/%4u!", Settings::get().getRenderWidth(), Settings::get().getRenderHeight());
		HRESULT hr = m_pD3Ddev->CreateRenderTarget(Settings::get().getRenderWidth(), Settings::get().getRenderHeight(), Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
		RSManager::get().registerMainRenderSurface(*ppSurface);
		return hr;
	}
	return m_pD3Ddev->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
	return m_pD3Ddev->CreateStateBlock(Type, ppSB);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
{
	return RSManager::get().redirectCreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** VERTexBuffer, HANDLE* pSharedHandle)
{
	return m_pD3Ddev->CreateVertexBuffer(Length, Usage, FVF, Pool, VERTexBuffer, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
	return m_pD3Ddev->CreateVertexDeclaration(pVertexElements, ppDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
	HRESULT res = m_pD3Ddev->CreateVertexShader(pFunction, ppShader);
	if (Settings::get().getLogLevel() > 12)
	{
		SDLOG(12, "CreateVertexShader data: %p : shader: %p", pFunction, *ppShader);
		LPD3DXBUFFER buffer;
		D3DXDisassembleShader(pFunction, false, NULL, &buffer);
		SDLOG(12, "===== disassembly:\n%s\n==============", buffer->GetBufferPointer());
	}
	return res;
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
{
	return m_pD3Ddev->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::DeletePatch(UINT Handle)
{
	return m_pD3Ddev->DeletePatch(Handle);
}

HRESULT APIENTRY hkIDirect3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
	return m_pD3Ddev->EndStateBlock(ppSB);
}

HRESULT APIENTRY hkIDirect3DDevice9::EvictManagedResources()
{
	return m_pD3Ddev->EvictManagedResources();
}

UINT APIENTRY hkIDirect3DDevice9::GetAvailableTextureMem()
{
	return m_pD3Ddev->GetAvailableTextureMem();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetClipPlane(DWORD Index, float *pPlane)
{
	return m_pD3Ddev->GetClipPlane(Index, pPlane);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9 *pClipStatus)
{
	return m_pD3Ddev->GetClipStatus(pClipStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
	return m_pD3Ddev->GetCreationParameters(pParameters);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetCurrentTexturePalette(UINT *pPaletteNumber)
{
	return m_pD3Ddev->GetCurrentTexturePalette(pPaletteNumber);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9 **ppZStencilSurface)
{
	return m_pD3Ddev->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDeviceCaps(D3DCAPS9 *pCaps)
{
	return m_pD3Ddev->GetDeviceCaps(pCaps);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDirect3D(IDirect3D9 **ppD3D9)
{
	HRESULT hRet = m_pD3Ddev->GetDirect3D(ppD3D9);
	if (SUCCEEDED(hRet))
		*ppD3D9 = m_pD3Dint;
	return hRet;
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
	SDLOG(15, "GetDisplayMode %u", iSwapChain);
	return m_pD3Ddev->GetDisplayMode(iSwapChain, pMode);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
	return m_pD3Ddev->GetFrontBufferData(iSwapChain, pDestSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetFVF(DWORD* pFVF)
{
	return m_pD3Ddev->GetFVF(pFVF);
}

void APIENTRY hkIDirect3DDevice9::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp)
{
	return m_pD3Ddev->GetGammaRamp(iSwapChain, pRamp);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
	return m_pD3Ddev->GetIndices(ppIndexData);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetLight(DWORD Index, D3DLIGHT9 *pLight)
{
	return m_pD3Ddev->GetLight(Index, pLight);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetLightEnable(DWORD Index, BOOL *pEnable)
{
	return m_pD3Ddev->GetLightEnable(Index, pEnable);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetMaterial(D3DMATERIAL9 *pMaterial)
{
	return m_pD3Ddev->GetMaterial(pMaterial);
}

float APIENTRY hkIDirect3DDevice9::GetNPatchMode()
{
	return m_pD3Ddev->GetNPatchMode();
}

unsigned int APIENTRY hkIDirect3DDevice9::GetNumberOfSwapChains()
{
	return m_pD3Ddev->GetNumberOfSwapChains();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY *pEntries)
{
	return m_pD3Ddev->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
	return m_pD3Ddev->GetPixelShader(ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	return m_pD3Ddev->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	return m_pD3Ddev->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	return m_pD3Ddev->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
{
	return m_pD3Ddev->GetRasterStatus(iSwapChain, pRasterStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State, DWORD *pValue)
{
	return m_pD3Ddev->GetRenderState(State, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderTarget(DWORD renderTargetIndex, IDirect3DSurface9** ppRenderTarget)
{
	return m_pD3Ddev->GetRenderTarget(renderTargetIndex, ppRenderTarget);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderTargetData(IDirect3DSurface9* renderTarget, IDirect3DSurface9* pDestSurface)
{
	return m_pD3Ddev->GetRenderTargetData(renderTarget, pDestSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
	return m_pD3Ddev->GetSamplerState(Sampler, Type, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetScissorRect(RECT* pRect)
{
	return m_pD3Ddev->GetScissorRect(pRect);
}

BOOL APIENTRY hkIDirect3DDevice9::GetSoftwareVertexProcessing()
{
	return m_pD3Ddev->GetSoftwareVertexProcessing();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* OffsetInBytes, UINT* pStride)
{
	return m_pD3Ddev->GetStreamSource(StreamNumber, ppStreamData, OffsetInBytes, pStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetStreamSourceFreq(UINT StreamNumber, UINT* Divider)
{
	return m_pD3Ddev->GetStreamSourceFreq(StreamNumber, Divider);
}

// GameOverlay will hook this method. Method must be large enough to hold GameOverlay hook, direct jump will crash !
HRESULT APIENTRY hkIDirect3DDevice9::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
{
	// Add some space, 16bytes should be enough
	_asm
	{
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
	}

	return m_pD3Ddev->GetSwapChain(iSwapChain, pSwapChain);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTexture(DWORD Stage, IDirect3DBaseTexture9 **ppTexture)
{
	return m_pD3Ddev->GetTexture(Stage, ppTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue)
{
	return m_pD3Ddev->GetTextureStageState(Stage, Type, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix)
{
	return m_pD3Ddev->GetTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
	return m_pD3Ddev->GetVertexDeclaration(ppDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
	return m_pD3Ddev->GetVertexShader(ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	return m_pD3Ddev->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	return m_pD3Ddev->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	return m_pD3Ddev->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetViewport(D3DVIEWPORT9 *pViewport)
{
	return m_pD3Ddev->GetViewport(pViewport);
}

HRESULT APIENTRY hkIDirect3DDevice9::LightEnable(DWORD LightIndex, BOOL bEnable)
{
	return m_pD3Ddev->LightEnable(LightIndex, bEnable);
}

HRESULT APIENTRY hkIDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix)
{
	SDLOG(5, "MultiplyTransform state: %u matrix: \n%s", State, D3DMatrixToString(pMatrix));
	return m_pD3Ddev->MultiplyTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
{
	return m_pD3Ddev->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

ULONG APIENTRY hkIDirect3DDevice9::Release()
{
	ULONG refs = m_pD3Ddev->Release();
	if (!refs)
		delete this;

	m_pD3Ddev = nullptr;
	return refs;
}

HRESULT APIENTRY hkIDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	RSManager::get().releaseResources();
	SDLOG(0, "Reset ------");

	D3DPRESENT_PARAMETERS adjusted = RSManager::get().adjustPresentationParameters(pPresentationParameters);

	HRESULT hRet = m_pD3Ddev->Reset(&adjusted);

	if (SUCCEEDED(hRet))
	{
		SDLOG(0, " - succeeded");
		RSManager::get().initResources();
	}
	else
	{
		SDLOG(0, "ERROR: Reset Failed!");
		SDLOG(0, "Error code: %s", DXGetErrorString(hRet));
	}

	return hRet;
}

HRESULT APIENTRY hkIDirect3DDevice9::SetClipPlane(DWORD Index, CONST float *pPlane)
{
	return m_pD3Ddev->SetClipPlane(Index, pPlane);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9 *pClipStatus)
{
	return m_pD3Ddev->SetClipStatus(pClipStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber)
{
	return m_pD3Ddev->SetCurrentTexturePalette(PaletteNumber);
}

void APIENTRY hkIDirect3DDevice9::SetCursorPosition(int X, int Y, DWORD Flags)
{
	m_pD3Ddev->SetCursorPosition(X, Y, Flags);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 *pCursorBitmap)
{
	return m_pD3Ddev->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
	SDLOG(5, "SetDepthStencilSurface %p", pNewZStencil);
	return RSManager::get().redirectSetDepthStencilSurface(pNewZStencil);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs)
{
	return m_pD3Ddev->SetDialogBoxMode(bEnableDialogs);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetFVF(DWORD FVF)
{
	return m_pD3Ddev->SetFVF(FVF);
}

void APIENTRY hkIDirect3DDevice9::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
	m_pD3Ddev->SetGammaRamp(iSwapChain, Flags, pRamp);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
	return m_pD3Ddev->SetIndices(pIndexData);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetLight(DWORD Index, CONST D3DLIGHT9 *pLight)
{
	return m_pD3Ddev->SetLight(Index, pLight);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9 *pMaterial)
{
	return m_pD3Ddev->SetMaterial(pMaterial);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetNPatchMode(float nSegments)
{
	return m_pD3Ddev->SetNPatchMode(nSegments);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY *pEntries)
{
	return m_pD3Ddev->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader)
{
	return m_pD3Ddev->SetPixelShader(pShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
	return m_pD3Ddev->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	return m_pD3Ddev->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	return m_pD3Ddev->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
	return RSManager::get().redirectSetRenderState(State, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
	SDLOG(14, "SetSamplerState sampler %lu:   state type: %s   value: %lu", Sampler, D3DSamplerStateTypeToString(Type), Value);
	if (Settings::get().getFilteringOverride() == 2)
	{
		SDLOG(10, " - aniso sampling activated!");
		if (Type == D3DSAMP_MAXANISOTROPY)
		{
			return m_pD3Ddev->SetSamplerState(Sampler, Type, 16);
		}
		else if (Type != D3DSAMP_MINFILTER && Type != D3DSAMP_MAGFILTER)
		{
			return m_pD3Ddev->SetSamplerState(Sampler, Type, Value);
		}
		else
		{
			return m_pD3Ddev->SetSamplerState(Sampler, Type, D3DTEXF_ANISOTROPIC);
		}
	}
	else if (Settings::get().getFilteringOverride() == 1)
	{
		if ((Type == D3DSAMP_MINFILTER || Type == D3DSAMP_MIPFILTER) && (Value == D3DTEXF_POINT || Value == D3DTEXF_NONE))
		{
			SDLOG(10, " - linear override activated!");
			return m_pD3Ddev->SetSamplerState(Sampler, Type, D3DTEXF_LINEAR);
		}
	}
	return m_pD3Ddev->SetSamplerState(Sampler, Type, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetScissorRect(CONST RECT* pRect)
{
	SDLOG(5, "SetScissorRect %s", RectToString(pRect));
	// These are scissor rects used for shadow rendering, should not be suppressed:
	//SetScissorRect RECT[   0/   0/1024/1024]
	//SetScissorRect RECT[1024/   0/2048/1024]
	//SetScissorRect RECT[   0/1024/1024/2048]
	//SetScissorRect RECT[1024/1024/2048/2048]
	if (RSManager::get().isViewport(*pRect)
	        || (pRect->left == 0 && pRect->top == 0 && pRect->right == 1024 && pRect->bottom == 1024)
	        || (pRect->left == 1024 && pRect->top == 0 && pRect->right == 2048 && pRect->bottom == 1024)
	        || (pRect->left == 0 && pRect->top == 1024 && pRect->right == 1024 && pRect->bottom == 2048)
	        || (pRect->left == 1024 && pRect->top == 1024 && pRect->right == 2048 && pRect->bottom == 2048)
	   )
	{
		return m_pD3Ddev->SetScissorRect(pRect);
	}
	SDLOG(5, " - Lyrical Tokarev, kill them all!", RectToString(pRect));
	return D3D_OK;
}

HRESULT APIENTRY hkIDirect3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware)
{
	return m_pD3Ddev->SetSoftwareVertexProcessing(bSoftware);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
	return m_pD3Ddev->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetStreamSourceFreq(UINT StreamNumber, UINT Divider)
{
	return m_pD3Ddev->SetStreamSourceFreq(StreamNumber, Divider);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTexture(DWORD Stage, IDirect3DBaseTexture9 *pTexture)
{
	unsigned index = RSManager::get().getTextureIndex((IDirect3DTexture9*)pTexture);
	SDLOG(6, "setTexture %d, %p (index %u)", Stage, pTexture, index);
	return RSManager::get().redirectSetTexture(Stage, pTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
	return RSManager::get().redirectSetTextureStageState(Stage, Type, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix)
{
	SDLOG(0, "SetTransform state: %u matrix: \n%s", State, D3DMatrixToString(pMatrix));
	return m_pD3Ddev->SetTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
	return m_pD3Ddev->SetVertexDeclaration(pDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
	return m_pD3Ddev->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	return m_pD3Ddev->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

BOOL APIENTRY hkIDirect3DDevice9::ShowCursor(BOOL bShow)
{
	return m_pD3Ddev->ShowCursor(bShow);
}

HRESULT APIENTRY hkIDirect3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
{
	SDLOG(5, "StretchRect src -> dest, sR -> dR : %p -> %p,  %s -> %s", pSourceSurface, pDestSurface, RectToString(pSourceRect), RectToString(pDestRect));
	return RSManager::get().redirectStretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

HRESULT APIENTRY hkIDirect3DDevice9::TestCooperativeLevel()
{
	SDLOG(8, "TestCooperativeLevel");
	HRESULT hr = m_pD3Ddev->TestCooperativeLevel();
	SDLOG(8, " - returned %s", (hr == D3D_OK ? "OK" : (D3DERR_DEVICELOST ? "DEVICELOST" : (hr == D3DERR_DEVICENOTRESET ? "DEVICENOTRESET" : "DRIVERINTERNALERROR"))));
	return hr;
}

HRESULT APIENTRY hkIDirect3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
{
	return m_pD3Ddev->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

HRESULT APIENTRY hkIDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9 *pSourceTexture, IDirect3DBaseTexture9 *pDestinationTexture)
{
	return m_pD3Ddev->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::ValidateDevice(DWORD *pNumPasses)
{
	return m_pD3Ddev->ValidateDevice(pNumPasses);
}
