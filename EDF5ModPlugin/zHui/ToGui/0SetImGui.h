#pragma once
#include <d3d11.h>

extern "C" {
	int __fastcall togui_GetDXGISwapChain(int protectECX, IDXGISwapChain* pSwapChain);
}

HRESULT __stdcall togui_Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
HRESULT __stdcall togui_ResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

void togui_InitializeImGui();
