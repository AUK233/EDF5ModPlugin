#pragma once
#include <d3d11.h>

typedef struct g_D3D11DeviceInfo_t {
	ID3D11Device* device; // maybe?
	ID3D11DeviceContext* context;
	char pad10[0x628];
	ID3D11UnorderedAccessView** pCurrentCSUnorderedAccessViews[3];
	char pad6A8[0x310];
	ID3D11ComputeShader* pCurrentCSShader;
	char pad9C0[0x288];
	ID3D11SamplerState** pCurrentCSSamplerState[12];
	char padCA8[0x250];
	ID3D11Buffer** pCurrentCSBuffer[11];
	char padF50[0x1418];
	ID3D11ShaderResourceView** pCurrentCSShaderResourceView[16];
} *Pg_D3D11DeviceInfo;
#if 1
static_assert(offsetof(g_D3D11DeviceInfo_t, pCurrentCSUnorderedAccessViews) == 0x638);
static_assert(offsetof(g_D3D11DeviceInfo_t, pCurrentCSShader) == 0x960);
static_assert(offsetof(g_D3D11DeviceInfo_t, pCurrentCSSamplerState) == 0xBF0);
static_assert(offsetof(g_D3D11DeviceInfo_t, pCurrentCSBuffer) == 0xEA0);
static_assert(offsetof(g_D3D11DeviceInfo_t, pCurrentCSShaderResourceView) == 0x2310);
#endif

// ===============================================================

typedef struct alignas(16) xgl_system_Light_t {
	float m_vector[3], m_pad0;
	float m_color[3], m_pad1;
	float m_specular_color[3], m_pad2;
}*Pxgl_system_Light;

// size is 0x240, but incomplete at present
typedef struct alignas(16) xgl_system_CB_t {
	float g_xgl_view[4][4];
	float g_xgl_view_inverse[4][4];
	float g_xgl_projection[4][4];
	float g_xgl_view_projection[4][4];
	float g_xgl_view_projection_inverse[4][4];
	UINT32 g_xgl_target_dimension[2];
	float g_xgl_time;
	int g_xgl_id;
	xgl_system_Light_t g_xgl_light[4];
	float g_xgl_ambient_color[3], g_pad3;
	float g_xgl_fog_mul, g_xgl_fog_add, g_xgl_fade_mul, g_xgl_fade_add;
	float g_xgl_fog_color[4];
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

typedef struct GameRenderer_RTV_t {
	char pad00[0x18];
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
} *PGameRenderer_RTV;
#if 1
static_assert(offsetof(GameRenderer_RTV_t, pColorPass1RT5) == 0x108);
#endif

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
	char pad670[0x38];
	GameRenderer_RTV_t RTV;
	char pad818[0x318];
	ID3D11Buffer** CB_xgl_system;
} *PGameRenderer1259680;
#if 1
static_assert(offsetof(GameRenderer1259680_t, ConstantBuffer0) == 0x10);
static_assert(offsetof(GameRenderer1259680_t, ConstantBuffer1) == 0x270);
static_assert(offsetof(GameRenderer1259680_t, pDSVInfo638) == 0x638);
static_assert(offsetof(GameRenderer1259680_t, pDSVInfo668) == 0x668);
static_assert(offsetof(GameRenderer1259680_t, RTV.pColorPass1RT0) == 0x6C0);
static_assert(offsetof(GameRenderer1259680_t, RTV.pColorPass1RT1) == 0x6F0);
static_assert(offsetof(GameRenderer1259680_t, RTV.pColorPass1RT2) == 0x720);
static_assert(offsetof(GameRenderer1259680_t, RTV.pColorPass1RT3) == 0x750);
static_assert(offsetof(GameRenderer1259680_t, RTV.pColorPass1RT4) == 0x780);
static_assert(offsetof(GameRenderer1259680_t, RTV.pColorPass1RT5) == 0x7B0);
static_assert(offsetof(GameRenderer1259680_t, RTV.pColorPass1RT6) == 0x7E0);
static_assert(offsetof(GameRenderer1259680_t, RTV.pColorPass1RT7) == 0x810);
static_assert(offsetof(GameRenderer1259680_t, CB_xgl_system) == 0xB30);
#endif

// size is 0x38, in EDF5.exe+5E1C6E
typedef struct alignas(16) G_TextureBackBuffer_t {
	void* vf_table;
	char pad08[8];
	void* pResource; Pg_D3D_ResourceInfo pInfo;
}*PG_TextureBackBuffer;
#if 1
static_assert(offsetof(G_TextureBackBuffer_t, pInfo) == 0x18);
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
PG_TextureBackBuffer __fastcall DXGI_GetTextureBackBuffer1256CB8();

extern "C"{
	Pg_D3D_ResourceInfo __fastcall Get_g_xgl_draw_utility_color_textrure();
	Pg_D3D_ResourceInfo __fastcall Get_g_xgl_viewXYZ_id_texture();
}