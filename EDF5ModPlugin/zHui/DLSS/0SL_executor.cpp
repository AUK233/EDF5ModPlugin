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
#include <d3d12.h>

#include "0SL_executor.h"

extern PFN_D3D12_CREATE_DEVICE fnD3D12CreateDevice;
extern "C" {
	extern int Config_PostProcess;
	extern int Config_PostProcessTexIndex;
	extern int Config_DLAA;
	extern int Config_DLSSFG;
}

namespace D3D {
	void __fastcall StreamLineProcessor_t::CreateD3D12Device() {
		// create dxgi factory
		auto hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));

		if (FAILED(hr)) return;

		// check hardware adapter
		ComPtr<IDXGIAdapter1> hardwareAdapter;
		for (UINT i = 0; i < 3; ++i) {
			ComPtr<IDXGIAdapter1> adapter;
			if (m_dxgiFactory->EnumAdapters1(i, &adapter) == DXGI_ERROR_NOT_FOUND)
				break;

			if (SUCCEEDED(fnD3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_1, _uuidof(ID3D12Device), nullptr))) {
				hardwareAdapter = adapter;
				break;
			}
		}
		if (!hardwareAdapter) return;

		// create d3d12 device
		hr = fnD3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&m_d3d12Device));
		if (FAILED(hr)) return;

		// create command queue
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		hr = m_d3d12Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
		if (FAILED(hr)) return;

		// create command allocator and command list
		m_d3d12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
		m_d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, nullptr, IID_PPV_ARGS(&m_commandList));
		m_commandList->Close();

		// create fence
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		hr = m_d3d12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fenceNative));

		// done!
#if defined(EnableDXDebugInformation)
		std::wstring hrText = std::format(L"D3D12Device result: {:X}.\n", hr);
		OutputDebugStringW(hrText.c_str());
#endif	

		m_status = D3D::StreamLineProcessorStatus_t::eD3D12;
	}

	HRESULT __fastcall StreamLineProcessor_t::SwapChainGetBuffer(IDXGISwapChain* pSwapChain, PGameDXGIRender pGameDXGI, ID3D11Texture2D** pOut){
		// get ID3D11DeviceContext4
		D3DResourceCommonRelease(m_context4);
		pGameDXGI->pD3D11DeviceContext->QueryInterface(IID_PPV_ARGS(&m_context4));

		if (!Config_DLSSFG && m_status != StreamLineProcessorStatus_t::eD3D12) {
			auto hr = pSwapChain->GetBuffer(0, IID_PPV_ARGS(pOut));
			return hr;
		}

		//g_gameSceneResource.D3D_Release(); use Reload() instead
		// if a vk resource exists, it must be released first.
		g_gameSceneResource.VK_ReleaseVKResource(vk);

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
		outDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		outDesc.CPUAccessFlags = 0;
		//outDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED | D3D11_RESOURCE_MISC_SHARED_NTHANDLE;
		g_gameSceneResource.D3D_Reload(device, &outDesc, m_d3d12Device);

		*pOut = g_gameSceneResource.d11;
		g_gameSceneResource.d11->AddRef();

		if (m_status == StreamLineProcessorStatus_t::eD3D12) {
			// create fence for d3d11
			ComPtr<ID3D11Device5> d3d11Device5;
			auto hr = device->QueryInterface(IID_PPV_ARGS(&d3d11Device5));

			hr = d3d11Device5->CreateFence(0, D3D11_FENCE_FLAG_SHARED, IID_PPV_ARGS(&m_fence11));
			HANDLE sharedHandle = nullptr;
			m_fence11->CreateSharedHandle(nullptr, GENERIC_ALL, nullptr, &sharedHandle);
			hr = m_d3d12Device->OpenSharedHandle(sharedHandle, IID_PPV_ARGS(&m_fence12));
			CloseHandle(sharedHandle);


#if defined(EnableDXDebugInformation)
			std::wstring hrText = std::format(L"Shared Fence result: {:X}.\n", hr);
			OutputDebugStringW(hrText.c_str());
#endif	

			/*
			D3D12_RESOURCE_DESC texDesc = {};
			texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			texDesc.Width = dufferDesc->Width;
			texDesc.Height = dufferDesc->Height;
			texDesc.DepthOrArraySize = 1;
			texDesc.MipLevels = 1;
			texDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			texDesc.SampleDesc.Count = 1;
			texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

			D3D12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);
			m_d3d12Device->CreateCommittedResource(
				&heapProps,
				D3D12_HEAP_FLAG_NONE,
				&texDesc,
				D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
				nullptr,
				IID_PPV_ARGS(&privateOutput)
			);
			*/
		}
		else {
			g_D3D11BackBuffer = 0;
			pSwapChain->GetBuffer(0, IID_PPV_ARGS(&g_D3D11BackBuffer));
			g_D3D11BackBuffer->Release();

			g_gameSceneResource.VK_CreateFromD11(vk);
		}


		return S_OK;
	}

	bool __fastcall StreamLineProcessor_t::SwapChainPresent(PGameDXGIRender pGameDXGI, UINT SyncInterval) {
		if (!g_gameSceneResource.d11) return false;
		bool presentDone = false;

		if (m_status == StreamLineProcessorStatus_t::eD3D12){
			if(m_fgFeature){
				presentDone = D3D12EvaluateFG(pGameDXGI, SyncInterval);
			}

			if(!presentDone){
				D3D12SetPresentBuffer();
				return false;
			} else {
				D3D12SetPresentBuffer();
				m_dxgiSwapChain3->Present(SyncInterval, 0);
				return true;
			}
			// end
		}
		else {
			auto context = pGameDXGI->pD3D11DeviceContext;

			if (vk && m_fgFeature){
				presentDone = VKEvaluateFG(pGameDXGI, SyncInterval);
			}

			if (!presentDone) {
				context->CopyResource(g_D3D11BackBuffer, g_gameSceneResource.d11);
				return false;
			}
		}
		
		return false;
	}

	void __fastcall StreamLineProcessor_t::D3D12CommandBegin() {
		m_context4->Signal(m_fence11, ++m_shareFenceValue);
		m_context4->Flush();

		m_commandAllocator->Reset();
		m_commandList->Reset(m_commandAllocator, nullptr);
	}

	void __fastcall StreamLineProcessor_t::D3D12CommandEnd() {
		auto hr = m_commandList->Close();
#if defined(EnableDXDebugInformation)
		if (FAILED(hr)) {
			std::wstring hrText = std::format(L"[Error] Command List Error Code: {:X}.\n", hr);
			OutputDebugStringW(hrText.c_str());
		}
#endif	
		ID3D12CommandList* lists[] = { m_commandList };

		m_commandQueue->Wait(m_fence12, m_shareFenceValue);
		m_commandQueue->ExecuteCommandLists(1, lists);
	}

	void __fastcall StreamLineProcessor_t::D3D12WaitFinish() {
		m_commandQueue->Signal(m_fenceNative, ++m_nativeFenceValue);

		if (m_fenceNative->GetCompletedValue() < m_nativeFenceValue) {
			m_fenceNative->SetEventOnCompletion(m_nativeFenceValue, m_fenceEvent);
			WaitForSingleObject(m_fenceEvent, INFINITE);
		}
	}

	void __fastcall StreamLineProcessor_t::D3D12SetPresentBuffer() {
		UINT currentIndex = m_dxgiSwapChain3->GetCurrentBackBufferIndex();
		auto backBuffer = v_d3d12BackBuffers[currentIndex];

		D3D12CommandBegin();

		/*
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = backBuffer;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
		m_commandList->ResourceBarrier(1, &barrier);*/

		m_commandList->CopyResource(backBuffer, g_gameSceneResource.d12);

		/*
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		m_commandList->ResourceBarrier(1, &barrier);*/

		D3D12CommandEnd();
		D3D12WaitFinish();
	}

	bool __fastcall StreamLineProcessor_t::GetParameters() {
		switch (m_status) {
		case StreamLineProcessorStatus_t::eD3D11:
			NVSDK_NGX_D3D11_GetCapabilityParameters(&m_srParameters);
			break;
		case StreamLineProcessorStatus_t::eD3D12:
			NVSDK_NGX_D3D12_GetCapabilityParameters(&m_srParameters);
			if (Config_DLSSFG)
				NVSDK_NGX_D3D12_GetCapabilityParameters(&m_fgParameters);
			break;
		case StreamLineProcessorStatus_t::eVULKAN:
			NVSDK_NGX_VULKAN_GetCapabilityParameters(&m_srParameters);
			if (Config_DLSSFG)
				NVSDK_NGX_VULKAN_GetCapabilityParameters(&m_fgParameters);
			break;
		default: return false;
		}

		if (m_srParameters) {
			if (Config_DLAA == 3) {
				NVSDK_NGX_Parameter_SetUI(m_srParameters, NVSDK_NGX_Parameter_DLSS_Hint_Render_Preset_DLAA, NVSDK_NGX_DLSS_Hint_Render_Preset_L);
			} else if (Config_DLAA == 4) {
				NVSDK_NGX_Parameter_SetUI(m_srParameters, NVSDK_NGX_Parameter_DLSS_Hint_Render_Preset_DLAA, NVSDK_NGX_DLSS_Hint_Render_Preset_M);
			} else {
				NVSDK_NGX_Parameter_SetUI(m_srParameters, NVSDK_NGX_Parameter_DLSS_Hint_Render_Preset_DLAA, NVSDK_NGX_DLSS_Hint_Render_Preset_Default);
			}

			return true;
		}
		else {
			return false;
		}
	}

	void __fastcall StreamLineProcessor_t::SetFeature(ID3D11DeviceContext* context, UINT Width, UINT Height, int playerCount) {
		if (playerCount == 2) {
			m_bIsSplitScreen = true;
		} else {
			m_bIsSplitScreen = false;
		}

		Reset();
		ReleaseFeature();
		if (!GetParameters()) return;

		NVSDK_NGX_DLSS_Create_Params DlssCreateParams;
		memset(&DlssCreateParams, 0, sizeof(DlssCreateParams));

		DlssCreateParams.Feature.InWidth = Width;
		DlssCreateParams.Feature.InHeight = Height;
		DlssCreateParams.Feature.InTargetWidth = Width;
		DlssCreateParams.Feature.InTargetHeight = Height;
		DlssCreateParams.Feature.InPerfQualityValue = NVSDK_NGX_PerfQuality_Value_DLAA;
		DlssCreateParams.InFeatureCreateFlags = NVSDK_NGX_DLSS_Feature_Flags_None | NVSDK_NGX_DLSS_Feature_Flags_IsHDR;
		DlssCreateParams.InEnableOutputSubrects = false;

		if (Config_DLAA == 2) {
			DlssCreateParams.InFeatureCreateFlags |= NVSDK_NGX_DLSS_Feature_Flags_AutoExposure;
		}

		int multiFrameCount = Config_DLSSFG;

		NVSDK_NGX_DLSSG_Create_Params DlssGCreateParams;
		memset(&DlssGCreateParams, 0, sizeof(DlssGCreateParams));
		DlssGCreateParams.Width = Width;
		DlssGCreateParams.Height = Height;
		DlssGCreateParams.RenderWidth = Width;
		DlssGCreateParams.RenderHeight = Height;
		DlssGCreateParams.DynamicResolutionScaling = false;

		switch (m_status) {
		case StreamLineProcessorStatus_t::eD3D11:
			NGX_D3D11_CREATE_DLSS_EXT(context, &m_srFeature, m_srParameters, &DlssCreateParams);
			break;
		case StreamLineProcessorStatus_t::eD3D12:
			D3D12CommandBegin();

			NGX_D3D12_CREATE_DLSS_EXT(m_commandList, 0, 0, &m_srFeature, m_srParameters, &DlssCreateParams);

			if (multiFrameCount && playerCount == 1) {
				if (multiFrameCount > DLSSMaxMultiFrameCount) {
					m_multiFrameCount = DLSSMaxMultiFrameCount;
				} else {
					m_multiFrameCount = multiFrameCount;
				}

				DlssGCreateParams.NativeBackbufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
				NGX_D3D12_CREATE_DLSSG(m_commandList, 0, 0, &m_fgFeature, m_fgParameters, &DlssGCreateParams);

				m_bNeedFG = true;
			}

			D3D12CommandEnd();
			break;
		case StreamLineProcessorStatus_t::eVULKAN:
			vk->VK_Enter();

			NGX_VULKAN_CREATE_DLSS_EXT(vk->m_vkCMDlist, 0, 0, &m_srFeature, m_srParameters, &DlssCreateParams);

			if (multiFrameCount && playerCount == 1) {
				m_multiFrameCount = 1;

				DlssGCreateParams.NativeBackbufferFormat = VK_FORMAT_R8G8B8A8_UNORM;
				NGX_VK_CREATE_DLSSG(vk->m_vkCMDlist, 0, 0, &m_fgFeature, m_fgParameters, &DlssGCreateParams);

				m_bNeedFG = true;
			}

			vk->VK_Leave(VK_NULL_HANDLE);
			break;
		default: break;
		}
	}

	void __fastcall StreamLineProcessor_t::Release() {
		ReleaseFeature();

		switch (m_status) {
		case StreamLineProcessorStatus_t::eD3D11:
			NVSDK_NGX_D3D11_Shutdown1(nullptr);
			break;
		case StreamLineProcessorStatus_t::eD3D12:
			NVSDK_NGX_D3D12_Shutdown1(nullptr);
			break;
		case StreamLineProcessorStatus_t::eVULKAN:
			NVSDK_NGX_VULKAN_Shutdown1(nullptr);
			break;
		default: break;
		}

		if (m_status == StreamLineProcessorStatus_t::eD3D12) {
			BOOL fullscreen;
			IDXGIOutput* pTarget;
			m_dxgiSwapChain3->GetFullscreenState(&fullscreen, &pTarget);
			if (fullscreen == TRUE) {
				pTarget->Release();
				m_dxgiSwapChain3->SetFullscreenState(FALSE, nullptr);
			}

			/*
			D3D12WaitFinish();
			CloseHandle(m_fenceEvent);
			m_d3d12Device->Release();*/
		}
		// end
	}

	void __fastcall StreamLineProcessor_t::ReleaseFeature() {
		if (m_srFeature) {
			switch (m_status) {
			case StreamLineProcessorStatus_t::eD3D11:
				NVSDK_NGX_D3D11_ReleaseFeature(m_srFeature);
				NVSDK_NGX_D3D11_DestroyParameters(m_srParameters);
				break;
			case StreamLineProcessorStatus_t::eD3D12:
				NVSDK_NGX_D3D12_ReleaseFeature(m_srFeature);
				NVSDK_NGX_D3D12_DestroyParameters(m_srParameters);
				break;
			case StreamLineProcessorStatus_t::eVULKAN:
				NVSDK_NGX_VULKAN_ReleaseFeature(m_srFeature);
				NVSDK_NGX_VULKAN_DestroyParameters(m_srParameters);
				break;
			default: break;
			}

			m_srFeature = nullptr;
			m_srParameters = nullptr;
		}

		if (m_fgFeature) {
			switch (m_status) {
			case StreamLineProcessorStatus_t::eD3D12:
				NVSDK_NGX_D3D12_ReleaseFeature(m_fgFeature);
				NVSDK_NGX_D3D12_DestroyParameters(m_fgParameters);
				break;
			case StreamLineProcessorStatus_t::eVULKAN:
				NVSDK_NGX_VULKAN_ReleaseFeature(m_fgFeature);
				NVSDK_NGX_VULKAN_DestroyParameters(m_fgParameters);
				break;
			default: break;
			}

			m_fgFeature = nullptr;
			m_fgParameters = nullptr;
		}
		// end
	}

	void __fastcall StreamLineProcessor_t::Reset() {
		m_srReset = 1;
		m_fgReset = 1;
		JitterIndex = 1;
		*(UINT64*)v_jitter = 0;
	}

	float __fastcall StreamLineProcessor_t::Halton(int index, int base) {
		float result = 0.0f;
		float f = 1.0f / base;
		int i = index;
		while (i > 0) {
			result += f * (i % base);
			i = i / base;
			f = f / base;
		}
		result -= 0.5f;
		return result;
	}

	void __fastcall StreamLineProcessor_t::GetJitter(int playerCount) {
		if (playerCount > 1) {
			*(UINT64*)v_jitter = 0;
			m_srReset = 1;
			return;
		}

		v_jitter[0] = Halton(JitterIndex, 2);
		v_jitter[1] = Halton(JitterIndex, 3);
		JitterIndex++;
		m_srReset = 0;
	}

	void __fastcall StreamLineProcessor_t::EvaluateSR(ID3D11DeviceContext* context, PAddPostProcess pPP, PGameRenderer_RTV pRTV, NVSDK_NGX_D3D11_DLSS_Eval_Params& D3D11DlssEvalParams, int playerIndex) {
		D3D11DlssEvalParams.Feature.pInColor = pPP->m_ColorRes[playerIndex].d11;
		D3D11DlssEvalParams.Feature.InSharpness = 1;

		D3D11DlssEvalParams.pInDepth = pPP->m_DepthRes[playerIndex].d11;
		D3D11DlssEvalParams.pInMotionVectors = pPP->m_MotionVectorRes.d11;

		D3D11DlssEvalParams.InJitterOffsetX = v_jitter[0];
		D3D11DlssEvalParams.InJitterOffsetY = v_jitter[1];
		D3D11DlssEvalParams.InRenderSubrectDimensions.Width = pPP->m_resolution.Width;
		D3D11DlssEvalParams.InRenderSubrectDimensions.Height = pPP->m_resolution.Height;

		D3D11DlssEvalParams.InMVScaleX = 1.0;
		D3D11DlssEvalParams.InMVScaleY = 1.0;
		
		D3D11DlssEvalParams.GBufferSurface.pInAttrib[NVSDK_NGX_GBUFFER_ALBEDO] = pRTV->pColorPass1RT0->pTexture;
		D3D11DlssEvalParams.GBufferSurface.pInAttrib[NVSDK_NGX_GBUFFER_SPECULAR] = pRTV->pColorPass1RT1->pTexture;
		D3D11DlssEvalParams.GBufferSurface.pInAttrib[NVSDK_NGX_GBUFFER_NORMALS] = pRTV->pColorPass1RT2->pTexture;

		NVSDK_NGX_Parameter_SetF(m_srParameters, NVSDK_NGX_Parameter_Denoise, 1.0);
		NGX_D3D11_EVALUATE_DLSS_EXT(context, m_srFeature, m_srParameters, &D3D11DlssEvalParams);
	}

	void __fastcall StreamLineProcessor_t::EvaluateSR(PAddPostProcess pPP, NVSDK_NGX_D3D12_DLSS_Eval_Params& D3D12DlssEvalParams, int playerIndex) {
		D3D12DlssEvalParams.Feature.pInColor = pPP->m_ColorRes[playerIndex].d12;
		D3D12DlssEvalParams.Feature.pInOutput = pPP->m_MidColorRes[playerIndex].d12;
		D3D12DlssEvalParams.Feature.InSharpness = 1;

		D3D12DlssEvalParams.pInDepth = pPP->m_DepthRes[playerIndex].d12;
		D3D12DlssEvalParams.pInMotionVectors = pPP->m_MotionVectorRes.d12;

		D3D12DlssEvalParams.InJitterOffsetX = v_jitter[0];
		D3D12DlssEvalParams.InJitterOffsetY = v_jitter[1];
		D3D12DlssEvalParams.InRenderSubrectDimensions.Width = pPP->m_resolution.Width;
		D3D12DlssEvalParams.InRenderSubrectDimensions.Height = pPP->m_resolution.Height;

		D3D12DlssEvalParams.InMVScaleX = 1.0;
		D3D12DlssEvalParams.InMVScaleY = 1.0;

		NVSDK_NGX_Parameter_SetF(m_srParameters, NVSDK_NGX_Parameter_Denoise, 1.0);
		NGX_D3D12_EVALUATE_DLSS_EXT(m_commandList, m_srFeature, m_srParameters, &D3D12DlssEvalParams);
	}

	void __fastcall StreamLineProcessor_t::EvaluateSR(PAddPostProcess pPP, NVSDK_NGX_VK_DLSS_Eval_Params& D3DvkDlssEvalParams, int playerIndex) {

		D3DvkDlssEvalParams.Feature.pInColor = pPP->m_ColorRes[playerIndex].vk;
		D3DvkDlssEvalParams.Feature.pInOutput = pPP->m_MidColorRes[playerIndex].vk;
		D3DvkDlssEvalParams.Feature.InSharpness = 1;

		D3DvkDlssEvalParams.pInDepth = pPP->m_DepthRes[playerIndex].vk;
		D3DvkDlssEvalParams.pInMotionVectors = pPP->m_MotionVectorRes.vk;

		D3DvkDlssEvalParams.InJitterOffsetX = v_jitter[0];
		D3DvkDlssEvalParams.InJitterOffsetY = v_jitter[1];
		D3DvkDlssEvalParams.InRenderSubrectDimensions.Width = pPP->m_resolution.Width;
		D3DvkDlssEvalParams.InRenderSubrectDimensions.Height = pPP->m_resolution.Height;

		D3DvkDlssEvalParams.InMVScaleX = 1.0;
		D3DvkDlssEvalParams.InMVScaleY = 1.0;

		NVSDK_NGX_Parameter_SetF(m_srParameters, NVSDK_NGX_Parameter_Denoise, 1.0);

		NGX_VULKAN_EVALUATE_DLSS_EXT(vk->m_vkCMDlist, m_srFeature, m_srParameters, &D3DvkDlssEvalParams);
	}

	PAddPostProcess __fastcall StreamLineProcessor_t::CheckNeedFG() {
		if (!m_bNeedFG) return 0;
		if (m_bIsSplitScreen) return 0;
		if (!m_multiFrameCount) return 0;

		auto pSys = XGS_GetXGSSystemPointer();
		if (!pSys) return 0;
		auto pCamera = pSys->player[0].pCamera;
		if (!pCamera) return 0;
		auto pPlayer = pCamera->pGameObject;
		if (!pPlayer) return 0;

		return GetAddPostProcessPointer();
	}

	bool __fastcall StreamLineProcessor_t::D3D12EvaluateFG(PGameDXGIRender pGameDXGI, UINT SyncInterval) {
		auto pPP = CheckNeedFG();
		if (!pPP) return false;


		NVSDK_NGX_D3D12_DLSSG_Eval_Params FGParams;
		ZeroMemory(&FGParams, sizeof(FGParams));
		FGParams.pBackbuffer = g_gameSceneResource.d12;
		FGParams.pDepth = pPP->m_DepthRes[0].d12;
		FGParams.pMVecs = pPP->m_fgMVRes.d12;
		FGParams.pHudless = pPP->m_FGhudLessRes.d12;
		FGParams.pOutputInterpFrame = pPP->m_FGOutputInterpRes.d12;

		NVSDK_NGX_DLSSG_Opt_Eval_Params OptEvalParams = {};
		OptEvalParams.reset = m_fgReset;
		OptEvalParams.depthInverted = false;
		OptEvalParams.mvecScale[0] = 1;
		OptEvalParams.mvecScale[1] = 1;
		OptEvalParams.multiFrameCount = m_multiFrameCount;
		OptEvalParams.multiFrameIndex = 1;

		EvaluateFG(FGParams, OptEvalParams, pPP);
		m_fgReset = 0;
		if (m_multiFrameCount > 1){
			m_dxgiSwapChain3->Present(SyncInterval, 0);
			for (int i = 2; i <= m_multiFrameCount; i++) {
				OptEvalParams.multiFrameIndex = i;

				EvaluateFG(FGParams, OptEvalParams, pPP);
				m_dxgiSwapChain3->Present(SyncInterval, 0);
			}
			return true;
		}
		else{
			m_dxgiSwapChain3->Present(SyncInterval, 0);

			//auto pApp = DXGI_GetApplication1253708();
			//int sleep_time = pApp->FrameInterval * 500;// yeah 0.5x1000
			//Sleep(sleep_time);
			return false;
		}
	}

	void StreamLineProcessor_t::EvaluateFG(NVSDK_NGX_D3D12_DLSSG_Eval_Params& FGParams, NVSDK_NGX_DLSSG_Opt_Eval_Params& OptEvalParams, PAddPostProcess pPP) {
		UINT currentIndex = m_dxgiSwapChain3->GetCurrentBackBufferIndex();
		auto backBuffer = v_d3d12BackBuffers[currentIndex];

		D3D12CommandBegin();

		auto ResultDLSS = NGX_D3D12_EVALUATE_DLSSG(m_commandList, m_fgFeature, m_fgParameters, &FGParams, &OptEvalParams);
#if defined(EnableDXDebugInformation)
		if (ResultDLSS != NVSDK_NGX_Result_Success) {
			std::wstring hrText = std::format(L"NGX_D3D12_EVALUATE_DLSSG result: 0x{:X}.\n", (UINT)ResultDLSS);
			OutputDebugStringW(hrText.c_str());
		}
#endif	
		m_commandList->CopyResource(backBuffer, pPP->m_FGOutputInterpRes.d12);

		D3D12CommandEnd();
		D3D12WaitFinish();
	}

	bool __fastcall StreamLineProcessor_t::VKEvaluateFG(PGameDXGIRender pGameDXGI, UINT SyncInterval) {
		if(!g_gameSceneResource.vk) return false;

		auto pPP = CheckNeedFG();
		if (!pPP) return false;

		auto context = pGameDXGI->pD3D11DeviceContext;

		NVSDK_NGX_VK_DLSSG_Eval_Params FGParams;
		ZeroMemory(&FGParams, sizeof(FGParams));
		FGParams.pBackbuffer = g_gameSceneResource.vk;
		FGParams.pDepth = pPP->m_DepthRes[0].vk;
		FGParams.pMVecs = pPP->m_fgMVRes.vk;
		FGParams.pHudless = pPP->m_FGhudLessRes.vk;
		FGParams.pOutputInterpFrame = pPP->m_FGOutputInterpRes.vk;
		//FGParams.pOutputRealFrame = &pNGX_dlss->vkOutputReal;

		NVSDK_NGX_DLSSG_Opt_Eval_Params OptEvalParams = {};
		OptEvalParams.reset = m_fgReset;
		OptEvalParams.depthInverted = false;
		OptEvalParams.mvecScale[0] = 1;
		OptEvalParams.mvecScale[1] = 1;
		//OptEvalParams.clipToPrevClip = clipToPrevClipMatrix;
		//OptEvalParams.prevClipToClip = prevClipToClipMatrix;
		OptEvalParams.multiFrameCount = 1;
		OptEvalParams.multiFrameIndex = 1;

		vk->VK_Enter();
		auto ResultDLSS = NGX_VK_EVALUATE_DLSSG(vk->m_vkCMDlist, m_fgFeature, m_fgParameters, &FGParams, &OptEvalParams);
		vk->VK_Leave(VK_NULL_HANDLE);
#if defined(EnableDXDebugInformation)
		if (ResultDLSS != NVSDK_NGX_Result_Success) {
			std::wstring hrText = std::format(L"NGX_VK_EVALUATE_DLSSG result: 0x{:X}.\n", (UINT)ResultDLSS);
			OutputDebugStringW(hrText.c_str());
		}
#endif	
		m_fgReset = 0;

		context->CopyResource(g_D3D11BackBuffer, pPP->m_FGOutputInterpRes.d11);
		pGameDXGI->pDXGISwapChain->Present(SyncInterval, 0);

		Sleep(8);
		return false;
	}
}
