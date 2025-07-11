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

#include "utiliy.h"
#include "commonNOP.h"
#include "nvsl.h"

#define DEBUGMODE

//#if defined(NVSL)
//#else
//#endif

#if defined(NVSL)
#else
__declspec(align(16)) typedef struct NGX_DLSS_t {
	NVSDK_NGX_Parameter* m_ngxParameters;
	NVSDK_NGX_Handle* m_dlssFeature;
	int m_bDlssAvailable;
} *PNGX_DLSS;
PNGX_DLSS pNGX_dlss = nullptr;
#endif

extern "C" {
	void __fastcall ASMdx11CreateDevice();
	uintptr_t dx11CreateDeviceRetAddr;
	void __fastcall ASMsysExitGame();
	void __fastcall ASMdxgiSwapChainPresent();
	uintptr_t dxgiSwapChainPresentRetAddr;
}
PCID3D11Forwarder pD3D11Forwarder;

void InstallNVIDIAdlss(PBYTE hmodEXE)
{
	// EDF5.exe+5E10F0
	hookGameBlockWithInt3((void*)(hmodEXE + 0x5E10F0), (uintptr_t)ASMdx11CreateDevice);
	WriteHookToProcess((void*)(hmodEXE + 0x5E10F0 + 15), (void*)&nop1, 1U);
	dx11CreateDeviceRetAddr = (uintptr_t)(hmodEXE + 0x5E1100);

	// EDF5.exe+50732A, Sys_Exit_Game
	hookGameBlock((void*)(hmodEXE + 0x50732A), (uintptr_t)ASMsysExitGame);

	// EDF5.exe+5E316E
	hookGameBlockWithInt3((void*)(hmodEXE + 0x5E316E), (uintptr_t)ASMdxgiSwapChainPresent);
	WriteHookToProcess((void*)(hmodEXE + 0x5E316E + 15), (void*)&nop10, 10U);
	dxgiSwapChainPresentRetAddr = (uintptr_t)(hmodEXE + 0x5E3187);

	// EDF5.exe+1256D00
	auto tempP = hmodEXE + 0x1256D00;
	pD3D11Forwarder = (PCID3D11Forwarder)tempP;

#if defined(NVSL)
	sl::Preferences pref{};
	sl::Feature myFeatures[] = { sl::kFeatureDLSS };
	pref.featuresToLoad = myFeatures;
	pref.numFeaturesToLoad = _countof(myFeatures);
	pref.applicationId = 231313132;
	pref.renderAPI = sl::RenderAPI::eD3D11;
	auto slresult = slInit(pref);
	if (slresult != sl::Result::eOk) {
		TriggerDLSSFailureResult((UINT32)slresult + 1000, 0);
	}
#else
	PNGX_DLSS p = (PNGX_DLSS)_aligned_malloc(sizeof(NGX_DLSS_t), 16U);
	if (p) {
		ZeroMemory(p, sizeof(NGX_DLSS_t));
		pNGX_dlss = p;
	}
#endif

	MessageBoxW(NULL, L"Check EDF5.exe+5E10F0", L"info", MB_OK);
	//
}

void __fastcall TriggerDLSSFailureResult(UINT32 slresult, int FreeDLSS)
{
#if defined(DEBUGMODE)
	MessageBoxW(NULL, std::to_wstring((UINT32)slresult).c_str(), L"error", MB_OK);
#endif

#if defined(NVSL)
#else
	if (FreeDLSS) {
		if (pNGX_dlss) {
			_aligned_free(pNGX_dlss);
			pNGX_dlss = nullptr;
		}
	}
#endif
}

HRESULT __stdcall Initialize_NGX_dlss(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext)
{
#if defined(NVSL)
	IDXGISwapChain* pSwapChain = nullptr;
	auto result = D3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, NULL, &pSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);

	auto slresult = slSetD3DDevice(*ppDevice);
	if (slresult != sl::Result::eOk) {
		TriggerDLSSFailureResult((UINT32)slresult + 2000, 1);
	}
#else
	auto result = D3D11CreateDevice(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);

#endif

	if (result < 0) {
		return result;
	}
	// set dlss
	InitializeDLSS(*ppDevice);
	InitializeDLSSFeatures(*ppImmediateContext);
	// pDevice vft+40 is CreateUnorderedAccessView 
	// pDevice vft+48 is CreateRenderTargetView 

	return result;
}

void __fastcall InitializeDLSS(ID3D11Device* InDevice)
{
#if defined(NVSL)

	sl::AdapterInfo adapterInfo{};
	auto slresult = slIsFeatureSupported(sl::kFeatureDLSS, adapterInfo);
	if (slresult != sl::Result::eOk) {
		TriggerDLSSFailureResult((UINT32)slresult + 2200, 0);
	}
#else
	if (!pNGX_dlss) {
		return;
	}

	auto slresult = NVSDK_NGX_D3D11_Init(231313132, L".", InDevice);
	if (slresult != NVSDK_NGX_Result_Success) {
		TriggerDLSSFailureResult((UINT32)slresult - NVSDK_NGX_Result_Fail, 1);
		return;
	}

	slresult = NVSDK_NGX_D3D11_GetCapabilityParameters(&pNGX_dlss->m_ngxParameters);
	if (slresult != NVSDK_NGX_Result_Success) {
		TriggerDLSSFailureResult((UINT32)slresult - NVSDK_NGX_Result_Fail, 1);
		return;
	}
#endif
}

void __fastcall InitializeDLSSFeatures(ID3D11DeviceContext* deviceContext)
{
#if defined(NVSL)
	/*sl::DLSSOptimalSettings dlssSettings;
	auto slresult = slDLSSGetOptimalSettings(dlssOptions, dlssSettings);
	if (slresult != sl::Result::eOk) {
		TriggerDLSSFailureResult((UINT32)slresult + 2100, 1);
	}*/

	UINT numViewports = 1;
	D3D11_VIEWPORT dxViewport;
	deviceContext->RSGetViewports(&numViewports, &dxViewport);
	sl::ViewportHandle m_viewport = { (UINT32)GetActiveWindow() };

	sl::DLSSOptions dlssOptions = {};
	dlssOptions.mode = sl::DLSSMode::eUltraPerformance;
	dlssOptions.outputWidth = 1920;
	dlssOptions.outputHeight = 1080;
	dlssOptions.colorBuffersHDR = sl::Boolean::eFalse;

	auto slresult = slDLSSSetOptions(m_viewport, dlssOptions);
	if (slresult != sl::Result::eOk) {
		TriggerDLSSFailureResult((UINT32)slresult + 2100, 1);
	}


#else
	PNGX_DLSS p = pNGX_dlss;
	if (!p) {
		return;
	}

	unsigned int CreationNodeMask = 1;
	unsigned int VisibilityNodeMask = 1;
	NVSDK_NGX_Result ResultDLSS = NVSDK_NGX_Result_Fail;

	int MotionVectorResolutionLow = 1; // we let the Snippet do the upsampling of the motion vector
	// Next create features	
	int DlssCreateFeatureFlags = NVSDK_NGX_DLSS_Feature_Flags_None;
	DlssCreateFeatureFlags |= MotionVectorResolutionLow ? NVSDK_NGX_DLSS_Feature_Flags_MVLowRes : 0;

	NVSDK_NGX_Parameter_SetUI(p->m_ngxParameters, NVSDK_NGX_Parameter_DLSS_Hint_Render_Preset_DLAA, 0);              // will remain the chosen weights after OTA
	NVSDK_NGX_Parameter_SetUI(p->m_ngxParameters, NVSDK_NGX_Parameter_DLSS_Hint_Render_Preset_Quality, 0);           // ^
	NVSDK_NGX_Parameter_SetUI(p->m_ngxParameters, NVSDK_NGX_Parameter_DLSS_Hint_Render_Preset_Balanced, 0);          // ^
	NVSDK_NGX_Parameter_SetUI(p->m_ngxParameters, NVSDK_NGX_Parameter_DLSS_Hint_Render_Preset_Performance, 0);       // ^
	NVSDK_NGX_Parameter_SetUI(p->m_ngxParameters, NVSDK_NGX_Parameter_DLSS_Hint_Render_Preset_UltraPerformance, 0);

	NVSDK_NGX_DLSS_Create_Params DlssCreateParams;
	memset(&DlssCreateParams, 0, sizeof(DlssCreateParams));

	DlssCreateParams.Feature.InWidth = 1920 / 2;
	DlssCreateParams.Feature.InHeight = 1080 / 2;
	DlssCreateParams.Feature.InTargetWidth = 1920;
	DlssCreateParams.Feature.InTargetHeight = 1080;
	DlssCreateParams.Feature.InPerfQualityValue = NVSDK_NGX_PerfQuality_Value_UltraPerformance;
	DlssCreateParams.InFeatureCreateFlags = DlssCreateFeatureFlags;


	ResultDLSS = NGX_D3D11_CREATE_DLSS_EXT(deviceContext, &p->m_dlssFeature, p->m_ngxParameters, &DlssCreateParams);
	if (ResultDLSS != NVSDK_NGX_Result_Success) {
		TriggerDLSSFailureResult((UINT32)ResultDLSS, 0);
		return;
	}

	p->m_bDlssAvailable = 1;
#endif
}

void __fastcall Evaluate_NGX_dlss(ID3D11DeviceContext* d3dcontext, ID3D11Device* InDevice, IDXGISwapChain* pSwapChain)
{
#if defined(NVSL)
	sl::ViewportHandle myViewport = { (UINT32)GetActiveWindow() };
	const sl::BaseStructure* inputs[] = { &myViewport };

	sl::FrameToken* currentFrame{};
	slGetNewFrameToken(currentFrame);

	sl::ResourceTag depthTag = sl::ResourceTag{ nullptr, sl::kBufferTypeDepth, sl::ResourceLifecycle::eValidUntilPresent };
	slSetTagForFrame(*currentFrame, myViewport, &depthTag, 1, 0);

	sl::Constants consts = {};
	consts.mvecScale = { 1,1 };
	slSetConstants(consts, *currentFrame, myViewport);

	auto slresult = slEvaluateFeature(sl::kFeatureDLSS, *currentFrame, inputs, _countof(inputs), 0);
	static int initialized = 0;
	if (!initialized) {
		if (slresult != sl::Result::eOk) {
			TriggerDLSSFailureResult((UINT32)slresult + 9900, 1);
		}
	}

	initialized++;
	if (initialized > 6000) {
		initialized = 0;
	}
#else 
	if (!pNGX_dlss) {
		return;
	}

	NVSDK_NGX_D3D11_DLSS_Eval_Params D3D11DlssEvalParams;
	memset(&D3D11DlssEvalParams, 0, sizeof(D3D11DlssEvalParams));

	ID3D11DepthStencilView* pDepthStencilView = pD3D11Forwarder->pDepthStencilView;
	if (pDepthStencilView) {
		ID3D11Resource* pDepthResource;
		pDepthStencilView->GetResource(&pDepthResource);
		D3D11DlssEvalParams.pInDepth = pDepthResource;
	}

	ID3D11RenderTargetView* pRenderTargetView1 = pD3D11Forwarder->pRenderTargetView[0];
	if (pRenderTargetView1) {
		ID3D11Resource* pColorResource1;
		pRenderTargetView1->GetResource(&pColorResource1);
		D3D11DlssEvalParams.Feature.pInColor = pColorResource1;
		D3D11DlssEvalParams.Feature.pInOutput = pColorResource1;
	}
	// NVSL.dll+1CD9 

	//D3D11DlssEvalParams.Feature.pInColor = unresolvedColorResource;
	//D3D11DlssEvalParams.Feature.pInOutput = resolvedColorResource;
	D3D11DlssEvalParams.InJitterOffsetX = 10;
	D3D11DlssEvalParams.InJitterOffsetY = 10;
	D3D11DlssEvalParams.InRenderSubrectDimensions = { 1920, 1080 };
	D3D11DlssEvalParams.InReset = 1;
	D3D11DlssEvalParams.InMVScaleX = 0.25;
	D3D11DlssEvalParams.InMVScaleY = 0.25;
	D3D11DlssEvalParams.InColorSubrectBase = { 0,0 };
	D3D11DlssEvalParams.InDepthSubrectBase = { 0,0 };
	D3D11DlssEvalParams.InTranslucencySubrectBase = { 0,0 };
	D3D11DlssEvalParams.InMVSubrectBase = { 0,0 };

	auto slresult = NGX_D3D11_EVALUATE_DLSS_EXT(d3dcontext, pNGX_dlss->m_dlssFeature, pNGX_dlss->m_ngxParameters, &D3D11DlssEvalParams);
	/*ID3D11DeviceContext* new_d3dcontext;
	InDevice->GetImmediateContext(&new_d3dcontext);
	NGX_D3D11_EVALUATE_DLSS_EXT(new_d3dcontext, pNGX_dlss->m_dlssFeature, pNGX_dlss->m_ngxParameters, &D3D11DlssEvalParams);*/
	static int initialized = 0;
	if (!initialized) {
		if (slresult != NVSDK_NGX_Result_Success) {
			TriggerDLSSFailureResult((UINT32)slresult - NVSDK_NGX_Result_Fail, 0);
			initialized = 1;

			/*DXGI_SWAP_CHAIN_DESC chainDesc = { 0 };
			pSwapChain->GetDesc(&chainDesc);
			chainDesc.BufferDesc.Width /= 2;
			chainDesc.BufferDesc.Height /= 2;
			chainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
			pSwapChain->ResizeTarget(&chainDesc.BufferDesc);*/
		}
	}
#endif
}

void __fastcall Release_NGX_dlss()
{
#if defined(NVSL)
	slShutdown();
#else
	if (pNGX_dlss) {
		NVSDK_NGX_D3D11_DestroyParameters(pNGX_dlss->m_ngxParameters);
		if (pNGX_dlss->m_bDlssAvailable) {
			NVSDK_NGX_D3D11_ReleaseFeature(pNGX_dlss->m_dlssFeature);
		}

		_aligned_free(pNGX_dlss);
		pNGX_dlss = nullptr;
	}

	NVSDK_NGX_D3D11_Shutdown1(nullptr);
#endif

#if defined(DEBUGMODE)
	//MessageBoxW(NULL, L"done!", L"check", MB_OK);
#endif
}
