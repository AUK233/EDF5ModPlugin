#pragma once
#include <d3d11on12.h>
#include "zHui/ToGui/0GetDXGI.h"

bool __fastcall streamline_InitializePointers();

// true is success, false is fail
bool __fastcall streamline_CreateD3D11On12Device(UINT Flags, ID3D11Device** ppDevice, ID3D11DeviceContext** ppImmediateContext);

extern "C"{
	bool __fastcall streamline_CreateSwapChain(DXGI_SWAP_CHAIN_DESC* pChainDesc, IDXGISwapChain1** ppSwapChain, IDXGISwapChain1* pD3D11SwapChain);
	HRESULT __fastcall streamline_SwapChainGetBuffer(IDXGISwapChain* pSwapChain, PGameDXGIRender pGameDXGI, ID3D11Texture2D** pOut);
	void __fastcall streamline_SwapChainPresent(PGameDXGIRender pGameDXGI);
	void __fastcall streamline_SwapChainPresentToggle();
	bool __fastcall streamline_MapLoadResources(void* saveRcx);
}
