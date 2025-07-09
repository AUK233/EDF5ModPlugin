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
//#include "include/sl_hooks.h"
//#include "include/sl_dlss.h"
//#pragma comment(lib, "./nvsl/include/sl.interposer.lib")

#define DEBUGMODE



typedef struct NGX_DLSS_t {
	NVSDK_NGX_Parameter* m_ngxParameters;
	NVSDK_NGX_Handle* m_dlssFeature;
	int m_bDlssAvailable;
} *PNGX_DLSS;
PNGX_DLSS pNGX_dlss = nullptr;

extern "C" {
	void __fastcall ASMdx11CreateDevice();
	uintptr_t dx11CreateDeviceRetAddr;
	void __fastcall ASMsysExitGame();
	void __fastcall ASMdx11EvaluateInMission();
	uintptr_t dx11EvaluateInMissionRetAddr;
}

void InstallNVIDIAdlss(PBYTE hmodEXE)
{
	// EDF5.exe+5E11BF
	/*hookGameBlockWithInt3((void*)(hmodEXE + 0x5E11BF), (uintptr_t)ASMdx11CreateDevice);
	WriteHookToProcess((void*)(hmodEXE + 0x5E11BF + 15), (void*)&nop3, 3U);
	dx11CreateDeviceRetAddr = (uintptr_t)(hmodEXE + 0x5E11D1);*/
	// EDF5.exe+5E10F0
	hookGameBlockWithInt3((void*)(hmodEXE + 0x5E10F0), (uintptr_t)ASMdx11CreateDevice);
	WriteHookToProcess((void*)(hmodEXE + 0x5E10F0 + 15), (void*)&nop1, 1U);
	dx11CreateDeviceRetAddr = (uintptr_t)(hmodEXE + 0x5E1100);

	// EDF5.exe+50732A, Sys_Exit_Game
	hookGameBlock((void*)(hmodEXE + 0x50732A), (uintptr_t)ASMsysExitGame);

	// EDF5.exe+5F0467
	hookGameBlockWithInt3((void*)(hmodEXE + 0x5F0467), (uintptr_t)ASMdx11EvaluateInMission);
	WriteHookToProcess((void*)(hmodEXE + 0x5F0467 + 15), (void*)&nop10, 10U);
	dx11EvaluateInMissionRetAddr = (uintptr_t)(hmodEXE + 0x5F0480);

	/*sl::Preferences pref;
	auto slresult = slInit(pref);
	if (slresult != sl::Result::eOk) {
		MessageBoxW(NULL, std::to_wstring((UINT32)slresult).c_str(), L"error", MB_OK);
	}*/

	PNGX_DLSS p = (PNGX_DLSS)_aligned_malloc(sizeof(NGX_DLSS_t), 16U);
	if (p) {
		ZeroMemory(p, sizeof(NGX_DLSS_t));
		pNGX_dlss = p;
	}
}

void __fastcall TriggerDLSSFailureResult(UINT32 slresult, int FreeDLSS)
{
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

HRESULT __stdcall Initialize_NGX_dlss(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext)
{

	//IDXGISwapChain* pSwapChain = nullptr;
	//auto result = D3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, NULL, &pSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);

	
	/*sl::DLSSOptimalSettings dlssSettings;
	sl::DLSSOptions dlssOptions;

	dlssOptions.mode = sl::DLSSMode::eUltraPerformance;
	dlssOptions.outputWidth = 1920;
	dlssOptions.outputHeight = 1080;
	auto slresult = slDLSSGetOptimalSettings(dlssOptions, dlssSettings);
	if (slresult != sl::Result::eOk) {;
		MessageBoxW(NULL, std::to_wstring((UINT32)slresult).c_str(), L"error", MB_OK);
	}*/

	auto result = D3D11CreateDevice(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);
	if (result < 0) {
		return result;
	}

	// set dlss
	InitializeDLSS(*ppDevice);
	InitializeDLSSFeatures(*ppImmediateContext);

	return result;
}

void __fastcall InitializeDLSS(ID3D11Device* InDevice)
{
	if (!pNGX_dlss) {
		return;
	}

	auto slresult = NVSDK_NGX_D3D11_Init(231313132, L".", InDevice);
	if (slresult != NVSDK_NGX_Result_Success) {
		TriggerDLSSFailureResult((UINT32)slresult, 1);
		return;
	}

	slresult = NVSDK_NGX_D3D11_GetCapabilityParameters(&pNGX_dlss->m_ngxParameters);
	if (slresult != NVSDK_NGX_Result_Success) {
		TriggerDLSSFailureResult((UINT32)slresult, 1);
		return;
	}
}

void __fastcall InitializeDLSSFeatures(ID3D11DeviceContext* deviceContext)
{
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

	DlssCreateParams.Feature.InWidth = 1920/2;
	DlssCreateParams.Feature.InHeight = 1080/2;
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
}

void __fastcall Evaluate_NGX_dlss(ID3D11DeviceContext* d3dcontext, ID3D11Device* InDevice)
{
	NVSDK_NGX_D3D11_DLSS_Eval_Params D3D11DlssEvalParams;

	memset(&D3D11DlssEvalParams, 0, sizeof(D3D11DlssEvalParams));

	D3D11DlssEvalParams.InJitterOffsetX = 0.1;
	D3D11DlssEvalParams.InJitterOffsetY = 0.1;
	D3D11DlssEvalParams.InReset = 1;
	D3D11DlssEvalParams.InMVScaleX = 0.25;
	D3D11DlssEvalParams.InMVScaleY = 0.25;

	NGX_D3D11_EVALUATE_DLSS_EXT(d3dcontext, pNGX_dlss->m_dlssFeature, pNGX_dlss->m_ngxParameters, &D3D11DlssEvalParams);
	/*ID3D11DeviceContext* new_d3dcontext;
	InDevice->GetImmediateContext(&new_d3dcontext);
	NGX_D3D11_EVALUATE_DLSS_EXT(new_d3dcontext, pNGX_dlss->m_dlssFeature, pNGX_dlss->m_ngxParameters, &D3D11DlssEvalParams);*/
}

void __fastcall Release_NGX_dlss()
{
	if (pNGX_dlss) {
		NVSDK_NGX_D3D11_DestroyParameters(pNGX_dlss->m_ngxParameters);
		if (pNGX_dlss->m_bDlssAvailable) {
			NVSDK_NGX_D3D11_ReleaseFeature(pNGX_dlss->m_dlssFeature);
		}

		_aligned_free(pNGX_dlss);
		pNGX_dlss = nullptr;
	}

	NVSDK_NGX_D3D11_Shutdown1(nullptr);

#if defined(DEBUGMODE)
	//MessageBoxW(NULL, L"done!", L"check", MB_OK);
#endif
}
