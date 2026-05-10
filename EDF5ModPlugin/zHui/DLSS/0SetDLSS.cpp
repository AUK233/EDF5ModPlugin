#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>
#include <vector>

#include "lib/nvsdk_ngx.h"
#include "lib/nvsdk_ngx_defs.h"
#include "lib/nvsdk_ngx_params.h"
#include "lib/nvsdk_ngx_helpers.h"
//#pragma comment(lib, "./zHui/DLSS/lib/nvsdk_ngx_d.lib")

#include "Base/g_DXresource.h"
#include "Base/g_system.h"
#include "1SetPostProcess.h"

#include "0SetDLSS.h"

//#define DEBUGMODE
__declspec(align(16)) typedef struct NGX_DLSS_t {
	ID3D11Texture2D* ColorBuffer;
	ID3D11Texture2D* OutColor;
	ID3D11Texture2D* DepthBuffer;
	int resolution[2];
	NVSDK_NGX_Parameter* m_ngxParameters;
	NVSDK_NGX_Handle* m_dlssFeature;
	int m_bDlssAvailable;
	int JitterIndex;
	int IsReset;
	int IsSplitScreen;
	int IsReady;
} *PNGX_DLSS;

PNGX_DLSS pNGX_dlss = nullptr;
D3D::PD3DPostProcess pD3DPostProcess = nullptr;

void __fastcall DLSS_CreateFeature(int playerCount) {
	if (!pD3DPostProcess) return;
	if (pD3DPostProcess->PlayerCount == playerCount) return;

	pD3DPostProcess->PlayerCount = playerCount;

	auto pTexBack = DXGI_GetTextureBackBuffer1256CB8();
	auto pBuffer = pTexBack->pInfo;

	int res[2];
	res[0] = pBuffer->width;
	res[1] = pBuffer->height;
	if (playerCount == 2) {
		res[0] /= 2;
	}

	pD3DPostProcess->SetBuffer(res[0], res[1]);

	if (pNGX_dlss){
		if (playerCount == 2) {
			pNGX_dlss->IsSplitScreen = 1;
		}
		else {
			pNGX_dlss->IsSplitScreen = 0;
		}

		DLSS_SetFeature(pD3DPostProcess->Context, res[0], res[1]);
	}
	// end
}

void* __fastcall DLSS_Draw(ID3D11DeviceContext* pContext, int OutOffset, void* saveRCX){
	if (!pD3DPostProcess) return saveRCX;
	if (!pNGX_dlss) return saveRCX;

	int playerIndex = 0;
	if (OutOffset) playerIndex = 1;


	auto sys = XGS_GetXGSSystemPointer();
	auto pColorRes = (Pg_D3D_ResourceInfo)sys->player[playerIndex].pDrawColorInfo;
	//auto pColorRes = Get_g_xgl_draw_utility_color_textrure();
	if (!pColorRes) return saveRCX;
	if (!pColorRes->pUAV) return saveRCX;

	D3D11_TEXTURE2D_DESC inDesc;
	auto pTexture = pColorRes->pTexture;
	pTexture->GetDesc(&inDesc);
	// pNGX_dlss->IsReady &&
	if (inDesc.Format != DXGI_FORMAT_R16G16B16A16_FLOAT) return saveRCX;
		
	//pContext->CopyResource(pOutColor, pColorRes->pTexture);

	pContext->CSSetShaderResources(0, 1, &pColorRes->pSRV);
	pContext->CSSetShader(pD3DPostProcess->PostProcessCS, nullptr, 0);
	pContext->CSSetUnorderedAccessViews(0, 1, &pD3DPostProcess->OutUAV[playerIndex], nullptr);
	pContext->Dispatch((pColorRes->width + 15) / 16, (pColorRes->height + 15) / 16, 1);

	pNGX_dlss->ColorBuffer = pD3DPostProcess->OutColor[playerIndex];
	pD3DPostProcess->Context = pContext;

	pNGX_dlss->OutColor = pColorRes->pTexture;
	pNGX_dlss->resolution[0] = pColorRes->width;
	pNGX_dlss->resolution[1] = pColorRes->height;

	/**/
	ID3D11Texture2D* depthBuffer = 0;
	auto pDSVInfo = (Pg_D3D_ResourceInfo)sys->player[playerIndex].pDSVInfo;
	if (pDSVInfo) {
		depthBuffer = pDSVInfo->pTexture;
	}
	pNGX_dlss->DepthBuffer = depthBuffer;

	DLSS_Evaluate();
	return saveRCX;
	// end
}

void __fastcall DLSS_Release(){
	if (pNGX_dlss) {
		NVSDK_NGX_D3D11_DestroyParameters(pNGX_dlss->m_ngxParameters);
		if (pNGX_dlss->m_bDlssAvailable) {
			NVSDK_NGX_D3D11_ReleaseFeature(pNGX_dlss->m_dlssFeature);
		}

		_aligned_free(pNGX_dlss);
		pNGX_dlss = nullptr;
	}

	if (pD3DPostProcess) {
		pD3DPostProcess->ReleaseBuffer();
		_aligned_free(pD3DPostProcess);
		pD3DPostProcess = nullptr;
	}

	NVSDK_NGX_D3D11_Shutdown1(nullptr);
}

void __fastcall DLSS_Initialization(ID3D11Device** ppDevice, ID3D11DeviceContext** ppImmediateContext, DXGI_SWAP_CHAIN_DESC* pChainDesc) {
	auto device = *ppDevice;
	auto context = *ppImmediateContext;

	if (!pD3DPostProcess) {
		auto pD3D = (D3D::PD3DPostProcess)_aligned_malloc(sizeof(D3D::D3DPostProcess_t), 16U);
		if (pD3D) {
			pD3DPostProcess = pD3D;
			ZeroMemory(pD3D, sizeof(D3D::D3DPostProcess_t));
			pD3D->Initialize(device, context, pChainDesc);
		}
		// end
	}

	if (pNGX_dlss) return;

	auto p = (PNGX_DLSS)_aligned_malloc(sizeof(NGX_DLSS_t), 16U);
	if (!p) return;

	ZeroMemory(p, sizeof(NGX_DLSS_t));
	pNGX_dlss = p;

	p->JitterIndex = 1;

	NVSDK_NGX_FeatureCommonInfo featureCommonInfo = {};
	featureCommonInfo.LoggingInfo.DisableOtherLoggingSinks = false;
	featureCommonInfo.LoggingInfo.MinimumLoggingLevel = NVSDK_NGX_LOGGING_LEVEL_VERBOSE;
	/*
	NVSDK_NGX_D3D11_Init(0x3AC09EF, L".", device);
	*/

	auto slresult = NVSDK_NGX_D3D11_Init(231313132, L"Z:\\TEMP", device, &featureCommonInfo); // this id is from ngx_dlss_demo 
	if (slresult != NVSDK_NGX_Result_Success) {
		DLSS_TriggerFailureResult((UINT32)slresult - NVSDK_NGX_Result_Fail, 1);
		return;
	}

	slresult = NVSDK_NGX_D3D11_GetCapabilityParameters(&pNGX_dlss->m_ngxParameters);
	if (slresult != NVSDK_NGX_Result_Success) {
		DLSS_TriggerFailureResult((UINT32)(slresult - NVSDK_NGX_Result_Fail) + 200, 1);
		return;
	}

	NVSDK_NGX_Parameter_SetUI(p->m_ngxParameters, NVSDK_NGX_Parameter_DLSS_Hint_Render_Preset_DLAA, NVSDK_NGX_DLSS_Hint_Render_Preset_Default);
}

void __fastcall DLSS_TriggerFailureResult(UINT32 slresult, int FreeDLSS) {
#if defined(DEBUGMODE)
	MessageBoxW(NULL, std::to_wstring((UINT32)slresult).c_str(), L"error", MB_OK);
#endif

	if (FreeDLSS) {
		if (pNGX_dlss) {
			_aligned_free(pNGX_dlss);
			pNGX_dlss = nullptr;
		}
	}
}

void __fastcall DLSS_SetFeature(ID3D11DeviceContext* pContext, UINT Width, UINT Height) {
	auto p = pNGX_dlss;

	if (!p) return;
	p->m_bDlssAvailable = 0;

	if (p->m_bDlssAvailable) {
		NVSDK_NGX_D3D11_ReleaseFeature(pNGX_dlss->m_dlssFeature);
		DLSS_Reset();
	}

	NVSDK_NGX_DLSS_Create_Params DlssCreateParams;
	memset(&DlssCreateParams, 0, sizeof(DlssCreateParams));

	DlssCreateParams.Feature.InWidth = Width;
	DlssCreateParams.Feature.InHeight = Height;
	DlssCreateParams.Feature.InTargetWidth = Width;
	DlssCreateParams.Feature.InTargetHeight = Height;
	DlssCreateParams.Feature.InPerfQualityValue = NVSDK_NGX_PerfQuality_Value_DLAA;
	DlssCreateParams.InFeatureCreateFlags = NVSDK_NGX_DLSS_Feature_Flags_None;
	DlssCreateParams.InEnableOutputSubrects = false;


	auto ResultDLSS = NGX_D3D11_CREATE_DLSS_EXT(pContext, &p->m_dlssFeature, p->m_ngxParameters, &DlssCreateParams);
	if (ResultDLSS != NVSDK_NGX_Result_Success) {
		DLSS_TriggerFailureResult((UINT32)ResultDLSS, 1);
		return;
	}

	p->m_bDlssAvailable = 1;
}

void __fastcall DLSS_GetBuffer(ID3D11DeviceContext* pContext, UINT NumViews, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView){
	if (!pNGX_dlss) return;

	ID3D11Texture2D* pResource;
	D3D11_TEXTURE2D_DESC inDesc;

	if (NumViews == 8){
		DLSS_ClearBuffer();
		pDepthStencilView->GetResource((ID3D11Resource**)&pNGX_dlss->DepthBuffer);
		pNGX_dlss->IsReady = 1;

		// get MotionVectors, old is 4 DXGI_FORMAT_R16G16_FLOAT
		/*
		if (ppRenderTargetViews[5]) {
			ppRenderTargetViews[5]->GetResource((ID3D11Resource**)&pResource);
			pResource->GetDesc(&inDesc);
			if (inDesc.Format == DXGI_FORMAT_R16G16B16A16_FLOAT) {
				DLSS_ClearBuffer();
				pNGX_dlss->MVBuffer = pResource;
				ppRenderTargetViews[0]->GetResource((ID3D11Resource**)&pNGX_dlss->ColorBuffer);
				pDepthStencilView->GetResource((ID3D11Resource**)&pNGX_dlss->DepthBuffer);

				pNGX_dlss->IsReady = 1;
			}
			pResource->Release();
		}*/
	}
	// end
}

void __fastcall DLSS_ClearBuffer(){
	pNGX_dlss->ColorBuffer = nullptr;
	pNGX_dlss->OutColor = nullptr;
	pNGX_dlss->DepthBuffer = nullptr;

	/*if (pNGX_dlss->ColorBuffer) {
		pNGX_dlss->ColorBuffer->Release();
		pNGX_dlss->ColorBuffer = nullptr;
	}

	if (pNGX_dlss->MVBuffer) {
		pNGX_dlss->MVBuffer->Release();
		pNGX_dlss->MVBuffer = nullptr;
	}

	if (pNGX_dlss->DepthBuffer) {
		pNGX_dlss->DepthBuffer->Release();
		pNGX_dlss->DepthBuffer = nullptr;
	}
	*/
	pNGX_dlss->IsReady = 0;
}

void __fastcall DLSS_Reset() {
	if (!pNGX_dlss) return;

	pNGX_dlss->JitterIndex = 1;
	pNGX_dlss->IsReset = 1;
	DLSS_ClearBuffer();
}

float __fastcall DLSS_Halton(int index, int base){
	float result = 0.0f;
	float f = 1.0f / base;
	int i = index;
	while (i > 0) {
		result += f * (i % base);
		i = i / base;
		f = f / base;
	}
	return result;
}

void __fastcall DLSS_GetJitter(float* out){
	if (!pNGX_dlss) return;

	if (pNGX_dlss->IsSplitScreen){
		*(UINT64*)out = 0;
		pNGX_dlss->IsReset = 1;
		return;
	}

	out[0] = DLSS_Halton(pNGX_dlss->JitterIndex, 2);
	out[1] = DLSS_Halton(pNGX_dlss->JitterIndex, 3);
	pNGX_dlss->JitterIndex++;
}

void __fastcall DLSS_Evaluate(){
	if (!pD3DPostProcess) return;
	if (!pNGX_dlss) return;

	auto ColorBuffer = pNGX_dlss->ColorBuffer;
	auto OutColor = pNGX_dlss->OutColor;
	auto DepthBuffer = pNGX_dlss->DepthBuffer;
	if (OutColor && DepthBuffer){
		float jitter[2];
		DLSS_GetJitter(jitter);
		NVSDK_NGX_D3D11_DLSS_Eval_Params D3D11DlssEvalParams;
		memset(&D3D11DlssEvalParams, 0, sizeof(D3D11DlssEvalParams));

		D3D11DlssEvalParams.Feature.pInColor = ColorBuffer;
		D3D11DlssEvalParams.Feature.pInOutput = OutColor;
		D3D11DlssEvalParams.Feature.InSharpness = 1;

		D3D11DlssEvalParams.pInDepth = DepthBuffer;
		D3D11DlssEvalParams.pInMotionVectors = pD3DPostProcess->BlackMV;

		D3D11DlssEvalParams.InJitterOffsetX = jitter[0];
		D3D11DlssEvalParams.InJitterOffsetY = jitter[1];
		D3D11DlssEvalParams.InRenderSubrectDimensions.Width = pNGX_dlss->resolution[0];
		D3D11DlssEvalParams.InRenderSubrectDimensions.Height = pNGX_dlss->resolution[1];

		D3D11DlssEvalParams.InReset = pNGX_dlss->IsReset;
		D3D11DlssEvalParams.InMVScaleX = 1.0;
		D3D11DlssEvalParams.InMVScaleY = 1.0;

		auto result = NGX_D3D11_EVALUATE_DLSS_EXT(pD3DPostProcess->Context, pNGX_dlss->m_dlssFeature, pNGX_dlss->m_ngxParameters, &D3D11DlssEvalParams);

		pNGX_dlss->IsReset = 0;
	}

	DLSS_ClearBuffer();
}
