#pragma once
#include <d3d11.h>

extern "C" {
	void __fastcall DLSS_Tese();
	void __fastcall DLSS_Release();
}

void __fastcall DLSS_Initialization(ID3D11Device** ppDevice, ID3D11DeviceContext** ppImmediateContext, DXGI_SWAP_CHAIN_DESC* pChainDesc);
void __fastcall DLSS_TriggerFailureResult(UINT32 slresult, int FreeDLSS);
void __fastcall DLSS_GetBuffer(ID3D11DeviceContext* pContext, UINT NumViews, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView);
void __fastcall DLSS_ClearBuffer();
void __fastcall DLSS_Evaluate(ID3D11Texture2D* out);
