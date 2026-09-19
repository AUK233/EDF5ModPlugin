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
#include "zHui/ToGui/DirectXTK/DDSTextureLoader.h"

#include "Base/g_system.h"
#include "Base/g_criFS.h"
#include "shader/1SetPostProcess_CS.hpp"
#include "shader/1SetPostProcess_MV.hpp"
#include "shader/1SetPostProcess_FG.hpp"

#include "0SL_PostProcess.h"

extern "C" {
	extern int Config_PostProcess;
	extern int Config_PostProcessTexIndex;
}

namespace D3D {
	void AddPostProcess_t::Initialize(ID3D11Device* device, ID3D11DeviceContext* context){

		// create constant buffer
		D3D11_BUFFER_DESC cb_desc0 = {};
		cb_desc0.Usage = D3D11_USAGE_DEFAULT;
		cb_desc0.ByteWidth = sizeof(xgl_system_CB_t);
		cb_desc0.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb_desc0.CPUAccessFlags = 0;
		device->CreateBuffer(&cb_desc0, nullptr, &CB_Previous_xgl_system);

		// load compute shader
		ComPtr<ID3DBlob> cs_blob;
		ComPtr<ID3DBlob> error_blob;

		if (Config_PostProcess == 2) {
			auto hr = D3DCompileFromFile(L"./subtitle/test.hlsl", nullptr, nullptr, "CS_main", "cs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &cs_blob, &error_blob);
			if (hr == S_OK) {
				device->CreateComputeShader(cs_blob->GetBufferPointer(), cs_blob->GetBufferSize(), nullptr, &m_PostProcessCS);
			}
		} else {
			device->CreateComputeShader(D3DPostProcess_ComputeShader, sizeof(D3DPostProcess_ComputeShader), nullptr, &m_PostProcessCS);
		}

		device->CreateComputeShader(D3DPostProcess_MotionVector, sizeof(D3DPostProcess_MotionVector), nullptr, &m_MotionVectorCS);
		device->CreateComputeShader(D3DPostProcess_ToFGBuffer, sizeof(D3DPostProcess_ToFGBuffer), nullptr, &m_ToFGBufferCS);
	}

	void AddPostProcess_t::Buffer_Create(ID3D11Device* device, const AddPostProcessRes_t& resolution, ID3D12Device* device12, UINT DLSS_Level) {
		m_resolution = resolution;

		D3D11_TEXTURE2D_DESC outDesc = {};
		outDesc.Width = resolution.Width;
		outDesc.Height = resolution.Height;
		outDesc.MipLevels = 1;
		outDesc.ArraySize = 1;
		outDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		outDesc.SampleDesc.Count = 1;
		outDesc.SampleDesc.Quality = 0;
		outDesc.Usage = D3D11_USAGE_DEFAULT;
		outDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
		outDesc.CPUAccessFlags = 0;
		outDesc.MiscFlags = 0;
		m_ColorRes[0].D3D_Create(device, &outDesc, device12);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;

		if (m_ColorRes[0].d11) device->CreateUnorderedAccessView(m_ColorRes[0].d11, &uavDesc, &m_ColorUAV[0]);

		if (m_playerCount == 2) {
			m_ColorRes[1].D3D_Create(device, &outDesc, device12);

			if (m_ColorRes[1].d11) device->CreateUnorderedAccessView(m_ColorRes[1].d11, &uavDesc, &m_ColorUAV[1]);
		}
	}

	void AddPostProcess_t::Buffer_Release(void* deviceVK) {
		D3DResourceCommonRelease(m_ColorUAV[0]);
		m_ColorRes[0].Release(deviceVK);

		D3DResourceCommonRelease(m_ColorUAV[1]);
		m_ColorRes[1].Release(deviceVK);
	}

	void AddPostProcess_t::Execute(Pg_D3D11DeviceInfo pD3D, int playerIndex, const AddPostProcessRes_t& ThreadGroupCount, ID3D11ShaderResourceView** ppColor, ID3D11ShaderResourceView** ppDepth){
		auto context = pD3D->context;

		context->CSSetShaderResources(0, 1, ppColor);
		context->CSSetShaderResources(1, 1, ppDepth);
		context->CSSetShaderResources(9, 1, &m_LookupTable_SRV);
		context->CSSetSamplers(11, 1, &m_LUTSamplerLinear);

		context->CSSetShader(m_PostProcessCS, nullptr, 0);

		context->CSSetUnorderedAccessViews(0, 1, &m_ColorUAV[playerIndex], nullptr);
		context->Dispatch(ThreadGroupCount.Width, ThreadGroupCount.Height, 1);

		pD3D->pCurrentCSShaderResourceView[0] = 0;
		pD3D->pCurrentCSShaderResourceView[1] = 0;
		pD3D->pCurrentCSShaderResourceView[9] = 0;
		pD3D->pCurrentCSShader = 0;
		pD3D->pCurrentCSSamplerState[11] = 0;
		pD3D->pCurrentCSUnorderedAccessViews[0] = 0;

		ID3D11ShaderResourceView* nullSRV = nullptr;
		context->CSSetShaderResources(0, 1, &nullSRV);
		context->CSSetShaderResources(1, 1, &nullSRV);
		context->CSSetShaderResources(9, 1, &nullSRV);
		ID3D11SamplerState* nullSampler = nullptr;
		context->CSSetSamplers(11, 1, &nullSampler);
		ID3D11ComputeShader* nullCS = nullptr;
		context->CSSetShader(nullptr, nullptr, 0);
		ID3D11UnorderedAccessView* nullUAV = nullptr;
		context->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
	}

	void AddPostProcess_t::LUTBuffer_Load(ID3D11Device* device) {
		D3D11_SAMPLER_DESC samp_desc = {};
		samp_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samp_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samp_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samp_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samp_desc.MaxAnisotropy = 1;
		samp_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samp_desc.MinLOD = 0;
		samp_desc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT hr = device->CreateSamplerState(&samp_desc, &m_LUTSamplerLinear);

		// =====================================================================
		auto index = Config_PostProcessTexIndex;
		if (index > 3) index = 0;

		m_LookupTable_SRV = LUTBuffer_Create(device, index);
	}

	void AddPostProcess_t::LUTBuffer_Reload(ID3D11Device* device){
		if (m_LookupTable_SRV_Backup) {
			m_LookupTable_SRV_Backup->Release();
			m_LookupTable_SRV_Backup = nullptr;
		}

		if (m_LUTindex > 3) m_LUTindex = 0;

		m_LookupTable_SRV_Backup = m_LookupTable_SRV;
		m_LookupTable_SRV = LUTBuffer_Create(device, m_LUTindex);
	}

	ID3D11ShaderResourceView* AddPostProcess_t::LUTBuffer_Create(ID3D11Device* device, int index) {
		CriFileSystemGet_t getFile;
		auto pFile = getFile.Open(GetLUTFilePath(index));
		// L"./subtitle/LUT_DefaultEnhance.dds"

		ID3D11ShaderResourceView* out;
		DirectX::CreateDDSTextureFromMemory(device, getFile.fs->data, getFile.fs->data_size, nullptr, &out);

		return out;
	}

	std::wstring AddPostProcess_t::GetLUTFilePath(int index) {
		switch (index) {
		case 1:
			return L"app:/ui/LUT_HistoricalReproduction.dds";
		case 2:
			return L"app:/ui/LUT_DefaultEnhance1.dds";
		case 3:
			return L"app:/ui/LUT_DefaultEnhance2.dds";
		default:
			return L"app:/ui/LUT_DefaultEnhance0.dds";
		}
		// end
	}
// end
}
