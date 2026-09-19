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

#include <wrl/client.h>
using namespace Microsoft::WRL;

#include "0SL_executor.h"
#include "0SL_wapper.h"

D3D::PStreamLineProcessor g_StreamLineProcessor = nullptr;
PFN_D3D12_CREATE_DEVICE fnD3D12CreateDevice = nullptr;
PFN_D3D12_GET_DEBUG_INTERFACE fnD3D12GetDebugInterface = nullptr;

bool __fastcall streamline_InitializePointers() {
	using namespace D3D;
	if (g_StreamLineProcessor) return false;

	auto hmodD3D12 = LoadLibraryW(L"d3d12.dll");
	if (!hmodD3D12) return false;

	fnD3D12CreateDevice = (PFN_D3D12_CREATE_DEVICE)GetProcAddress(hmodD3D12, "D3D12CreateDevice");
	fnD3D12GetDebugInterface = (PFN_D3D12_GET_DEBUG_INTERFACE)GetProcAddress(hmodD3D12, "D3D12GetDebugInterface");


	auto p = (PStreamLineProcessor)_aligned_malloc(sizeof(StreamLineProcessor_t), 16U);
	if (!p) return false;

	ZeroMemory(p, sizeof(StreamLineProcessor_t));
	g_StreamLineProcessor = p;

	return true;
}

ComPtr<ID3D12Debug> debugController;

bool __fastcall streamline_CreateD3D11On12Device(UINT Flags, ID3D11Device** ppDevice, ID3D11DeviceContext** ppImmediateContext) {
	if(!streamline_InitializePointers()) return false;

	if (SUCCEEDED(fnD3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}

	auto pSL = g_StreamLineProcessor;
	// create dxgi factory
	IDXGIFactory7* dxgiFactory;
	auto hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	if (FAILED(hr)) return false;
	pSL->m_dxgiFactory = dxgiFactory;

	// check hardware adapter
	ComPtr<IDXGIAdapter1> hardwareAdapter;
	for (UINT i = 0; i < 3; ++i) {
		ComPtr<IDXGIAdapter1> adapter;
		if (dxgiFactory->EnumAdapters1(i, &adapter) == DXGI_ERROR_NOT_FOUND)
			break;

		if (SUCCEEDED(fnD3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_1, _uuidof(ID3D12Device), nullptr))) {
			hardwareAdapter = adapter;
			break;
		}
	}
	if (!hardwareAdapter) return false;

	// create d3d12 device
	ID3D12Device* d3d12Device;
	hr = fnD3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&d3d12Device));

	if (FAILED(hr)) return false;
	pSL->m_d3d12Device = d3d12Device;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	hr = d3d12Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pSL->m_commandQueue));
	if (FAILED(hr)) return false;


	// create d3d11on12 device
	hr = D3D11On12CreateDevice(d3d12Device, Flags | D3D11_CREATE_DEVICE_DEBUG, // | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS,
		nullptr, 0,
		reinterpret_cast<IUnknown**>(&pSL->m_commandQueue), 1, 0,
		ppDevice, ppImmediateContext, nullptr);
	if (FAILED(hr)) return false;

	hr = (*ppDevice)->QueryInterface(IID_PPV_ARGS(&pSL->m_d3d11On12Device));

	/**/
	ID3D10Multithread* pMultithread = nullptr;
	if (SUCCEEDED((*ppDevice)->QueryInterface(IID_PPV_ARGS(&pMultithread)))) {
		pMultithread->SetMultithreadProtected(TRUE);
		pMultithread->Release();
	}

	std::wstring hrText = std::format(L"d3d11On12Device result: {0}.", hr);
	MessageBoxW(NULL, hrText.c_str(), L"debug", MB_OK);

	pSL->m_status = D3D::StreamLineProcessorStatus_t::eD3D12;

	return true;
}

bool __fastcall streamline_CreateSwapChain(DXGI_SWAP_CHAIN_DESC* pChainDesc, IDXGISwapChain1** ppSwapChain, IDXGISwapChain1* pD3D11SwapChain){
	if (pD3D11SwapChain) pD3D11SwapChain->Release();

	auto pSL = g_StreamLineProcessor;
	if(!pSL) return false;


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
		ComPtr<ID3D12Resource> backBuffer12;
		pSwapChain->GetBuffer(n, IID_PPV_ARGS(&backBuffer12));

		D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
		pSL->m_d3d11On12Device->CreateWrappedResource(backBuffer12.Get(), &d3d11Flags,
			D3D12_RESOURCE_STATE_COMMON, // D3D12_RESOURCE_STATE_RENDER_TARGET
			D3D12_RESOURCE_STATE_PRESENT,
			IID_PPV_ARGS(&pSL->v_wrappedBackBuffers[n]));
	}


	return true;
}

ID3D11Texture2D* tempBuffer = 0;
ID3D11Texture2D* tempBuffer2 = 0;

HRESULT __fastcall streamline_SwapChainGetBuffer(IDXGISwapChain* pSwapChain, PGameDXGIRender pGameDXGI, ID3D11Texture2D** pOut) {
	auto pSL = g_StreamLineProcessor;
	if (!pSL)// return E_FAIL;
	{
		pSwapChain->GetBuffer(0, IID_PPV_ARGS(&tempBuffer));
		tempBuffer->Release();

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
		outDesc.MiscFlags = 0;
		auto hr1 = device->CreateTexture2D(&outDesc, 0, &tempBuffer2);

		*pOut = tempBuffer2;
		tempBuffer2->AddRef();
		return hr1;
	}

	auto hr = pSL->SwapChainGetBuffer(pGameDXGI, pOut);
	return hr;
}

bool needPresent = true;

void __fastcall streamline_SwapChainPresent(PGameDXGIRender pGameDXGI) {
	if (!needPresent) return;

	auto pSL = g_StreamLineProcessor;
	if (!pSL) {
		auto context = pGameDXGI->pD3D11DeviceContext;
		context->CopyResource(tempBuffer, tempBuffer2);
		return;
	}

	if (pSL->m_status != D3D::StreamLineProcessorStatus_t::eD3D12) return;

	pSL->SwapChainPresent(pGameDXGI);
}

void __fastcall streamline_SwapChainPresentToggle() {
	needPresent = !needPresent;
}

extern "C" extern uintptr_t map_async_obj_initC3650;

bool __fastcall streamline_MapLoadResources(void* saveRcx) {
	/**/
	DWORD_PTR originalProcessMask = 0;
	DWORD_PTR originalSystemMask = 0;
	auto currentProcess = GetCurrentProcess();

	GetProcessAffinityMask(currentProcess, &originalProcessMask, &originalSystemMask);
	SetProcessAffinityMask(currentProcess, 0b100);

	typedef bool(__fastcall* MapLoadResources_t)(void*);
	auto CallFunc = (MapLoadResources_t)map_async_obj_initC3650;
	auto ret = CallFunc(saveRcx);

	/*
	, void* saveRdx, void* saveR8, void* saveR9
	typedef void(__fastcall* MapLoadResources_t)(void*, void*, void*, void*);
	auto CallFunc = (MapLoadResources_t)MapLoadResourcesB2370;
	CallFunc(saveRcx, saveRdx, saveR8, saveR9);
	*/

	SetProcessAffinityMask(currentProcess, originalProcessMask);
	return ret;



	// EDF5.exe+EB09E is the smallest granularity, but the performance is too poor.
	// MapLoadResourcesB2370 = (uintptr_t)(hmodEXE + 0xB2370);

	// EDF5.exe+C21CC
	//hookGameBlockWithInt3((void*)(hmodEXE + 0xC21CC), (uintptr_t)ASMmap_async_obj_init);
	//WriteHookToProcess((void*)(hmodEXE + 0xC21CC + 15), (void*)&nop3, 3U);
	/*map_async_obj_initRetAddr = (uintptr_t)(hmodEXE + 0xC21DE);
	map_async_obj_initC3650 = (uintptr_t)(hmodEXE + 0xC3650); */
	// EDF5.exe+B256C
	/*
	void __fastcall ASMmap_async_obj_init();
	uintptr_t map_async_obj_initRetAddr;
	uintptr_t map_async_obj_initC3650;
	BYTE setThreadCount[] = {
		0x41, 0xB8, 0x02, 0x00, 0x00, 0x00, // mov r8d, 2
		0x0F, 0x1F, 0x40, 0x00
	};
	WriteHookToProcess((void*)(hmodEXE + 0xB256C), &setThreadCount, 10);*/
}
