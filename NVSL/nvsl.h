#pragma once
#include <d3d11.h>
#pragma comment(lib, "D3D11.lib")

#include "include/nvsdk_ngx_helpers.h"
//#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "./include/nvsdk_ngx_d.lib")

void InstallNVIDIAdlss(PBYTE hmodEXE);
void __fastcall TriggerDLSSFailureResult(UINT32 slresult, int FreeDLSS);

extern "C" {
    HRESULT WINAPI Initialize_NGX_dlss(
        _In_opt_ IDXGIAdapter* pAdapter,
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
    void __fastcall InitializeDLSSFeatures(ID3D11DeviceContext* deviceContext);

    void __fastcall Evaluate_NGX_dlss(ID3D11DeviceContext* d3dcontext, ID3D11Device* InDevice);
    void __fastcall Release_NGX_dlss();
}
