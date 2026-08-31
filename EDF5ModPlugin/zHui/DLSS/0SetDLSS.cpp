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

#define HASVK
#if defined(HASVK)
#include "0SetDLSS_VK.hpp"
#endif

#define DEBUGMODE
__declspec(align(16)) typedef struct NGX_DLSS_t {
	ID3D11Texture2D* ColorBuffer;
	ID3D11Texture2D* OutColor;
	ID3D11Texture2D* DepthBuffer;
	int resolution[2];
	NVSDK_NGX_Parameter* m_ngxParameters;
	NVSDK_NGX_Handle* m_dlssFeature;
#if defined(HASVK)
	VkDevice vkDevice;
#endif
	int m_bDlssAvailable;
	int JitterIndex;
	int IsReset;
	int IsSplitScreen;
	int IsReady;
#if defined(HASVK)
	xess_context_handle_t m_xessContext; // intel xess!
	IDXGIVkInteropDevice* pVkInterop;
	VkInstance vkInstance;
	VkPhysicalDevice vkPhysDevice;
	VkQueue vkQueue;
	VkCommandPool vkCMDPool;
	VkCommandBuffer vkCMDlist;
	VkFence dlssFence;
	uint32_t QueueFamilyIndex;
	// set in DLSS_CreateFeature
	NVSDK_NGX_Resource_VK vkInColor[2];
	NVSDK_NGX_Resource_VK vkMV;
#endif
} *PNGX_DLSS;

PNGX_DLSS pNGX_dlss = nullptr;
D3D::PD3DPostProcess pD3DPostProcess = nullptr;

extern "C" {
	extern int Config_PostProcess;
	extern int Config_DLAA;
}

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

	if (pNGX_dlss && pNGX_dlss->vkDevice) {
		NVSDK_NGX_Resource_VK_Destroy(pNGX_dlss->vkDevice, &pNGX_dlss->vkInColor[0]);
		NVSDK_NGX_Resource_VK_Destroy(pNGX_dlss->vkDevice, &pNGX_dlss->vkInColor[1]);
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

		if (pNGX_dlss->vkDevice) {
			NVSDK_NGX_Resource_VK_Get(pNGX_dlss->vkDevice, pD3DPostProcess->OutColor[0], &pNGX_dlss->vkInColor[0]);
			NVSDK_NGX_Resource_VK_Get(pNGX_dlss->vkDevice, pD3DPostProcess->BlackMV, &pNGX_dlss->vkMV);

			if (playerCount == 2) {
				NVSDK_NGX_Resource_VK_Get(pNGX_dlss->vkDevice, pD3DPostProcess->OutColor[1], &pNGX_dlss->vkInColor[1]);
			}
		}
		// end
	}
	// end
}

void* __fastcall DLSS_Draw(Pg_D3D11DeviceInfo pD3D, int OutOffset, void* saveRCX){
	if (!pD3DPostProcess) return saveRCX;

	int playerIndex = 0;
	if (OutOffset) playerIndex = 1;

	auto sys = XGS_GetXGSSystemPointer();
	auto pColorRes = (Pg_D3D_ResourceInfo)sys->player[playerIndex].pDrawColorInfo;
	//auto pColorRes = Get_g_xgl_draw_utility_color_textrure();
	if (!pColorRes) return saveRCX;
	if (!pColorRes->pUAV) return saveRCX;

	auto pDSVInfo = (Pg_D3D_ResourceInfo)sys->player[playerIndex].pDSVInfo;
	if (!pDSVInfo) return saveRCX;
	if (!pDSVInfo->pSRV) return saveRCX;

	D3D11_TEXTURE2D_DESC inDesc;
	auto pTexture = pColorRes->pTexture;
	pTexture->GetDesc(&inDesc);
	if (inDesc.Format != DXGI_FORMAT_R16G16B16A16_FLOAT) return saveRCX;

	ID3D11DeviceContext* pContext = pD3D->context;
	if (Config_PostProcess) {
		pD3D->pCurrentCSShaderResourceView[0] = &pColorRes->pSRV;
		pContext->CSSetShaderResources(0, 1, &pColorRes->pSRV);
		pD3D->pCurrentCSShaderResourceView[1] = &pDSVInfo->pSRV;
		pContext->CSSetShaderResources(1, 1, &pDSVInfo->pSRV);

		pD3D->pCurrentCSShaderResourceView[9] = &pD3DPostProcess->LookupTable_SRV;
		pContext->CSSetShaderResources(9, 1, &pD3DPostProcess->LookupTable_SRV);
		pD3D->pCurrentCSSamplerState[11] = &pD3DPostProcess->LUTSamplerLinear;
		pContext->CSSetSamplers(11, 1, &pD3DPostProcess->LUTSamplerLinear);

		pD3D->pCurrentCSShader = pD3DPostProcess->PostProcessCS;
		pContext->CSSetShader(pD3DPostProcess->PostProcessCS, nullptr, 0);

		pD3D->pCurrentCSUnorderedAccessViews[0] = &pD3DPostProcess->OutUAV[playerIndex];
		pContext->CSSetUnorderedAccessViews(0, 1, &pD3DPostProcess->OutUAV[playerIndex], nullptr);

		//pContext->CSSetUnorderedAccessViews(1, 1, &pD3DPostProcess->LinearDepthUAV[playerIndex], nullptr);

		pContext->Dispatch((pColorRes->width + 15) / 16, (pColorRes->height + 15) / 16, 1);
	}
	else if (pNGX_dlss) {
		pContext->CopyResource(pD3DPostProcess->OutColor[playerIndex], pColorRes->pTexture);
	}
	else { return saveRCX; }

	/*
	auto p1259680 = DXGI_GetGameRenderer1259680();
	auto old_cb0 = p1259680->CB_xgl_system;
		pContext->CSSetConstantBuffers(0, 1, old_cb0);
		pContext->CSSetConstantBuffers(2, 1, &pD3DPostProcess->PreviousCB_xgl_system);

		auto xyzID = &sys->player[playerIndex].pRTV->pColorPass1RT5->pSRV;
		pD3D->pCurrentCSShaderResourceView[0] = xyzID;
		pContext->CSSetShaderResources(0, 1, xyzID);

		pD3D->pCurrentCSShader = pD3DPostProcess->MotionVectorCS;
		pContext->CSSetShader(pD3DPostProcess->MotionVectorCS, nullptr, 0);
		pContext->CSSetUnorderedAccessViews(0, 1, &pD3DPostProcess->MotionVectorUAV, nullptr);

		pContext->Dispatch((pColorRes->width + 15) / 16, (pColorRes->height + 15) / 16, 1);
		pD3D->pCurrentCSUnorderedAccessViews[0] = 0;
		ID3D11UnorderedAccessView* nullUAV = nullptr;
		pContext->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
	pContext->CopyResource(pD3DPostProcess->PreviousCB_xgl_system, *old_cb0);*/

	// evaluate dlss
	if (pNGX_dlss) {
		if (!pNGX_dlss->IsSplitScreen) {
			auto p1259680 = DXGI_GetGameRenderer1259680();
			auto old_cb0 = p1259680->CB_xgl_system;
			if(!pNGX_dlss->IsReset){
				pContext->CSSetConstantBuffers(0, 1, old_cb0);
				pContext->CSSetConstantBuffers(2, 1, &pD3DPostProcess->PreviousCB_xgl_system);

				auto xyzID = &sys->player[playerIndex].pRTV->pColorPass1RT5->pSRV;
				pD3D->pCurrentCSShaderResourceView[0] = xyzID;
				pContext->CSSetShaderResources(0, 1, xyzID);

				pD3D->pCurrentCSShader = pD3DPostProcess->MotionVectorCS;
				pContext->CSSetShader(pD3DPostProcess->MotionVectorCS, nullptr, 0);
				pContext->CSSetUnorderedAccessViews(0, 1, &pD3DPostProcess->MotionVectorUAV, nullptr);

				pContext->Dispatch((pColorRes->width + 15) / 16, (pColorRes->height + 15) / 16, 1);
				pD3D->pCurrentCSUnorderedAccessViews[0] = 0;
				ID3D11UnorderedAccessView* nullUAV = nullptr;
				pContext->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
			} else {
				FLOAT clearValues[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
				pContext->ClearUnorderedAccessViewFloat(pD3DPostProcess->MotionVectorUAV, clearValues);
			}
			pContext->CopyResource(pD3DPostProcess->PreviousCB_xgl_system, *old_cb0);
		}

		pNGX_dlss->ColorBuffer = pD3DPostProcess->OutColor[playerIndex];
		pD3DPostProcess->Context = pContext;

		pNGX_dlss->OutColor = pColorRes->pTexture;
		pNGX_dlss->resolution[0] = pColorRes->width;
		pNGX_dlss->resolution[1] = pColorRes->height;

		/*if (Config_PostProcess) {
			pNGX_dlss->DepthBuffer = pD3DPostProcess->LinearDepth[playerIndex];
		} else {
			
		}*/
		pNGX_dlss->DepthBuffer = pDSVInfo->pTexture;

		DLSS_Evaluate(playerIndex);
	} else {
		pContext->CopyResource(pColorRes->pTexture, pD3DPostProcess->OutColor[playerIndex]);
	}

	return saveRCX;
}

void __fastcall DLSS_Release(){

	if (pD3DPostProcess) {
		pD3DPostProcess->ReleaseBuffer();
		_aligned_free(pD3DPostProcess);
		pD3DPostProcess = nullptr;
	}

	if (pNGX_dlss) {
#if defined(HASVK)
		if (pNGX_dlss->vkDevice) {
			if (pNGX_dlss->m_xessContext) {
				xessDestroyContext(pNGX_dlss->m_xessContext);
			} else {
				NVSDK_NGX_VULKAN_DestroyParameters(pNGX_dlss->m_ngxParameters);
				if (pNGX_dlss->m_bDlssAvailable) {
					NVSDK_NGX_VULKAN_ReleaseFeature(pNGX_dlss->m_dlssFeature);
				}
				NVSDK_NGX_VULKAN_Shutdown1(nullptr);
			}

			vkFreeCommandBuffers(pNGX_dlss->vkDevice, pNGX_dlss->vkCMDPool, 1, &pNGX_dlss->vkCMDlist);
			vkDestroyCommandPool(pNGX_dlss->vkDevice, pNGX_dlss->vkCMDPool, nullptr);
			vkDestroyFence(pNGX_dlss->vkDevice, pNGX_dlss->dlssFence, nullptr);
		} else
#endif
		{
			NVSDK_NGX_D3D11_DestroyParameters(pNGX_dlss->m_ngxParameters);
			if (pNGX_dlss->m_bDlssAvailable) {
				NVSDK_NGX_D3D11_ReleaseFeature(pNGX_dlss->m_dlssFeature);
			}
			NVSDK_NGX_D3D11_Shutdown1(nullptr);
		}
		_aligned_free(pNGX_dlss);
		pNGX_dlss = nullptr;

	}
	// end
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

	if (!Config_DLAA || pNGX_dlss) return;

#if ndef(HASVK)
	static const GUID IID_IDXGIVkInteropDevice = { 0xe2ef5fa5, 0xdc21, 0x4af7, { 0x90, 0xc4, 0xf6, 0x7e, 0xf6, 0xa0, 0x93, 0x23 } };
	void* pVkInterop = nullptr;
	HRESULT hr = device->QueryInterface(IID_IDXGIVkInteropDevice, (void**)&pVkInterop);
	if (pVkInterop) return;
#endif

	auto p = (PNGX_DLSS)_aligned_malloc(sizeof(NGX_DLSS_t), 16U);
	if (!p) return;

	ZeroMemory(p, sizeof(NGX_DLSS_t));
	pNGX_dlss = p;

	p->JitterIndex = 1;
	/*const char* path = getenv("DXVK_ENABLE_NVAPI");
	if (path[0] == '1') {
		MessageBoxW(NULL, L"has DXVK_ENABLE_NVAPI", L"test", MB_OK);
	}*/

#if defined(HASVK)
	IDXGIVkInteropDevice* pVkInterop = nullptr;
	HRESULT hr = device->QueryInterface(IID_IDXGIVkInteropDevice, (void**)&pVkInterop);
	if (pVkInterop) {
		pVkInterop->GetVulkanHandles(&p->vkInstance, &p->vkPhysDevice, &p->vkDevice);
		pVkInterop->GetSubmissionQueue(&p->vkQueue, &p->QueueFamilyIndex);
		pVkInterop->Release();
	}

	if (p->vkDevice) {
		if(!LoadVulkanLibrary()){
			DLSS_TriggerFailureResult(100086, 1);
			return;
		}

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = p->QueueFamilyIndex;
		if (vkCreateCommandPool(p->vkDevice, &poolInfo, nullptr, &p->vkCMDPool) != VK_SUCCESS) {
			DLSS_TriggerFailureResult(3335, 1);
			return;
		}

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = p->vkCMDPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;
		if (vkAllocateCommandBuffers(p->vkDevice, &allocInfo, &p->vkCMDlist) != VK_SUCCESS) {
			DLSS_TriggerFailureResult(3336, 1);
			return;
		}

		p->pVkInterop = pVkInterop;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		vkCreateFence(p->vkDevice, &fenceInfo, nullptr, &p->dlssFence);

		// set xess
		if (Config_DLAA == 4) {
			auto status = xessVKCreateContext(p->vkInstance, p->vkPhysDevice, p->vkDevice, &p->m_xessContext);
			if (status != XESS_RESULT_SUCCESS) {
				DLSS_TriggerFailureResult((UINT32)(-status), 1);
				return;
			}

			//xessSetLoggingCallback(p->m_xessContext, XESS_LOGGING_LEVEL_DEBUG, XeSSLoggingCallback);

			return;
		}

		NVSDK_NGX_FeatureCommonInfo featureCommonInfo = {};
		featureCommonInfo.LoggingInfo.DisableOtherLoggingSinks = false;
		featureCommonInfo.LoggingInfo.MinimumLoggingLevel = NVSDK_NGX_LOGGING_LEVEL_VERBOSE;
		auto vkresult = NVSDK_NGX_VULKAN_Init(231313132, L"Z:\\TEMP", p->vkInstance, p->vkPhysDevice, p->vkDevice,0,0, &featureCommonInfo);
		if (vkresult != NVSDK_NGX_Result_Success) {
			DLSS_TriggerFailureResult((UINT32)vkresult - NVSDK_NGX_Result_Fail, 1);
			return;
		}

		vkresult = NVSDK_NGX_VULKAN_GetCapabilityParameters(&pNGX_dlss->m_ngxParameters);
		if (vkresult != NVSDK_NGX_Result_Success) {
			DLSS_TriggerFailureResult((UINT32)(vkresult - NVSDK_NGX_Result_Fail) + 200, 1);
			return;
		}
	} else
#endif
	{
		/*
		NVSDK_NGX_FeatureCommonInfo featureCommonInfo = {};
		featureCommonInfo.LoggingInfo.DisableOtherLoggingSinks = false;
		featureCommonInfo.LoggingInfo.MinimumLoggingLevel = NVSDK_NGX_LOGGING_LEVEL_VERBOSE;
		auto slresult = NVSDK_NGX_D3D11_Init(231313132, L"Z:\\TEMP", device, &featureCommonInfo);
		*/

		auto slresult = NVSDK_NGX_D3D11_Init(231313132, L".", device); // this id is from ngx_dlss_demo 
		if (slresult != NVSDK_NGX_Result_Success) {
			DLSS_TriggerFailureResult((UINT32)slresult - NVSDK_NGX_Result_Fail, 1);
			return;
		}

		slresult = NVSDK_NGX_D3D11_GetCapabilityParameters(&pNGX_dlss->m_ngxParameters);
		if (slresult != NVSDK_NGX_Result_Success) {
			DLSS_TriggerFailureResult((UINT32)(slresult - NVSDK_NGX_Result_Fail) + 200, 1);
			return;
		}
	}


	if (Config_DLAA == 3){
		NVSDK_NGX_Parameter_SetUI(p->m_ngxParameters, NVSDK_NGX_Parameter_DLSS_Hint_Render_Preset_DLAA, NVSDK_NGX_DLSS_Hint_Render_Preset_L);
	}
	else {
		NVSDK_NGX_Parameter_SetUI(p->m_ngxParameters, NVSDK_NGX_Parameter_DLSS_Hint_Render_Preset_DLAA, NVSDK_NGX_DLSS_Hint_Render_Preset_Default);
	}

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

	if (p->m_bDlssAvailable) {
#if defined(HASVK)
		if (p->vkDevice) {
			NVSDK_NGX_VULKAN_ReleaseFeature(pNGX_dlss->m_dlssFeature);
		} else
#endif
		{
			NVSDK_NGX_D3D11_ReleaseFeature(pNGX_dlss->m_dlssFeature);
		}

		DLSS_Reset();
	}
	p->m_bDlssAvailable = 0;

#if defined(HASVK)
	if (p->m_xessContext) {
		DLSS_Reset();

		xess_vk_init_params_t params;
		memset(&params, 0, sizeof(params));
		params.outputResolution.x = Width;
		params.outputResolution.y = Height;
		params.qualitySetting = XESS_QUALITY_SETTING_AA;
		params.initFlags = 0; // XESS_INIT_FLAG_HIGH_RES_MV
		params.creationNodeMask = 0;
		params.visibleNodeMask = 0;
		params.tempBufferHeap = VK_NULL_HANDLE;
		params.tempTextureHeap = VK_NULL_HANDLE;

		auto status = xessVKInit(p->m_xessContext, &params);

		if (status != XESS_RESULT_SUCCESS) {
			DLSS_TriggerFailureResult((UINT32)(-status), 1);
			return;
		}

		xess_2d_t outputResolution = { Width, Height };
		xess_2d_t returnValue[3];
		xessGetOptimalInputResolution(p->m_xessContext, &outputResolution, XESS_QUALITY_SETTING_AA, &returnValue[0], &returnValue[1], &returnValue[2]);
		//MessageBoxW(NULL, L"vulkan done!", L"test", MB_OK);

		return;
	}
#endif

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


	NVSDK_NGX_Result ResultDLSS;
#if defined(HASVK)
	if (p->vkDevice) {
		DLSS_VK_Enter(p);

		ResultDLSS = NGX_VULKAN_CREATE_DLSS_EXT(p->vkCMDlist, 0, 0, &p->m_dlssFeature, p->m_ngxParameters, &DlssCreateParams);

		DLSS_VK_Leave(p, VK_NULL_HANDLE);
	} else
#endif
	{
		ResultDLSS = NGX_D3D11_CREATE_DLSS_EXT(pContext, &p->m_dlssFeature, p->m_ngxParameters, &DlssCreateParams);
	}

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
	result -= 0.5f;
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

void __fastcall DLSS_Evaluate(int playerIndex){

	float jitter[2];
	DLSS_GetJitter(jitter);
#if defined(HASVK)
	auto vkDevice = pNGX_dlss->vkDevice;
	if (vkDevice) {
		NVSDK_NGX_Resource_VK pInOutput{};
		NVSDK_NGX_Resource_VK_Get(vkDevice, pNGX_dlss->OutColor, &pInOutput);
		NVSDK_NGX_Resource_VK pInDepth{};
		NVSDK_NGX_Resource_VK_Get(vkDevice, pNGX_dlss->DepthBuffer, &pInDepth);
		/*
		NVSDK_NGX_Resource_VK pInColor{};
		NVSDK_NGX_Resource_VK_Get(vkDevice, pNGX_dlss->ColorBuffer, &pInColor);
		NVSDK_NGX_Resource_VK pInMotionVectors{};
		NVSDK_NGX_Resource_VK_Get(vkDevice, pD3DPostProcess->BlackMV, &pInMotionVectors);
		*/

		if (pNGX_dlss->m_xessContext) {
			xess_vk_execute_params_t exec_params;
			memset(&exec_params, 0, sizeof(xess_vk_execute_params_t));

			memcpy(&exec_params.colorTexture, &pNGX_dlss->vkInColor[playerIndex].Resource.ImageViewInfo, sizeof(xess_vk_image_view_info));
			memcpy(&exec_params.depthTexture, &pInDepth.Resource.ImageViewInfo, sizeof(xess_vk_image_view_info));
			memcpy(&exec_params.velocityTexture, &pNGX_dlss->vkMV.Resource.ImageViewInfo, sizeof(xess_vk_image_view_info));
			memcpy(&exec_params.outputTexture, &pInOutput.Resource.ImageViewInfo, sizeof(xess_vk_image_view_info));

			exec_params.inputWidth = pNGX_dlss->resolution[0];
			exec_params.inputHeight = pNGX_dlss->resolution[1];
			exec_params.jitterOffsetX = jitter[0];
			exec_params.jitterOffsetY = jitter[1];
			exec_params.exposureScale = 1.0f;
			exec_params.resetHistory = pNGX_dlss->IsReset;

			DLSS_VK_Enter(pNGX_dlss);
			auto status = xessVKExecute(pNGX_dlss->m_xessContext, pNGX_dlss->vkCMDlist, &exec_params);
			DLSS_VK_Leave(pNGX_dlss, pNGX_dlss->dlssFence);


			/*
			static int count = 0;
			if (!count) {
				if (status == XESS_RESULT_SUCCESS) {
					MessageBoxW(NULL, L"Done!", L"info", MB_OK);
				} else {
					MessageBoxW(NULL, std::to_wstring((UINT32)-status).c_str(), L"error", MB_OK);
				}
				count++;
			}*/
		} else {
			NVSDK_NGX_VK_DLSS_Eval_Params D3DvkDlssEvalParams;
			memset(&D3DvkDlssEvalParams, 0, sizeof(D3DvkDlssEvalParams));

			D3DvkDlssEvalParams.Feature.pInColor = &pNGX_dlss->vkInColor[playerIndex];
			D3DvkDlssEvalParams.Feature.pInOutput = &pInOutput;
			D3DvkDlssEvalParams.Feature.InSharpness = 1;

			D3DvkDlssEvalParams.pInDepth = &pInDepth;
			D3DvkDlssEvalParams.pInMotionVectors = &pNGX_dlss->vkMV;

			D3DvkDlssEvalParams.InJitterOffsetX = jitter[0];
			D3DvkDlssEvalParams.InJitterOffsetY = jitter[1];
			D3DvkDlssEvalParams.InRenderSubrectDimensions.Width = pNGX_dlss->resolution[0];
			D3DvkDlssEvalParams.InRenderSubrectDimensions.Height = pNGX_dlss->resolution[1];

			D3DvkDlssEvalParams.InReset = pNGX_dlss->IsReset;
			D3DvkDlssEvalParams.InMVScaleX = 1.0;
			D3DvkDlssEvalParams.InMVScaleY = 1.0;
			NVSDK_NGX_Parameter_SetF(pNGX_dlss->m_ngxParameters, NVSDK_NGX_Parameter_Denoise, 1.0);


			DLSS_VK_Enter(pNGX_dlss);
			auto ResultDLSS = NGX_VULKAN_EVALUATE_DLSS_EXT(pNGX_dlss->vkCMDlist, pNGX_dlss->m_dlssFeature, pNGX_dlss->m_ngxParameters, &D3DvkDlssEvalParams);
			DLSS_VK_Leave(pNGX_dlss, pNGX_dlss->dlssFence);
		}

		vkWaitForFences(vkDevice, 1, &pNGX_dlss->dlssFence, VK_TRUE, UINT64_MAX);
		vkResetFences(vkDevice, 1, &pNGX_dlss->dlssFence);

		NVSDK_NGX_Resource_VK_Destroy(vkDevice, &pInOutput);
		NVSDK_NGX_Resource_VK_Destroy(vkDevice, &pInDepth);
		/*
		NVSDK_NGX_Resource_VK_Destroy(vkDevice, &pInColor);
		NVSDK_NGX_Resource_VK_Destroy(vkDevice, &pInMotionVectors); */
	} else
#endif
	{
		NVSDK_NGX_D3D11_DLSS_Eval_Params D3D11DlssEvalParams;
		memset(&D3D11DlssEvalParams, 0, sizeof(D3D11DlssEvalParams));

		D3D11DlssEvalParams.Feature.pInColor = pNGX_dlss->ColorBuffer;
		D3D11DlssEvalParams.Feature.pInOutput = pNGX_dlss->OutColor;
		D3D11DlssEvalParams.Feature.InSharpness = 1;

		D3D11DlssEvalParams.pInDepth = pNGX_dlss->DepthBuffer;
		D3D11DlssEvalParams.pInMotionVectors = pD3DPostProcess->BlackMV;

		D3D11DlssEvalParams.InJitterOffsetX = jitter[0];
		D3D11DlssEvalParams.InJitterOffsetY = jitter[1];
		D3D11DlssEvalParams.InRenderSubrectDimensions.Width = pNGX_dlss->resolution[0];
		D3D11DlssEvalParams.InRenderSubrectDimensions.Height = pNGX_dlss->resolution[1];

		D3D11DlssEvalParams.InReset = pNGX_dlss->IsReset;
		D3D11DlssEvalParams.InMVScaleX = 1.0;
		D3D11DlssEvalParams.InMVScaleY = 1.0;

		NVSDK_NGX_Parameter_SetF(pNGX_dlss->m_ngxParameters, NVSDK_NGX_Parameter_Denoise, 1.0);
		NGX_D3D11_EVALUATE_DLSS_EXT(pD3DPostProcess->Context, pNGX_dlss->m_dlssFeature, pNGX_dlss->m_ngxParameters, &D3D11DlssEvalParams);
	}

	pNGX_dlss->IsReset = 0;

	DLSS_ClearBuffer();
}

void __fastcall DLSS_VK_Enter(void* pDLSS) {
#if defined(HASVK)
	PNGX_DLSS p = (PNGX_DLSS)pDLSS;

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	p->pVkInterop->FlushRenderingCommands();
	p->pVkInterop->LockSubmissionQueue();

	vkBeginCommandBuffer(p->vkCMDlist, &beginInfo);
#endif
}

void __fastcall DLSS_VK_Leave(void* pDLSS, void* pFence) {
#if defined(HASVK)
	PNGX_DLSS p = (PNGX_DLSS)pDLSS;

	vkEndCommandBuffer(p->vkCMDlist);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &p->vkCMDlist;
	vkQueueSubmit(p->vkQueue, 1, &submitInfo, (VkFence)pFence);

	p->pVkInterop->ReleaseSubmissionQueue();
#endif
}
