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
#include <vector>
#include <cstdlib>
#include <d3dcompiler.h>
#include <wrl.h>
#include "zHui/ToGui/DirectXTK/DDSTextureLoader.h"

#include "Base/g_DXresource.h"
#include "Base/g_criFS.h"
#include "shader/1SetPostProcess_CS.hpp"
#include "shader/1SetPostProcess_MV.hpp"
#include "1SetPostProcess.h"

extern "C" {
	extern int Config_PostProcess;
}

namespace D3D {
	void D3DPostProcess_t::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, DXGI_SWAP_CHAIN_DESC* pChainDesc) {

		Device = device;
		Context = context;

		//SetBuffer(pChainDesc->BufferDesc.Width, pChainDesc->BufferDesc.Height);
		LoadComputeShader();

		// create constant buffer
		D3D11_BUFFER_DESC cb_desc0 = {};
		cb_desc0.Usage = D3D11_USAGE_DEFAULT;
		cb_desc0.ByteWidth = sizeof(xgl_system_CB_t);
		cb_desc0.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb_desc0.CPUAccessFlags = 0;
		device->CreateBuffer(&cb_desc0, nullptr, &PreviousCB_xgl_system);
	}

	void D3DPostProcess_t::LoadComputeShader() {
		Microsoft::WRL::ComPtr<ID3DBlob> cs_blob;
		Microsoft::WRL::ComPtr<ID3DBlob> error_blob;
		//ID3DBlob* cs_blob = nullptr;
		//ID3DBlob* error_blob = nullptr;

		if (Config_PostProcess == 2) {
			auto hr = D3DCompileFromFile(L"./subtitle/test.hlsl", nullptr, nullptr, "CS_main", "cs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &cs_blob, &error_blob);
			if (hr == S_OK) {
				Device->CreateComputeShader(cs_blob->GetBufferPointer(), cs_blob->GetBufferSize(), nullptr, &PostProcessCS);
			}
		} else {
			Device->CreateComputeShader(D3DPostProcess_ComputeShader, sizeof(D3DPostProcess_ComputeShader), nullptr, &PostProcessCS);
		}

		Device->CreateComputeShader(D3DPostProcess_MotionVector, sizeof(D3DPostProcess_MotionVector), nullptr, &MotionVectorCS);
		// end
	}

	void D3DPostProcess_t::ReleaseBuffer() {
		if (OutUAV[0]) {
			OutUAV[0]->Release();
			OutUAV[0] = nullptr;
		}

		if (OutColor[0]) {
			OutColor[0]->Release();
			OutColor[0] = nullptr;
		}

		// ===========================

		if (OutUAV[1]) {
			OutUAV[1]->Release();
			OutUAV[1] = nullptr;
		}

		if (OutColor[1]) {
			OutColor[1]->Release();
			OutColor[1] = nullptr;
		}

		// ===========================
		/*
		if (LinearDepthUAV[0]) {
			LinearDepthUAV[0]->Release();
			LinearDepthUAV[0] = nullptr;
		}

		if (LinearDepth[0]) {
			LinearDepth[0]->Release();
			LinearDepth[0] = nullptr;
		}

		if (LinearDepthUAV[1]) {
			LinearDepthUAV[1]->Release();
			LinearDepthUAV[1] = nullptr;
		}

		if (LinearDepth[1]) {
			LinearDepth[1]->Release();
			LinearDepth[1] = nullptr;
		}*/

		// ===========================

		if (MotionVectorUAV) {
			MotionVectorUAV->Release();
			MotionVectorUAV = nullptr;
		}

		if (BlackMV) {
			BlackMV->Release();
			BlackMV = nullptr;
		}
	}

	void D3DPostProcess_t::SetBuffer(UINT Width, UINT Height) {
		ReleaseBuffer();

		D3D11_TEXTURE2D_DESC outDesc = {};
		outDesc.Width = Width;
		outDesc.Height = Height;
		outDesc.MipLevels = 1;
		outDesc.ArraySize = 1;
		outDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		outDesc.SampleDesc.Count = 1;
		outDesc.SampleDesc.Quality = 0;
		outDesc.Usage = D3D11_USAGE_DEFAULT;
		outDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
		outDesc.CPUAccessFlags = 0;
		outDesc.MiscFlags = 0;
		Device->CreateTexture2D(&outDesc, 0, &OutColor[0]);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = outDesc.Format;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;

		if (OutColor[0]) {
			Device->CreateUnorderedAccessView(OutColor[0], &uavDesc, &OutUAV[0]);
		}

		if (PlayerCount == 2) {
			Device->CreateTexture2D(&outDesc, 0, &OutColor[1]);

			if (OutColor[1]) {
				Device->CreateUnorderedAccessView(OutColor[1], &uavDesc, &OutUAV[1]);
			}
		}

		// set linear depth buffer
		/*
		outDesc.Format = DXGI_FORMAT_R32_FLOAT;
		Device->CreateTexture2D(&outDesc, 0, &LinearDepth[0]);
		uavDesc.Format = DXGI_FORMAT_R32_FLOAT;
		Device->CreateUnorderedAccessView(LinearDepth[0], &uavDesc, &LinearDepthUAV[0]);

		if (PlayerCount == 2) {
			Device->CreateTexture2D(&outDesc, 0, &LinearDepth[1]);
			Device->CreateUnorderedAccessView(LinearDepth[1], &uavDesc, &LinearDepthUAV[1]);
		}*/

		// set black mv buffer
		outDesc.Format = DXGI_FORMAT_R16G16_FLOAT;
		//outDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;

		//UINT pixelSize = Width * 4;  // 2 bytes (X) + 2 bytes (Y)
		/*UINT dataSize = Height * pixelSize;
		std::vector<BYTE> zeroData(dataSize, 0);
		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = zeroData.data();
		initData.SysMemPitch = pixelSize;
		initData.SysMemSlicePitch = dataSize;
		Device->CreateTexture2D(&outDesc, &initData, &BlackMV);*/
		Device->CreateTexture2D(&outDesc, 0, &BlackMV);
		if (BlackMV) {
			uavDesc.Format = DXGI_FORMAT_R16G16_FLOAT;
			Device->CreateUnorderedAccessView(BlackMV, &uavDesc, &MotionVectorUAV);

			FLOAT clearValues[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			Context->ClearUnorderedAccessViewFloat(MotionVectorUAV, clearValues);
		}
		// end
	}

	void D3DPostProcess_t::LoadLUTBuffer() {
		D3D11_SAMPLER_DESC samp_desc = {};
		samp_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samp_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samp_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samp_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samp_desc.MaxAnisotropy = 1;
		samp_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samp_desc.MinLOD = 0;
		samp_desc.MaxLOD = D3D11_FLOAT32_MAX;

		HRESULT hr = Device->CreateSamplerState(&samp_desc, &LUTSamplerLinear);

		CriFileSystemGet_t getFile;
		auto fileIsExist = getFile.Open(L"app:/ui/LUT_DefaultEnhance.dds");

		DirectX::CreateDDSTextureFromMemory(Device, getFile.fs->data, getFile.fs->data_size, nullptr, &LookupTable_SRV);
	}

// end
}
