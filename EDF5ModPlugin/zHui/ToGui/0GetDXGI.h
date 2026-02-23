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
	ID3D11RenderTargetView* pMainRenderTargetView; // it will release when start mission.
} *PGameDXGIRender;
#if 1
static_assert(offsetof(GameDXGIRender_t, DXGISwapChainDesc) == 0x70);
static_assert(offsetof(GameDXGIRender_t, pD3D11Device) == 0xB8);
static_assert(offsetof(GameDXGIRender_t, pD3D11DeviceContext) == 0xC0);
static_assert(offsetof(GameDXGIRender_t, pDXGISwapChain) == 0xC8);
static_assert(offsetof(GameDXGIRender_t, pMainRenderTargetView) == 0x160);
#endif

// size is 0x240, but incomplete at present
typedef struct alignas(16) xgl_system_CB_t {
	float g_xgl_view[4][4];
	float g_xgl_view_inverse[4][4];
	float g_xgl_projection[4][4];
	float g_xgl_view_projection[4][4];
	float g_xgl_view_projection_inverse[4][4];
}*Pxgl_system_CB;

typedef struct alignas(16) xgl_transform_CB_t {
	float g_xgl_world[4][4];
}*Pxgl_transform_CB;

typedef struct alignas(16) xgl_transform_customize_t {
	float g_xgl_view[4][4];
	float g_xgl_projection[4][4];
}*Pxgl_transform_customize;

typedef struct GameRenderer1259680_t {
	void* vf_table;
	void* pointer8;
	xgl_system_CB_t ConstantBuffer0;
	char pad150[0x120];
	xgl_transform_CB_t ConstantBuffer1;
} *PGameRenderer1259680;
#if 1
static_assert(offsetof(GameRenderer1259680_t, ConstantBuffer0) == 0x10);
static_assert(offsetof(GameRenderer1259680_t, ConstantBuffer1) == 0x270);
#endif

typedef struct alignas(16) xgs_umbra_System125B080_t {
	uintptr_t pad00[2];
	char pad10[0x70];
	float matrix_view[4][4];
	float f4_C0[4], f4_D0[4];
	float matrix_projection[4][4];
	float f4_120[4];
} *Pxgs_umbra_System125B080;
#if 1
static_assert(offsetof(xgs_umbra_System125B080_t, matrix_view) == 0x80);
static_assert(offsetof(xgs_umbra_System125B080_t, matrix_projection) == 0xE0);
#endif

void DXGI_Initialize(PBYTE hmodEXE);

PGameDXGIRender __fastcall DXGI_GetGameDXGIRender();
PGameRenderer1259680 __fastcall DXGI_GetGameRenderer1259680();
Pxgs_umbra_System125B080 __fastcall DXGI_GetUmbraSystem125B080();

int* __fastcall DXGI_GetRealTimeResolution();
extern "C" {
	DXGI_SWAP_CHAIN_DESC* __fastcall DXGI_GetDXGISwapChainDesc();
	Pxgs_umbra_System125B080 __fastcall DXGI_GetUmbraSystem125B080ASM(uintptr_t p);
}
