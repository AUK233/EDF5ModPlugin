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

	HRESULT __fastcall StreamLineProcessor_t::SwapChainGetBuffer(PGameDXGIRender pGameDXGI, ID3D11Texture2D** pOut){
		//g_gameSceneResource.D3D_Release(); use Reload() instead

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
		outDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; //  | D3D11_BIND_UNORDERED_ACCESS;
		outDesc.CPUAccessFlags = 0;
		//outDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED | D3D11_RESOURCE_MISC_SHARED_NTHANDLE;
		g_gameSceneResource.D3D_Reload(device, &outDesc, m_d3d12Device);

		*pOut = g_gameSceneResource.d11;
		g_gameSceneResource.d11->AddRef();


		// create fence for d3d11
		ComPtr<ID3D11Device5> d3d11Device5;
		auto hr = device->QueryInterface(IID_PPV_ARGS(&d3d11Device5));

		hr = d3d11Device5->CreateFence(0, D3D11_FENCE_FLAG_SHARED, IID_PPV_ARGS(&m_Fence11));
		HANDLE sharedHandle = nullptr;
		m_Fence11->CreateSharedHandle(nullptr, DXGI_SHARED_RESOURCE_READ | DXGI_SHARED_RESOURCE_WRITE, nullptr, &sharedHandle);
		hr = m_d3d12Device->OpenSharedHandle(sharedHandle, IID_PPV_ARGS(&m_fence12));
		CloseHandle(sharedHandle);

#if defined(EnableDXDebugInformation)
		std::wstring hrText = std::format(L"Shared Fence result: {:X}.\n", hr);
		OutputDebugStringW(hrText.c_str());
#endif	


		return hr;
	}

	void __fastcall StreamLineProcessor_t::SwapChainPresent(PGameDXGIRender pGameDXGI) {
		if (m_status != StreamLineProcessorStatus_t::eD3D12) return;

		ComPtr<ID3D11DeviceContext4> d3d11Context4;
		pGameDXGI->pD3D11DeviceContext->QueryInterface(IID_PPV_ARGS(&d3d11Context4));
		d3d11Context4->Signal(m_Fence11, ++m_shareFenceValue);
		d3d11Context4->Flush();

		UINT currentIndex = m_dxgiSwapChain3->GetCurrentBackBufferIndex();
		auto backBuffer = v_d3d12BackBuffers[currentIndex];

		m_commandAllocator->Reset();
		m_commandList->Reset(m_commandAllocator, nullptr);


		// barrier
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = backBuffer;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
		m_commandList->ResourceBarrier(1, &barrier);

		m_commandList->CopyResource(backBuffer, g_gameSceneResource.d12);

		// barrier
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		m_commandList->ResourceBarrier(1, &barrier);

		auto hr = m_commandList->Close();
#if defined(EnableDXDebugInformation)
		if (FAILED(hr)) {

			std::wstring hrText = std::format(L"Command List Error Code: {:X}.\n", hr);
			OutputDebugStringW(hrText.c_str());
		}
#endif	
		ID3D12CommandList* lists[] = { m_commandList };

		m_commandQueue->Wait(m_fence12, m_shareFenceValue);
		m_commandQueue->ExecuteCommandLists(1, lists);
		WaitFinish();


		//DXGI_PRESENT_PARAMETERS pp;
		//ZeroMemory(&pp, sizeof(pp));
		//m_dxgiSwapChain3->Present1(0, 0, &pp);
	}

	void __fastcall StreamLineProcessor_t::WaitFinish() {
		m_commandQueue->Signal(m_fenceNative, ++m_nativeFenceValue);

		if (m_fenceNative->GetCompletedValue() < m_nativeFenceValue) {
			m_fenceNative->SetEventOnCompletion(m_nativeFenceValue, m_fenceEvent);
			WaitForSingleObject(m_fenceEvent, INFINITE);
		}
	}

	void __fastcall StreamLineProcessor_t::Release() {
		if (m_status == StreamLineProcessorStatus_t::eD3D12) {
			BOOL fullscreen;
			IDXGIOutput* pTarget;
			m_dxgiSwapChain3->GetFullscreenState(&fullscreen, &pTarget);
			if (fullscreen == TRUE) {
				pTarget->Release();
				m_dxgiSwapChain3->SetFullscreenState(FALSE, nullptr);
			}

			WaitFinish();

			CloseHandle(m_fenceEvent);
			m_d3d12Device->Release();
		}
		// end
	}
}
