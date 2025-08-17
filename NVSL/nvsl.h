#pragma once

// decide whether to use streamline
//#define NVSL

#include <d3d11.h>
#if defined(NVSL)
#include "include/sl_hooks.h"
#include "include/sl_dlss.h"
#pragma comment(lib, "./include/sl.interposer.lib")
#else
#pragma comment(lib, "D3D11.lib")
#include "include/nvsdk_ngx_helpers.h"
#pragma comment(lib, "./include/nvsdk_ngx_d.lib")
#endif

void InstallNVIDIAdlss(PBYTE hmodEXE);
void __fastcall TriggerDLSSFailureResult(UINT32 slresult, int FreeDLSS);

extern "C" {
    HRESULT WINAPI Initialize_NGX_dlss(
        DXGI_SWAP_CHAIN_DESC* pChainDesc,
        D3D_DRIVER_TYPE DriverType,
        HMODULE Software,
        UINT Flags,
        _In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
        UINT FeatureLevels,
        UINT SDKVersion,
        _COM_Outptr_opt_ ID3D11Device** ppDevice,
        _Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
        _COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext);

    void __fastcall InitializeDLSS(ID3D11Device* InDevice);
    void __fastcall InitializeDLSSFeatures(ID3D11DeviceContext* deviceContext, DXGI_SWAP_CHAIN_DESC* pChainDesc);

    void __fastcall Evaluate_NGX_dlss(ID3D11DeviceContext* d3dcontext, ID3D11Device* InDevice, IDXGISwapChain* pSwapChain);
    void __fastcall Release_NGX_dlss();
}

__declspec(align(16)) typedef struct CID3D11Forwarder_t {
    ID3D11Device* pDevice;
    ID3D11DeviceContext* pD3DDeviceContext;
    int pad10[4];
	ID3D11Device* pDevice1; // as same as pDevice
    void* pad28;
	ID3D11RenderTargetView* pRenderTargetView[8]; // what? has 8
	ID3D11DepthStencilView* pDepthStencilView;
} *PCID3D11Forwarder;
