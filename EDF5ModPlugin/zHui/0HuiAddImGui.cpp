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
#include "ToGui/0GetDXGI.h"
#include "ToGui/0SetImGui.h"
#include "ToGui/HUiHudPowerGuage.h"
#include "ToGui/HUiHudWeapon.h"
#include "ToGui/2SubtitleRenderer.h"
#include "DLSS/0SetDLSS.h"
#include "DLSS/1FullAO.h"

#include "0HuiAddImGui.h"

extern "C" {
	extern int Config_HUDEnhance;
	extern int Config_PostProcess;
	extern int Config_DLAA;
	extern int Config_EnhanceAO;

	void __fastcall ASMdx11CreateDevice();
	uintptr_t dx11CreateDeviceRetAddr;
	void __fastcall ASMsysExitGame();
	void __fastcall ASMgetPlayerCountInHQ(); // HUiHQCurrentStatus

	void __fastcall ASMRenderBufferToScreenBuffer();
	uintptr_t RenderBufferToScreenBufferRetAddr;

	void __fastcall ASMGetDXGISwapChain();
	uintptr_t GetDXGISwapChainRetAddr;

	void __fastcall ASMxgsSystemSetPlayerSlot();
	uintptr_t xgsSystemSetPlayerSlotRetAddr;

	// wait remove ASMrecordPlayerDamage
	void __fastcall ASMgetInflictDamageFromDamageFunc();
	uintptr_t getInflictDamageFromDamageFuncRetAddress;
}

void module_InitializeAddImGui(PBYTE hmodEXE)
{
	DXGI_Initialize(hmodEXE);

	if (Config_DLAA || Config_PostProcess){
		//_putenv_s("DXVK_ENABLE_NVAPI", "1");
		//_putenv_s("DXVK_HUD", "full");
		// EDF5.exe+5E10F0
		hookGameBlockWithInt3((void*)(hmodEXE + 0x5E10F0), (uintptr_t)ASMdx11CreateDevice);
		WriteHookToProcess((void*)(hmodEXE + 0x5E10F0 + 15), (void*)&nop1, 1U);
		dx11CreateDeviceRetAddr = (uintptr_t)(hmodEXE + 0x5E1100);

		// EDF5.exe+50732A, Sys_Exit_Game
		hookGameBlock((void*)(hmodEXE + 0x50732A), (uintptr_t)ASMsysExitGame);

		// EDF5.exe+510A1F
		hookGameBlockWithInt3((void*)(hmodEXE + 0x510A1F), (uintptr_t)ASMgetPlayerCountInHQ);
		WriteHookToProcess((void*)(hmodEXE + 0x510A1F + 15), (void*)&nop3, 3U);

		// EDF5.exe+5ED031
		hookGameBlockWithInt3((void*)(hmodEXE + 0x5ED031), (uintptr_t)ASMRenderBufferToScreenBuffer);
		RenderBufferToScreenBufferRetAddr = (uintptr_t)(hmodEXE + 0x5ED065);
	}

	if (Config_EnhanceAO) {
		HookFunction_D3D11_FullAO();
	}

	//MessageBoxW(NULL, L"test", L"debug", MB_OK);

	// ========================================================================
	// Next, all features are only available when HUD enhancement is enabled.
	if (!Config_HUDEnhance) return;

	// EDF5.exe+5E1BB9
	hookGameBlockWithInt3((void*)(hmodEXE + 0x5E1BB9), (uintptr_t)ASMGetDXGISwapChain);
	WriteHookToProcess((void*)(hmodEXE + 0x5E1BB9 + 15), (void*)&nop1, 1U);
	GetDXGISwapChainRetAddr = (uintptr_t)(hmodEXE + 0x5E1BCE);

	// EDF5.exe+613E80
	hookGameBlock14((void*)(hmodEXE + 0x613E80), (uintptr_t)ASMxgsSystemSetPlayerSlot);
	xgsSystemSetPlayerSlotRetAddr = (uintptr_t)(hmodEXE + 0x613E80 + 14);

	// EDF5.exe+2DB61F
	hookGameBlockWithInt3((void*)(hmodEXE + 0x2DB61F), (uintptr_t)ASMgetInflictDamageFromDamageFunc);
	WriteHookToProcess((void*)(hmodEXE + 0x2DB61F + 15), (void*)&nop3, 3U);
	getInflictDamageFromDamageFuncRetAddress = (uintptr_t)(hmodEXE + 0x2DB77C);

	module_UpdateHUiHudPowerGuage(hmodEXE);
	module_UpdateHUiHudWeapon(hmodEXE);
	DigitRenderer::SubtitleRenderer_Initialization(hmodEXE);
}

typedef HRESULT(WINAPI* ID3D11Device_CreateTexture2D)(ID3D11Device* pDevice, const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D);
ID3D11Device_CreateTexture2D fnID3D11Device_CreateTexture2D = 0;

typedef void(__stdcall* ID3D11DeviceContext_OMSetRenderTargets)(ID3D11DeviceContext* pContext, UINT NumViews, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView);
ID3D11DeviceContext_OMSetRenderTargets fnID3D11DeviceContext_OMSetRenderTargets = 0;

HRESULT WINAPI module_InitializeD3D11(DXGI_SWAP_CHAIN_DESC* pChainDesc, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext)
{
	//auto debugText = std::format(L"SampleDesc, count: {0}", pChainDesc->SampleDesc.Count);
	//MessageBoxW(NULL, debugText.c_str(), L"debug", MB_OK); 

	auto result = D3D11CreateDevice(0, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);
	if (result < 0) return result;

	if (Config_DLAA || Config_PostProcess) {
		pChainDesc->SampleDesc.Count = 1; // old is 8
		pChainDesc->SampleDesc.Quality = 0;

		if (!fnID3D11Device_CreateTexture2D) {
			// d3d11.dll+19610
			auto pVTable = *(ID3D11Device_CreateTexture2D**)(*ppDevice);
			auto addrCreateTexture2D = &pVTable[5]; // vft+0x28
			fnID3D11Device_CreateTexture2D = *addrCreateTexture2D;
			//auto addrToHook = (uintptr_t)New_CreateTexture2D;
			//WriteHookToProcess(addrCreateTexture2D, &addrToHook, 8U);
		}
		if (!fnID3D11DeviceContext_OMSetRenderTargets) {
			auto pVTable = *(ID3D11DeviceContext_OMSetRenderTargets**)(*ppImmediateContext);
			auto addrOMSetRenderTargets = &pVTable[33]; // vft+0x108
			fnID3D11DeviceContext_OMSetRenderTargets = *addrOMSetRenderTargets;

			//auto addrToHook = (uintptr_t)New_OMSetRenderTargets;
			//WriteHookToProcess(addrOMSetRenderTargets, &addrToHook, 8U);
		}

		DLSS_Initialization(ppDevice, ppImmediateContext, pChainDesc);
	}

	return result;
}

HRESULT WINAPI New_CreateTexture2D(ID3D11Device* pDevice, const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D) {

	if (pDesc->Usage != D3D11_USAGE_DEFAULT) {
	oldMethod:
		return fnID3D11Device_CreateTexture2D(pDevice, pDesc, pInitialData, ppTexture2D);
	}

	if (!pDesc->Format || pDesc->Format > DXGI_FORMAT_R16_SINT) goto oldMethod;

	if (!(pDesc->BindFlags & (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL))) goto oldMethod;
	if (pDesc->MipLevels > 1 && !(pDesc->MiscFlags & D3D11_RESOURCE_MISC_GENERATE_MIPS)) goto oldMethod;
	if (pDesc->SampleDesc.Count > 1 || pDesc->ArraySize > 1) goto oldMethod;

	D3D11_TEXTURE2D_DESC newDesc;
	memcpy(&newDesc, pDesc, sizeof(D3D11_TEXTURE2D_DESC));
	newDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	auto result = fnID3D11Device_CreateTexture2D(pDevice, &newDesc, pInitialData, ppTexture2D);
	return result;
}

void New_OMSetRenderTargets(ID3D11DeviceContext* pContext, UINT NumViews, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView) {
	
	//DLSS_GetBuffer(pContext, NumViews, ppRenderTargetViews, pDepthStencilView);
	return fnID3D11DeviceContext_OMSetRenderTargets(pContext, NumViews, ppRenderTargetViews, pDepthStencilView);

}
