#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d11on12.h>
#pragma comment(lib, "DXGI.lib")
#include "zHui/ToGui/0GetDXGI.h"

constexpr auto D3D12WrappedBackBuffersCount = 3;

namespace D3D {
	enum class StreamLineProcessorStatus_t : int {
		eFailed = 0,
		eD3D11 = 1,
		eD3D12 = 2,
		eVULKAN = 3,
	};

	__declspec(align(16)) class StreamLineProcessor_t {
	public:
		StreamLineProcessorStatus_t m_status;

		ID3D12Device* m_d3d12Device; ID3D12CommandQueue* m_commandQueue;
		IDXGIFactory7* m_dxgiFactory; ID3D11On12Device2* m_d3d11On12Device;
		IDXGISwapChain3* m_dxgiSwapChain3;
		ID3D11Resource* v_wrappedBackBuffers[D3D12WrappedBackBuffersCount];
		ID3D11Texture2D* g_gameSceneResource;

		HRESULT __fastcall SwapChainGetBuffer(PGameDXGIRender pGameDXGI, ID3D11Texture2D** pOut);
		void __fastcall SwapChainPresent(PGameDXGIRender pGameDXGI);
	};
	typedef StreamLineProcessor_t* PStreamLineProcessor;
}