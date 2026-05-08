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
#include <d3dcompiler.h>

#include "lib/nvsdk_ngx.h"
#include "lib/nvsdk_ngx_defs.h"
#include "lib/nvsdk_ngx_params.h"
#include "lib/nvsdk_ngx_helpers.h"
//#pragma comment(lib, "./zHui/DLSS/lib/nvsdk_ngx_d.lib")

#include "Base/g_DXresource.h"
#include "Base/g_system.h"

#include "0SetDLSS.h"

//#define DEBUGMODE
__declspec(align(16)) typedef struct NGX_DLSS_t {
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	ID3D11Texture2D* ColorBuffer;
	ID3D11Texture2D* DepthBuffer;
	ID3D11Texture2D* MVBuffer;
	int resolution[2];
	NVSDK_NGX_Parameter* m_ngxParameters;
	NVSDK_NGX_Handle* m_dlssFeature;
	int m_bDlssAvailable;
	int JitterIndex;
	int IsReset;
	int IsReady;
} *PNGX_DLSS;

PNGX_DLSS pNGX_dlss = nullptr;

ID3D11Texture2D* pOutColor = nullptr;
ID3D11UnorderedAccessView* pOutUAV = nullptr;
ID3D11Texture2D* pBlackMV = nullptr;
ID3D11ComputeShader* pCSTest = nullptr;

void __fastcall DLSS_Draw(ID3D11DeviceContext* pContext){
	if (!pNGX_dlss) return;

	int dlssReady = 0;
	D3D11_TEXTURE2D_DESC inDesc;

	auto sys = XGS_GetXGSSystemPointer();
	auto pColorRes = (Pg_D3D_ResourceInfo)sys->player[0].pDrawColorInfo;
	//auto pColorRes = Get_g_xgl_draw_utility_color_textrure();
	if (pColorRes) {
		auto pTexture = pColorRes->pTexture;
		pTexture->GetDesc(&inDesc);
		// pNGX_dlss->IsReady && 
		if (inDesc.Format == DXGI_FORMAT_R16G16B16A16_FLOAT && pColorRes->pUAV){
			//pContext->CopyResource(pOutColor, pColorRes->pTexture);

			pContext->CSSetShaderResources(0, 1, &pColorRes->pSRV);
			pContext->CSSetShader(pCSTest, nullptr, 0);
			pContext->CSSetUnorderedAccessViews(0, 1, &pOutUAV, nullptr);
			pContext->Dispatch((pColorRes->width + 7) / 8, (pColorRes->height + 7) / 8, 1);

			pNGX_dlss->ColorBuffer = pColorRes->pTexture;
			pNGX_dlss->resolution[0] = pColorRes->width;
			pNGX_dlss->resolution[1] = pColorRes->height;

			pNGX_dlss->Context = pContext;

			/**/
			ID3D11Texture2D* depthBuffer = 0;
			auto pDSVInfo = (Pg_D3D_ResourceInfo)sys->player[0].pDSVInfo;
			if (pDSVInfo) {
				depthBuffer = pDSVInfo->pTexture;
			}
			pNGX_dlss->DepthBuffer = depthBuffer;

			DLSS_Evaluate();
		}
	}

	/*auto pMVRes = Get_g_xgl_viewXYZ_id_texture();
	if (pMVRes) {
		auto pMVbuffer = pMVRes->pTexture;
		pMVbuffer->GetDesc(&inDesc);

		if (inDesc.Format == DXGI_FORMAT_R16G16B16A16_FLOAT && pMVRes->pSRV) {

			pNGX_dlss->MVBuffer = pMVRes->pTexture;
			dlssReady++;
		}
	}*/
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

	NVSDK_NGX_D3D11_Shutdown1(nullptr);
}

void __fastcall DLSS_Initialization(ID3D11Device** ppDevice, ID3D11DeviceContext** ppImmediateContext, DXGI_SWAP_CHAIN_DESC* pChainDesc) {
	if (pNGX_dlss) return;

	auto p = (PNGX_DLSS)_aligned_malloc(sizeof(NGX_DLSS_t), 16U);
	if (!p) return;

	ZeroMemory(p, sizeof(NGX_DLSS_t));
	pNGX_dlss = p;

	p->Device = *ppDevice;
	p->Context = *ppImmediateContext;
	p->JitterIndex = 1;

	NVSDK_NGX_FeatureCommonInfo featureCommonInfo = {};
	featureCommonInfo.LoggingInfo.DisableOtherLoggingSinks = false;
	featureCommonInfo.LoggingInfo.MinimumLoggingLevel = NVSDK_NGX_LOGGING_LEVEL_VERBOSE;
	/*
	NVSDK_NGX_D3D11_Init(0x3AC09EF, L".", p->Device);
	*/

	auto slresult = NVSDK_NGX_D3D11_Init(231313132, L"Z:\\TEMP", p->Device, &featureCommonInfo); // this id is from ngx_dlss_demo 
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

	NVSDK_NGX_DLSS_Create_Params DlssCreateParams;
	memset(&DlssCreateParams, 0, sizeof(DlssCreateParams));

	DlssCreateParams.Feature.InWidth = pChainDesc->BufferDesc.Width;
	DlssCreateParams.Feature.InHeight = pChainDesc->BufferDesc.Height;
	DlssCreateParams.Feature.InTargetWidth = pChainDesc->BufferDesc.Width;
	DlssCreateParams.Feature.InTargetHeight = pChainDesc->BufferDesc.Height;
	DlssCreateParams.Feature.InPerfQualityValue = NVSDK_NGX_PerfQuality_Value_DLAA;
	DlssCreateParams.InFeatureCreateFlags = NVSDK_NGX_DLSS_Feature_Flags_None;
	DlssCreateParams.InEnableOutputSubrects = false;


	auto ResultDLSS = NGX_D3D11_CREATE_DLSS_EXT(p->Context, &p->m_dlssFeature, p->m_ngxParameters, &DlssCreateParams);
	if (ResultDLSS != NVSDK_NGX_Result_Success) {
		DLSS_TriggerFailureResult((UINT32)ResultDLSS, 1);
		return;
	}

	p->m_bDlssAvailable = 1;

	D3D11_TEXTURE2D_DESC outDesc = {};
	outDesc.Width = pChainDesc->BufferDesc.Width;
	outDesc.Height = pChainDesc->BufferDesc.Height;
	outDesc.MipLevels = 1;
	outDesc.ArraySize = 1;
	outDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	outDesc.SampleDesc.Count = 1;
	outDesc.SampleDesc.Quality = 0;
	outDesc.Usage = D3D11_USAGE_DEFAULT;
	outDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
	outDesc.CPUAccessFlags = 0;
	outDesc.MiscFlags = 0;
	p->Device->CreateTexture2D(&outDesc, 0, &pOutColor);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = outDesc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;
	p->Device->CreateUnorderedAccessView(pOutColor, &uavDesc, &pOutUAV);

	// set black mv buffer
	outDesc.Format = DXGI_FORMAT_R16G16_FLOAT;
	outDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	UINT pixelSize = pChainDesc->BufferDesc.Width * 4;  // 2 bytes (X) + 2 bytes (Y)
	UINT dataSize = pChainDesc->BufferDesc.Height * pixelSize;
	std::vector<BYTE> zeroData(dataSize, 0);
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = zeroData.data();
	initData.SysMemPitch = pixelSize;
	initData.SysMemSlicePitch = dataSize;
	p->Device->CreateTexture2D(&outDesc, &initData, &pBlackMV);


	ID3DBlob* cs_blob = nullptr;
	ID3DBlob* error_blob = nullptr;
	D3DCompileFromFile(L"./subtitle/test.hlsl", nullptr, nullptr, "CS_main", "cs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &cs_blob, &error_blob);
	ID3D11ComputeShader* newCS = nullptr;
	p->Device->CreateComputeShader(cs_blob->GetBufferPointer(), cs_blob->GetBufferSize(), nullptr, &newCS);
	pCSTest = newCS;
	cs_blob->Release();
	if (error_blob) error_blob->Release();
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

	out[0] = DLSS_Halton(pNGX_dlss->JitterIndex, 2);
	out[1] = DLSS_Halton(pNGX_dlss->JitterIndex, 3);
	pNGX_dlss->JitterIndex++;
}

void __fastcall DLSS_Evaluate(){
	if (!pNGX_dlss) return;

	auto ColorBuffer = pNGX_dlss->ColorBuffer;
	auto DepthBuffer = pNGX_dlss->DepthBuffer;
	//auto MVBuffer = pNGX_dlss->MVBuffer;
	if (ColorBuffer && DepthBuffer){
		float jitter[2];
		DLSS_GetJitter(jitter);
		NVSDK_NGX_D3D11_DLSS_Eval_Params D3D11DlssEvalParams;
		memset(&D3D11DlssEvalParams, 0, sizeof(D3D11DlssEvalParams));

		D3D11DlssEvalParams.Feature.pInColor = pOutColor;
		D3D11DlssEvalParams.Feature.pInOutput = ColorBuffer;
		D3D11DlssEvalParams.Feature.InSharpness = 0.3;

		D3D11DlssEvalParams.pInDepth = DepthBuffer;
		D3D11DlssEvalParams.pInMotionVectors = pBlackMV;

		D3D11DlssEvalParams.InJitterOffsetX = jitter[0];
		D3D11DlssEvalParams.InJitterOffsetY = jitter[1];
		D3D11DlssEvalParams.InRenderSubrectDimensions.Width = pNGX_dlss->resolution[0];
		D3D11DlssEvalParams.InRenderSubrectDimensions.Height = pNGX_dlss->resolution[1];

		D3D11DlssEvalParams.InReset = pNGX_dlss->IsReset;
		D3D11DlssEvalParams.InMVScaleX = 1.0;
		D3D11DlssEvalParams.InMVScaleY = 1.0;

		auto result = NGX_D3D11_EVALUATE_DLSS_EXT(pNGX_dlss->Context, pNGX_dlss->m_dlssFeature, pNGX_dlss->m_ngxParameters, &D3D11DlssEvalParams);
		pNGX_dlss->IsReset = 0;
	}

	DLSS_ClearBuffer();
}
