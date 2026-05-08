#pragma once
#include <d3d11.h>

// size is 0x240, but incomplete at present
typedef struct alignas(16) xgl_system_CB_t {
	float g_xgl_view[4][4];
	float g_xgl_view_inverse[4][4];
	float g_xgl_projection[4][4];
	float g_xgl_view_projection[4][4];
	float g_xgl_view_projection_inverse[4][4];
	char pad140[0x100];
}*Pxgl_system_CB;
#if 1
static_assert(sizeof(xgl_system_CB_t) == 0x240);
#endif

typedef struct alignas(16) xgl_transform_CB_t {
	float g_xgl_world[4][4];
}*Pxgl_transform_CB;

typedef struct g_D3D_ResourceInfo_t {
	ID3D11Texture2D* pTexture;
	ID3D11RenderTargetView* pRTV;
	ID3D11ShaderResourceView* pSRV;
	ID3D11UnorderedAccessView* pUAV;
	int width;
	int height;
} *Pg_D3D_ResourceInfo;

typedef struct GameRenderer1259680_t {
	void* vf_table;
	void* pointer8;
	xgl_system_CB_t ConstantBuffer0;
	char pad250[0x20];
	xgl_transform_CB_t ConstantBuffer1;
	char pad2B0[0x388];
	Pg_D3D_ResourceInfo pDSVInfo638;
	char pad640[0x28];
	Pg_D3D_ResourceInfo pDSVInfo668; // Not all white
	char pad670[0x50];
	Pg_D3D_ResourceInfo pColorPass1RT0; // aka g_albedoRGB
	char pad6C8[0x28];
	Pg_D3D_ResourceInfo pColorPass1RT1; // aka g_specularRGB
	char pad6F8[0x28];
	Pg_D3D_ResourceInfo pColorPass1RT2; // aka g_normalXYZ
	char pad728[0x28];
	Pg_D3D_ResourceInfo pColorPass1RT3; // aka g_smooth_reflection_specpow
	char pad758[0x28];
	Pg_D3D_ResourceInfo pColorPass1RT4; // aka g_metallic_roughness
	char pad788[0x28];
	Pg_D3D_ResourceInfo pColorPass1RT5; // aka g_viewXYZ_id
	char pad7B8[0x28];
	Pg_D3D_ResourceInfo pColorPass1RT6; // aka g_light_accumulate
	char pad7E8[0x28];
	Pg_D3D_ResourceInfo pColorPass1RT7; // aka g_specular_accumulate
} *PGameRenderer1259680;
#if 1
static_assert(offsetof(GameRenderer1259680_t, ConstantBuffer0) == 0x10);
static_assert(offsetof(GameRenderer1259680_t, ConstantBuffer1) == 0x270);
static_assert(offsetof(GameRenderer1259680_t, pDSVInfo638) == 0x638);
static_assert(offsetof(GameRenderer1259680_t, pDSVInfo668) == 0x668);
static_assert(offsetof(GameRenderer1259680_t, pColorPass1RT0) == 0x6C0);
static_assert(offsetof(GameRenderer1259680_t, pColorPass1RT1) == 0x6F0);
static_assert(offsetof(GameRenderer1259680_t, pColorPass1RT2) == 0x720);
static_assert(offsetof(GameRenderer1259680_t, pColorPass1RT3) == 0x750);
static_assert(offsetof(GameRenderer1259680_t, pColorPass1RT4) == 0x780);
static_assert(offsetof(GameRenderer1259680_t, pColorPass1RT5) == 0x7B0);
static_assert(offsetof(GameRenderer1259680_t, pColorPass1RT6) == 0x7E0);
static_assert(offsetof(GameRenderer1259680_t, pColorPass1RT7) == 0x810);
#endif

// size is 0xC0, in EDF5.exe+3D8DF8
typedef struct alignas(16) G_Application_t {
	void* vf_table;
	char pad08[0x8];
	float FPS;
	char pad14[0x8C];
	void* pDSVResource; Pg_D3D_ResourceInfo pDSVInfo;
}*PG_Application;
#if 1
static_assert(offsetof(G_Application_t, pDSVInfo) == 0xA8);
#endif

void __fastcall XGS_DXresource_Initialize(PBYTE hmodEXE);

PGameRenderer1259680 __fastcall DXGI_GetGameRenderer1259680();
PG_Application __fastcall DXGI_GetApplication1253708();

extern "C"{
	Pg_D3D_ResourceInfo __fastcall Get_g_xgl_draw_utility_color_textrure();
	Pg_D3D_ResourceInfo __fastcall Get_g_xgl_viewXYZ_id_texture();
}