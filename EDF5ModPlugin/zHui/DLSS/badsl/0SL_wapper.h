#pragma once
#include <d3d11on12.h>
#include "zHui/ToGui/0GetDXGI.h"

void __fastcall streamline_TriggerFailureResult(UINT32 slresult, void* sl);
void __fastcall streamline_InitializeSLPointers();
bool __fastcall streamline_InitializeD3D12();
bool __fastcall streamline_InitializeVK(ID3D11Device* device, void* pVKinfo);
void __fastcall streamline_CreateD3D12Device();
void __fastcall streamline_InitializePostProcess(ID3D11Device* device, ID3D11DeviceContext* context);
void __fastcall streamline_Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

extern "C"{
	bool __fastcall streamline_CreateSwapChain(DXGI_SWAP_CHAIN_DESC* pChainDesc, IDXGISwapChain1** ppSwapChain, IDXGISwapChain1* pD3D11SwapChain);
	HRESULT __fastcall streamline_SwapChainGetBuffer(IDXGISwapChain* pSwapChain, PGameDXGIRender pGameDXGI, ID3D11Texture2D** pOut);
	void __fastcall streamline_SwapChainPresent(PGameDXGIRender pGameDXGI);

	void __fastcall streamline_Release();
	void __fastcall streamline_Reset();

	// playerCount is -1 means forced resolution change.
	void __fastcall streamline_SetFeature(int playerCount);
	// execute post-processing and super-resolution functions.
	void* __fastcall streamline_ExecuteSR(Pg_D3D11DeviceInfo pD3D, int OutOffset, void* saveRCX);
}
