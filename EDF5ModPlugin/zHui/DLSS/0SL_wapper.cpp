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
#include <filesystem>

#include "Base/g_system.h"
#include "0SL_executor.h"
#include "0SL_PostProcess.h"
#include "0SL_wapper.h"

//#define hasDX12Debug

PFN_D3D12_CREATE_DEVICE fnD3D12CreateDevice = nullptr;
PFN_D3D12_GET_DEBUG_INTERFACE fnD3D12GetDebugInterface = nullptr;

D3D::PStreamLineProcessor g_StreamLineProcessor = nullptr;
D3D::PAddPostProcess g_AddPostProcess = nullptr;
// 0 is PostProcess, 1 is DLAA, 2 is DLSSFG
bool g_bPostProcess[4] = { false, true, true, true };

extern "C"{
	extern int Config_DLAA;
	extern int Config_OnDX12;
}

bool __fastcall streamline_InitializeD3D12(){
	// check no dxvk
	if (std::filesystem::exists(L"./dxgi.dll")) return false;
	if (!Config_OnDX12) return false;

	auto hmodD3D12 = LoadLibraryW(L"d3d12.dll");
	if (!hmodD3D12) return false;

	fnD3D12CreateDevice = (PFN_D3D12_CREATE_DEVICE)GetProcAddress(hmodD3D12, "D3D12CreateDevice");
	fnD3D12GetDebugInterface = (PFN_D3D12_GET_DEBUG_INTERFACE)GetProcAddress(hmodD3D12, "D3D12GetDebugInterface");

	return true;
}

bool __fastcall streamline_InitializeSLPointers() {
	using namespace D3D;
	if (g_StreamLineProcessor) return false;

	bool bD3D12 = streamline_InitializeD3D12();


	auto p = (PStreamLineProcessor)_aligned_malloc(sizeof(StreamLineProcessor_t), 16U);
	if (!p) return false;

	ZeroMemory(p, sizeof(StreamLineProcessor_t));
	g_StreamLineProcessor = p;

	if (bD3D12) {
		p->m_status = StreamLineProcessorStatus_t::eD3D12;
	} else {
		p->m_status = StreamLineProcessorStatus_t::eD3D11;
	}

	return true;
}



void __fastcall streamline_CreateD3D12Device(){
	if (!streamline_InitializeSLPointers()) return;

#if defined(hasDX12Debug)
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(fnD3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->D3D_Release();
	}
#endif	

	auto pSL = g_StreamLineProcessor;
	if (pSL->m_status != D3D::StreamLineProcessorStatus_t::eD3D12) return;

	pSL->m_status = D3D::StreamLineProcessorStatus_t::eFailed;
	pSL->CreateD3D12Device();
}

void __fastcall streamline_InitializePostProcess(ID3D11Device* device, ID3D11DeviceContext* context) {
	using namespace D3D;
	if (g_AddPostProcess) return;

	auto p = (PAddPostProcess)_aligned_malloc(sizeof(AddPostProcess_t), 16U);
	if (!p) return;

	ZeroMemory(p, sizeof(AddPostProcess_t));
	g_AddPostProcess = p;

	p->Initialize(device, context);
	g_bPostProcess[0] = true;
}

void __fastcall streamline_Initialize(ID3D11Device* device, ID3D11DeviceContext* context){
	streamline_InitializeSLPointers();

	auto pSL = g_StreamLineProcessor;
	if (!pSL) return;

	streamline_InitializePostProcess(device, context);
	auto pPP = g_AddPostProcess;
	if (!pPP) return;
}

bool __fastcall streamline_CreateSwapChain(DXGI_SWAP_CHAIN_DESC* pChainDesc, IDXGISwapChain1** ppSwapChain, IDXGISwapChain1* pD3D11SwapChain){
	if (pD3D11SwapChain) pD3D11SwapChain->Release();

	auto pSL = g_StreamLineProcessor;
	if(!pSL) return false;
	if (pSL->m_status != D3D::StreamLineProcessorStatus_t::eD3D12) return false;

	if (pSL->m_dxgiSwapChain3) {
		pSL->m_dxgiSwapChain3->Release();
		pSL->m_dxgiSwapChain3 = nullptr;
	}


	int backBufferCount = D3D12WrappedBackBuffersCount;


	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = pChainDesc->BufferDesc.Width;
	swapChainDesc.Height = pChainDesc->BufferDesc.Height;
	swapChainDesc.Format = pChainDesc->BufferDesc.Format;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = pChainDesc->BufferUsage;
	swapChainDesc.BufferCount = backBufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = pChainDesc->Flags;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
	fullScreenDesc.RefreshRate = pChainDesc->BufferDesc.RefreshRate;
	fullScreenDesc.ScanlineOrdering = pChainDesc->BufferDesc.ScanlineOrdering;
	fullScreenDesc.Scaling = pChainDesc->BufferDesc.Scaling;
	fullScreenDesc.Windowed = pChainDesc->Windowed;

	*ppSwapChain = nullptr;
	auto hr = pSL->m_dxgiFactory->CreateSwapChainForHwnd(pSL->m_commandQueue, pChainDesc->OutputWindow, &swapChainDesc, 0, 0, ppSwapChain);

	//std::wstring hrText = std::format(L"CreateSwapChainForHwnd result: {0}.\nold BuffersCount : {0}", hr, pChainDesc->BufferCount);
	//MessageBoxW(NULL, hrText.c_str(), L"debug", MB_OK);
	if (FAILED(hr)) return false;

	auto pSwapChain = *ppSwapChain;

	// get swap chain3
	pSwapChain->QueryInterface(IID_PPV_ARGS(&pSL->m_dxgiSwapChain3));

	for (UINT n = 0; n < backBufferCount; n++) {
		pSwapChain->GetBuffer(n, IID_PPV_ARGS(&pSL->v_d3d12BackBuffers[n]));
		if (pSL->v_d3d12BackBuffers[n]) pSL->v_d3d12BackBuffers[n]->Release();
	}

	return true;
}

#if defined(hasDX12Debug)
ID3D11Texture2D* tempBuffer = 0;
ID3D11Texture2D* tempBuffer2 = 0;
#endif

HRESULT __fastcall streamline_SwapChainGetBuffer(IDXGISwapChain* pSwapChain, PGameDXGIRender pGameDXGI, ID3D11Texture2D** pOut) {
	auto pSL = g_StreamLineProcessor;
	if (!pSL || pSL->m_status != D3D::StreamLineProcessorStatus_t::eD3D12) {
#if defined(hasDX12Debug)
		pSwapChain->GetBuffer(0, IID_PPV_ARGS(&tempBuffer));
		tempBuffer->D3D_Release();

		auto device = pGameDXGI->pD3D11Device;

		auto dufferDesc = &pGameDXGI->DXGISwapChainDesc.BufferDesc;
		D3D11_TEXTURE2D_DESC outDesc = {};
		outDesc.Width = dufferDesc->Width;
		outDesc.Height = dufferDesc->Height;
		outDesc.MipLevels = 1;
		outDesc.ArraySize = 1;
		outDesc.Format = dufferDesc->Format;
		outDesc.SampleDesc.Count = 1;
		outDesc.SampleDesc.Quality = 0;
		outDesc.Usage = D3D11_USAGE_DEFAULT;
		outDesc.BindFlags = D3D11_BIND_RENDER_TARGET; // | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		outDesc.CPUAccessFlags = 0;
		outDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_NTHANDLE | D3D11_RESOURCE_MISC_SHARED;
		auto hr1 = device->CreateTexture2D(&outDesc, 0, &tempBuffer2);

		*pOut = tempBuffer2;
		tempBuffer2->AddRef();
		return hr1;
#else
		auto hr = pSwapChain->GetBuffer(0, IID_PPV_ARGS(pOut));
		return hr;
#endif
	}

	auto hr = pSL->SwapChainGetBuffer(pGameDXGI, pOut);
	return hr;
}

void __fastcall streamline_SwapChainPresent(PGameDXGIRender pGameDXGI) {
	auto pSL = g_StreamLineProcessor;
	if (!pSL) {
#if defined(hasDX12Debug)
		auto context = pGameDXGI->pD3D11DeviceContext;
		context->CopyResource(tempBuffer, tempBuffer2);
#endif
		return;
	}

	pSL->SwapChainPresent(pGameDXGI);
}

void __fastcall streamline_Release() {
	using namespace D3D;
	auto pPP = g_AddPostProcess;
	auto pSL = g_StreamLineProcessor;

	if (pPP) {
		void* deviceVK = 0;
		//if (pSL)
		pPP->Buffer_Release(deviceVK);
		_aligned_free(pPP);
		g_AddPostProcess = nullptr;
	}

	if (pSL) {
		pSL->Release();
		_aligned_free(pSL);
		g_StreamLineProcessor = nullptr;
	}
}

void __fastcall streamline_SetFeature(int playerCount) {
	using namespace D3D;
	auto pPP = g_AddPostProcess;
	if (!pPP) return;

	if (playerCount != -1) {
		if (pPP->m_playerCount == playerCount) return;
		pPP->m_playerCount = playerCount;
	} else {
		playerCount = pPP->m_playerCount;
	}

	auto pSL = g_StreamLineProcessor;
	ID3D12Device* device12 = 0;
	void* deviceVK = 0;
	if (pSL) {
		if (pSL->m_status == StreamLineProcessorStatus_t::eD3D12)
			device12 = pSL->m_d3d12Device;
		//else if (pSL->m_status == StreamLineProcessorStatus_t::eVULKAN)
		//	isVK = 1;
	}

	auto pDXGI = DXGI_GetGameDXGIRender();
	auto pTexBack = pDXGI->pTextureBackBuffer;
	auto pBuffer = pTexBack->pInfo;

	AddPostProcessRes_t res;
	res.Width = pBuffer->width;
	res.Height = pBuffer->height;
	if (playerCount == 2) {
		res.Width /= 2;
	}

	UINT DLSS_Level = 0;
	pPP->Buffer_Release(deviceVK);
	pPP->Buffer_Create(pDXGI->pD3D11Device, res, device12, DLSS_Level);

	if (pSL){
		if (playerCount == 2) {
			pSL->m_bIsSplitScreen = true;
		} else {
			pSL->m_bIsSplitScreen = false;
		}
	}
	// end
}

void* __fastcall streamline_ExecuteSR(Pg_D3D11DeviceInfo pD3D, int OutOffset, void* saveRCX) {
	if (OutOffset) return saveRCX;

	using namespace D3D;
	auto pPP = g_AddPostProcess;
	auto pSL = g_StreamLineProcessor;
	if (!pPP) return saveRCX;

	auto playerCount = pPP->m_playerCount;

	auto pSys = XGS_GetXGSSystemPointer();
	PXGS_System_Player player[2];
	player[0] = &pSys->player[0];
	player[1] = &pSys->player[1];

	AddPostProcessRes_t threadGroupCount;
	threadGroupCount.Width = (pPP->m_resolution.Width + 15) / 16;
	threadGroupCount.Height = (pPP->m_resolution.Height + 15) / 16;

	ID3D11DeviceContext* context = pD3D->context;
	Pg_D3D_ResourceInfo pColorToMV;
	Pg_D3D_ResourceInfo pDepthToMV;

	for (int i = 0; i < playerCount; i++) {
		auto pColorRes = (Pg_D3D_ResourceInfo)player[i]->pDrawColorInfo;
		auto pDSVInfo = (Pg_D3D_ResourceInfo)player[i]->pDSVInfo;
		if (i == 0) {
			pColorToMV = pColorRes;
			pDepthToMV = pDSVInfo;
		}

		if (g_bPostProcess[0]) {
			pPP->Execute(pD3D, i, threadGroupCount, &pColorRes->pSRV, &pDSVInfo->pSRV);
		} else if (pSL) {
			context->CopyResource(pPP->m_ColorRes[i].d11, pColorRes->pTexture);
		} else { return saveRCX; }

		context->CopyResource(pColorRes->pTexture, pPP->m_ColorRes[i].d11);
		if (i > 0) return saveRCX;
	}

	return saveRCX;
}
