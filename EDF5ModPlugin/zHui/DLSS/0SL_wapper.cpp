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
#define SLDEBUG

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

void __fastcall streamline_TriggerFailureResult(UINT32 slresult, void* sl) {
#if defined(SLDEBUG)
	MessageBoxW(NULL, std::to_wstring((UINT32)slresult).c_str(), L"error", MB_OK);
#endif

	if (sl) {
		auto pSL = (D3D::PStreamLineProcessor)sl;
		pSL->m_renderAPI = 0;
	}
}

void __fastcall streamline_InitializeSLPointers() {
	using namespace D3D;
	if (g_StreamLineProcessor) return;

	auto p = (PStreamLineProcessor)_aligned_malloc(sizeof(StreamLineProcessor_t), 16U);
	if (!p) return;

	ZeroMemory(p, sizeof(StreamLineProcessor_t));
	g_StreamLineProcessor = p;

	if (Config_OnDX12) {
		p->m_status = StreamLineProcessorStatus_t::eD3D12;
	} else {
		if (std::filesystem::exists(L"./dxgi.dll")) {
			p->m_status = StreamLineProcessorStatus_t::eVULKAN;
		} else {
			p->m_status = StreamLineProcessorStatus_t::eD3D11;
		}
	}

	p->m_renderAPI = (int)p->m_status;
	auto renderAPI = p->m_status;

	p->myViewport[0] = sl::ViewportHandle{ 0 };
	p->myViewport[1] = sl::ViewportHandle{ 1 };

	wchar_t exePath[MAX_PATH];
	GetModuleFileNameW(nullptr, exePath, MAX_PATH);
	std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();
	std::filesystem::path slDir = exeDir / L"SL";
	p->slDirStr = slDir.wstring();

	std::vector<sl::Feature> myFeatures;
	myFeatures.push_back(sl::kFeatureDLSS);

	sl::Preferences pref{};
	const wchar_t* pluginPaths[] = { p->slDirStr.c_str() };
	pref.pathsToPlugins = pluginPaths;
	pref.numPathsToPlugins = 1;
	pref.featuresToLoad = myFeatures.data();
	pref.numFeaturesToLoad = myFeatures.size();
	pref.flags = sl::PreferenceFlags::eDisableCLStateTracking | sl::PreferenceFlags::eUseFrameBasedResourceTagging; //  | sl::PreferenceFlags::eUseDXGIFactoryProxy | sl::PreferenceFlags::eUseManualHooking
	//pref.applicationId = 231313132;
	pref.engineVersion = "5.0";
	pref.projectId = "a0f57b54-1daf-4934-90ae-c4035c19df04";

#if defined(SLDEBUG)
	pref.logLevel = sl::LogLevel::eVerbose;
	pref.pathToLogsAndData = L"Z:\\TEMP";
#else
	pref.logLevel = sl::LogLevel::eOff;
	pref.pathToLogsAndData = nullptr;
#endif

	switch (renderAPI) {
	case StreamLineProcessorStatus_t::eD3D11:
		pref.renderAPI = sl::RenderAPI::eD3D11;
		break;
	case StreamLineProcessorStatus_t::eD3D12:
		pref.renderAPI = sl::RenderAPI::eD3D12;
		break;
	case StreamLineProcessorStatus_t::eVULKAN:
		pref.renderAPI = sl::RenderAPI::eVulkan;
		break;
	default: break;
	}
	auto slresult = slInit(pref);
	if (slresult != sl::Result::eOk) {
		streamline_TriggerFailureResult((UINT32)slresult + 1000, p);
		return;
	}
}

bool __fastcall streamline_InitializeD3D12() {
	// check no dxvk
	if (!g_StreamLineProcessor) return false;
	if (g_StreamLineProcessor->m_status != D3D::StreamLineProcessorStatus_t::eD3D12) return false;

	auto hmodD3D12 = LoadLibraryW(L"d3d12.dll");
	if (!hmodD3D12) return false;

	fnD3D12CreateDevice = (PFN_D3D12_CREATE_DEVICE)GetProcAddress(hmodD3D12, "D3D12CreateDevice");
	fnD3D12GetDebugInterface = (PFN_D3D12_GET_DEBUG_INTERFACE)GetProcAddress(hmodD3D12, "D3D12GetDebugInterface");

	return true;
}

bool __fastcall streamline_InitializeVK(ID3D11Device* device, void* pVKinfo){
	using namespace D3D;

	IDXGIVkInteropDevice* pVkInterop = nullptr;
	HRESULT hr = device->QueryInterface(IID_IDXGIVkInteropDevice, (void**)&pVkInterop);
	if (!pVkInterop) return false;

	VkDevice vkDevice; VkInstance vkInstance; VkPhysicalDevice vkPhysDevice;
	VkQueue vkQueue; uint32_t QueueFamilyIndex;

	pVkInterop->GetVulkanHandles(&vkInstance, &vkPhysDevice, &vkDevice);
	pVkInterop->GetSubmissionQueue(&vkQueue, &QueueFamilyIndex);
	pVkInterop->Release();

	if (!vkDevice) return false;

	auto p = (PslVulkanAPI)_aligned_malloc(sizeof(slVulkanAPI_t), 16U);
	if (!p) return false;

	ZeroMemory(p, sizeof(slVulkanAPI_t));

	p->Initialization();
	if (!p->vulkanModule) {
		_aligned_free(p);
		return false;
	}

	auto vulkanInfo = (sl::VulkanInfo*)pVKinfo;
	vulkanInfo->instance = vkInstance;
	vulkanInfo->physicalDevice = vkPhysDevice;
	vulkanInfo->device = vkDevice;
	vulkanInfo->computeQueueFamily = QueueFamilyIndex;

	p->m_vkInterop = pVkInterop;
	p->m_vkDevice = vkDevice;
	p->m_vkInstance = vkInstance;
	p->m_vkPhysDevice = vkPhysDevice;
	p->m_vkQueue = vkQueue;
	p->m_QueueFamilyIndex = QueueFamilyIndex;
	p->CreateFence();

	auto pSL = g_StreamLineProcessor;
	pSL->vk = p;

	return true;
}


void __fastcall streamline_CreateD3D12Device(){
	if (!streamline_InitializeD3D12()) return;

#if defined(hasDX12Debug)
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(fnD3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->D3D_Release();
	}
#endif	

	auto pSL = g_StreamLineProcessor;
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
	using namespace D3D;
	auto pSL = g_StreamLineProcessor;
	if (!pSL) return;

	streamline_InitializePostProcess(device, context);
	auto pPP = g_AddPostProcess;
	if (!pPP) return;

	sl::Result slresult;
	switch (pSL->m_status) {
	case StreamLineProcessorStatus_t::eD3D11:
		slresult = slSetD3DDevice(device);
		if (slresult != sl::Result::eOk) {
			streamline_TriggerFailureResult((UINT32)slresult + 1000, pSL);
			return;
		}

		break;
	case StreamLineProcessorStatus_t::eD3D12:
		slresult = slSetD3DDevice(pSL->m_d3d12Device);
		if (slresult != sl::Result::eOk) {
			streamline_TriggerFailureResult((UINT32)slresult + 1000, pSL);
			return;
		}

		break;
	case StreamLineProcessorStatus_t::eVULKAN: {
		sl::VulkanInfo vulkanInfo = {};
		if (!streamline_InitializeVK(device, &vulkanInfo)) return;

		slresult = slSetVulkanInfo(vulkanInfo);
		if (slresult != sl::Result::eOk) {
			streamline_TriggerFailureResult((UINT32)slresult + 1000, pSL);
			return;
		}

		break;
	}
	default: break;
	}
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

	auto curFrame = pSL->ClearFrame();
	pSL->SwapChainPresent(pGameDXGI);
}

void __fastcall streamline_Release() {
	using namespace D3D;
	auto pPP = g_AddPostProcess;
	auto pSL = g_StreamLineProcessor;

	if (pPP) {
		PslVulkanAPI deviceVK = 0;
		if (pSL) deviceVK = pSL->vk;

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

void __fastcall streamline_Reset() {
	auto pSL = g_StreamLineProcessor;
	if (pSL) {
		pSL->Reset();
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
	PslVulkanAPI deviceVK = 0;
	if (pSL) {
		if (pSL->m_status == StreamLineProcessorStatus_t::eD3D12)
			device12 = pSL->m_d3d12Device;
		else if (pSL->m_status == StreamLineProcessorStatus_t::eVULKAN)
			deviceVK = pSL->vk;
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
	if (pSL) {
		if (pSL->m_renderAPI > 1) {
			DLSS_Level = 2;
		} else {
			DLSS_Level = 1;
		}
	}
	pPP->Buffer_Release(deviceVK);
	pPP->Buffer_Create(pDXGI->pD3D11Device, res, device12, DLSS_Level);
	pPP->Buffer_VKCreate(deviceVK);

	if (pSL){
		sl::DLSSOptions dlssOptions = {};
		dlssOptions.mode = sl::DLSSMode::eDLAA;
		dlssOptions.outputWidth = res.Width;
		dlssOptions.outputHeight = res.Height;
		dlssOptions.sharpness = 1;
		dlssOptions.colorBuffersHDR = sl::Boolean::eFalse;
		// In sl.dlss, if there is no exposure texture, forced automatic exposure is applied.
		//dlssOptions.useAutoExposure = sl::Boolean::eFalse;
		//dlssOptions.dlaaPreset = sl::DLSSPreset::ePresetL;

		slDLSSSetOptions(pSL->myViewport[0], dlssOptions);
		if (playerCount == 2){
			slDLSSSetOptions(pSL->myViewport[1], dlssOptions);
		}

		/*
		if (playerCount == 2) {
			pSL->m_bIsSplitScreen = true;
		} else {
			pSL->m_bIsSplitScreen = false;
		}*/
	}
	// end
}

void* __fastcall streamline_ExecuteSR(Pg_D3D11DeviceInfo pD3D, int OutOffset, void* saveRCX) {
	if (OutOffset) return saveRCX;

	using namespace D3D;
	auto pPP = g_AddPostProcess;
	auto pSL = g_StreamLineProcessor;
	if (!pPP) return saveRCX;

	int inDlssMode = 0;
	if (pSL) {
		pSL->GetNewFrame();
		if (pSL->m_renderAPI && g_bPostProcess[1]) inDlssMode = 1;
	}

	auto playerCount = pPP->m_playerCount;

	auto pSys = XGS_GetXGSSystemPointer();
	PXGS_System_Player player[2];
	player[0] = &pSys->player[0];
	player[1] = &pSys->player[1];

	AddPostProcessRes_t threadGroupCount;
	threadGroupCount.Width = (pPP->m_resolution.Width + 15) / 16;
	threadGroupCount.Height = (pPP->m_resolution.Height + 15) / 16;

	ID3D11DeviceContext* context = pD3D->context;
	Pg_D3D_ResourceInfo pColorToMV[2] = {0, 0};

	for (int i = 0; i < playerCount; i++) {
		auto pColorRes = (Pg_D3D_ResourceInfo)player[i]->pDrawColorInfo;
		auto pDSVInfo = (Pg_D3D_ResourceInfo)player[i]->pDSVInfo;
		pColorToMV[i] = pColorRes;

		if (g_bPostProcess[0]) {
			pPP->Execute(pD3D, i, threadGroupCount, &pColorRes->pSRV, &pDSVInfo->pSRV);
		} else if (inDlssMode) {
			context->CopyResource(pPP->m_ColorRes[i].d11, pColorRes->pTexture);
			context->CopyResource(pPP->m_DepthRes[i].d11, pDSVInfo->pTexture);
		} else { return saveRCX; }

		if (!(inDlssMode & 1)) {
			context->CopyResource(pColorRes->pTexture, pPP->m_ColorRes[i].d11);
			if (i > 0) return saveRCX;
		}
	}


	if (inDlssMode & 1) {
		ComPtr<ID3D11DeviceContext4> d3d11Context4;
		context->QueryInterface(IID_PPV_ARGS(&d3d11Context4));

		bool isClearMV = (pSL->IsReset || playerCount != 1);
		pPP->CalculateMV(pD3D, player[0], threadGroupCount, isClearMV);

		sl::Extent slExtent = { 0, 0, pPP->m_resolution.Width, pPP->m_resolution.Height };
		// Used to prohibit flag: NVSDK_NGX_DLSS_Feature_Flags_MVLowRes
		sl::Extent mvExtent = { 0, 0, pPP->m_resolution.Width + 1, pPP->m_resolution.Height + 1 };
		StreamLineResource_t colorIn, colorOut, depth, mvec;

		pSL->GetJitter(playerCount);

		sl::Constants constants{};
		constants.jitterOffset = { pSL->v_jitter[0], pSL->v_jitter[1] };
		constants.reset = pSL->IsReset;
		constants.mvecScale = { 1, 1 };
		constants.cameraMotionIncluded = sl::Boolean::eTrue;
		constants.motionVectorsJittered = sl::Boolean::eFalse;

		pSL->IsReset = sl::Boolean::eFalse;

		if (pSL->m_status == StreamLineProcessorStatus_t::eD3D11) {
			for (int i = 0; i < playerCount; i++) {
				slSetConstants(constants, *pSL->currentFrame, pSL->myViewport[i]);

				auto pColorRes = (Pg_D3D_ResourceInfo)player[i]->pDrawColorInfo;
				auto pDSVInfo = (Pg_D3D_ResourceInfo)player[i]->pDSVInfo;

				colorIn.CreateFromD11(pPP->m_ColorRes[i].d11, sl::kBufferTypeScalingInputColor, sl::ResourceLifecycle::eValidUntilPresent, &slExtent);
				colorOut.CreateFromD11(pColorRes->pTexture, sl::kBufferTypeScalingOutputColor, sl::ResourceLifecycle::eValidUntilEvaluate, &slExtent);
				depth.CreateFromD11(pDSVInfo->pTexture, sl::kBufferTypeDepth, sl::ResourceLifecycle::eValidUntilEvaluate, &slExtent);
				mvec.CreateFromD11(pPP->m_MotionVector.d11, sl::kBufferTypeMotionVectors, sl::ResourceLifecycle::eValidUntilPresent, &mvExtent);

				//sl::ResourceTag tags[] = { colorIn.tag, colorOut.tag, depth.tag, mvec.tag };
				//slSetTagForFrame(*pSL->currentFrame, pSL->myViewport[0], tags, _countof(tags), context);

				//StreamLineResource_t albedo, normal;
				//albedo.CreateFromD11(player[i]->pRTV->pColorPass1RT0->pTexture, sl::kBufferTypeAlbedo, sl::ResourceLifecycle::eValidUntilEvaluate, &slExtent);
				//normal.CreateFromD11(player[i]->pRTV->pColorPass1RT2->pTexture, sl::kBufferTypeNormals, sl::ResourceLifecycle::eValidUntilEvaluate, &slExtent);

				const sl::BaseStructure* inputs[] = {
					&pSL->myViewport[i],
					&colorIn.tag,
					&colorOut.tag,
					&depth.tag,
					&mvec.tag,
					//&albedo.tag,
					//&normal.tag,
				};
				slEvaluateFeature(sl::kFeatureDLSS, *pSL->currentFrame, inputs, _countof(inputs), context);
			}
			// end
		} else if (pSL->m_status == StreamLineProcessorStatus_t::eD3D12) {

			d3d11Context4->Signal(pSL->m_fence11, ++pSL->m_shareFenceValue);
			d3d11Context4->Flush();

			pSL->m_commandAllocator->Reset();
			pSL->m_commandList->Reset(pSL->m_commandAllocator, nullptr);
			for (int i = 0; i < playerCount; i++) {
				slSetConstants(constants, *pSL->currentFrame, pSL->myViewport[i]);

				colorIn.CreateFromD12(pPP->m_ColorRes[i].d12, sl::kBufferTypeScalingInputColor, sl::ResourceLifecycle::eValidUntilPresent, &slExtent, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
				colorOut.CreateFromD12(pPP->m_MidColorRes[i].d12, sl::kBufferTypeScalingOutputColor, sl::ResourceLifecycle::eValidUntilPresent, &slExtent, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
				depth.CreateFromD12(pPP->m_DepthRes[i].d12, sl::kBufferTypeDepth, sl::ResourceLifecycle::eValidUntilPresent, &slExtent, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
				mvec.CreateFromD12(pPP->m_MotionVector.d12, sl::kBufferTypeMotionVectors, sl::ResourceLifecycle::eValidUntilPresent, &mvExtent, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

				const sl::BaseStructure* inputs[] = {
					&pSL->myViewport[i], &colorIn.tag, &colorOut.tag, &depth.tag, &mvec.tag,
				};

				D3D12_RESOURCE_BARRIER barriers[4] = {};

				barriers[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barriers[0].Transition.pResource = pPP->m_ColorRes[i].d12;
				barriers[0].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
				barriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;

				barriers[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barriers[1].Transition.pResource = pPP->m_MidColorRes[i].d12;
				barriers[1].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
				barriers[1].Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;

				barriers[2].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barriers[2].Transition.pResource = pPP->m_DepthRes[i].d12;
				barriers[2].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				barriers[2].Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
				barriers[2].Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;

				barriers[3].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barriers[3].Transition.pResource = pPP->m_MotionVector.d12;
				barriers[3].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				barriers[3].Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
				barriers[3].Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;

				pSL->m_commandList->ResourceBarrier(4, barriers);

				slEvaluateFeature(sl::kFeatureDLSS, *pSL->currentFrame, inputs, _countof(inputs), pSL->m_commandList);

				barriers[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
				barriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;

				barriers[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
				barriers[1].Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;

				barriers[2].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barriers[2].Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
				barriers[2].Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;

				barriers[3].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barriers[3].Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
				barriers[3].Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
				pSL->m_commandList->ResourceBarrier(4, barriers);
			}
			pSL->m_commandList->Close();
			ID3D12CommandList* lists[] = { pSL->m_commandList };

			pSL->m_commandQueue->Wait(pSL->m_fence12, pSL->m_shareFenceValue);
			pSL->m_commandQueue->ExecuteCommandLists(1, lists);

			//pSL->WaitFinish();

			pSL->m_commandQueue->Signal(pSL->m_fence12, ++pSL->m_shareFenceValue);
			d3d11Context4->Wait(pSL->m_fence11, pSL->m_shareFenceValue);

			pPP->CopyBuffer(pD3D, threadGroupCount, pColorToMV[0], pColorToMV[1]);
			// end
		}
		else if (pSL->m_status == StreamLineProcessorStatus_t::eVULKAN) {
			for (int i = 0; i < playerCount; i++) {
				slSetConstants(constants, *pSL->currentFrame, pSL->myViewport[i]);

				colorIn.CreateFromVK(pPP->m_ColorRes[i].vk, sl::kBufferTypeScalingInputColor, sl::ResourceLifecycle::eValidUntilPresent, &slExtent);
				colorOut.CreateFromVK(pPP->m_MidColorRes[i].vk, sl::kBufferTypeScalingOutputColor, sl::ResourceLifecycle::eValidUntilEvaluate, &slExtent);
				depth.CreateFromVK(pPP->m_DepthRes[i].vk, sl::kBufferTypeDepth, sl::ResourceLifecycle::eValidUntilEvaluate, &slExtent);
				mvec.CreateFromVK(pPP->m_MotionVector.vk, sl::kBufferTypeMotionVectors, sl::ResourceLifecycle::eValidUntilPresent, &mvExtent);

				const sl::BaseStructure* inputs[] = {
					&pSL->myViewport[i], &colorIn.tag, &colorOut.tag, &depth.tag, &mvec.tag,
				};

				pSL->vk->VK_Enter();
				slEvaluateFeature(sl::kFeatureDLSS, *pSL->currentFrame, inputs, _countof(inputs), pSL->vk->m_vkCMDlist);
				pSL->vk->VK_Leave(VK_NULL_HANDLE);
			}
			// end
		}

	}


	return saveRCX;
}
