#pragma once
#include <d3d11.h>

void module_InitializeAddImGui(PBYTE hmodEXE);

extern "C" {
    HRESULT WINAPI module_InitializeD3D11(
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
}

HRESULT WINAPI New_CreateTexture2D(ID3D11Device* pDevice, const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D);
void New_OMSetRenderTargets(ID3D11DeviceContext* pContext, UINT NumViews, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView);
