#pragma once
#include <d3d11.h>
#pragma comment(lib, "D3D11.lib")

typedef struct GameDXGIRender_t {
	char pad0[0x70];
	DXGI_SWAP_CHAIN_DESC DXGISwapChainDesc;
	ID3D11Device* pD3D11Device;
	ID3D11DeviceContext* pD3D11DeviceContext;
	IDXGISwapChain* pDXGISwapChain;
	char padD0[0x90];
	ID3D11RenderTargetView* pMainRenderTargetView;
} *PGameDXGIRender;
#if 1
static_assert(offsetof(GameDXGIRender_t, DXGISwapChainDesc) == 0x70);
static_assert(offsetof(GameDXGIRender_t, pD3D11Device) == 0xB8);
static_assert(offsetof(GameDXGIRender_t, pD3D11DeviceContext) == 0xC0);
static_assert(offsetof(GameDXGIRender_t, pDXGISwapChain) == 0xC8);
static_assert(offsetof(GameDXGIRender_t, pMainRenderTargetView) == 0x160);
#endif

void DXGI_Initialize(PBYTE hmodEXE);

extern "C" {
	PGameDXGIRender __fastcall DXGI_GetGameDXGIRender();
	DXGI_SWAP_CHAIN_DESC* __fastcall DXGI_GetDXGISwapChainDesc();
}
