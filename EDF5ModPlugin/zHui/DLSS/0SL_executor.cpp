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

#include "0SL_executor.h"

namespace D3D {
	HRESULT __fastcall StreamLineProcessor_t::SwapChainGetBuffer(PGameDXGIRender pGameDXGI, ID3D11Texture2D** pOut){
		if (g_gameSceneResource) {
			g_gameSceneResource->Release();
			g_gameSceneResource = nullptr;
		}

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
		auto hr = device->CreateTexture2D(&outDesc, 0, &g_gameSceneResource);

		*pOut = g_gameSceneResource;
		g_gameSceneResource->AddRef();

		return hr;
	}

	void __fastcall StreamLineProcessor_t::SwapChainPresent(PGameDXGIRender pGameDXGI) {
		UINT currentIndex = m_dxgiSwapChain3->GetCurrentBackBufferIndex();

		m_d3d11On12Device->AcquireWrappedResources(&v_wrappedBackBuffers[currentIndex], 1);

		auto context = pGameDXGI->pD3D11DeviceContext;
		context->CopyResource(v_wrappedBackBuffers[currentIndex], g_gameSceneResource);

		m_d3d11On12Device->ReleaseWrappedResources(&v_wrappedBackBuffers[currentIndex], 1);

		context->Flush();

		//DXGI_PRESENT_PARAMETERS pp;
		//ZeroMemory(&pp, sizeof(pp));
		//m_dxgiSwapChain3->Present1(0, 0, &pp);
    }
}
