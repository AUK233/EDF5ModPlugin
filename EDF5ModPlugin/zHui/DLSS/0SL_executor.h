#pragma once
#pragma comment(lib, "DXGI.lib")
#include "0SL_common.h"
#include "sllib/sl.h"
#include "sllib/sl_dlss.h"
#include "sllib/sl_dlss_g.h"
#include "zHui/ToGui/0GetDXGI.h"

constexpr auto D3D12WrappedBackBuffersCount = 3;

namespace D3D {
	class StreamLineResource_t {
	public:
		sl::Resource res;
		sl::ResourceTag tag;

		void _fastcall CreateFromD11(ID3D11Texture2D* tex, sl::BufferType type, sl::ResourceLifecycle lifecycle, sl::Extent* extent);
		void _fastcall CreateFromD12(ID3D12Resource* tex, sl::BufferType type, sl::ResourceLifecycle lifecycle, sl::Extent* extent, uint32_t state);
		void _fastcall CreateFromVK(PVKResource vkRes, sl::BufferType type, sl::ResourceLifecycle lifecycle, sl::Extent* extent);
	};

	enum class StreamLineProcessorStatus_t : int {
		eFailed = 0,
		eD3D11 = 1,
		eD3D12 = 2,
		eVULKAN = 3,
	};

	__declspec(align(16)) class StreamLineProcessor_t {
	public:
		StreamLineProcessorStatus_t m_status;
		bool m_bGetNewFrame;
		int m_renderAPI;
		PslVulkanAPI vk;

		// streamline =================================================
		sl::ViewportHandle myViewport[2];
		sl::FrameToken* currentFrame;
		std::wstring slDirStr;
		sl::Boolean IsReset;
		int JitterIndex; float v_jitter[2];
		// streamline end =============================================

		// D3D12 =====================================================
		ID3D12Device* m_d3d12Device; ID3D12CommandQueue* m_commandQueue;
		IDXGIFactory7* m_dxgiFactory; IDXGISwapChain3* m_dxgiSwapChain3;
		ID3D12CommandAllocator* m_commandAllocator;
		ID3D12GraphicsCommandList* m_commandList;

		ID3D12Fence* m_fenceNative; UINT64 m_nativeFenceValue; HANDLE m_fenceEvent;
		ID3D11Fence* m_fence11; ID3D12Fence* m_fence12; UINT64 m_shareFenceValue;

		ID3D12Resource* v_d3d12BackBuffers[D3D12WrappedBackBuffersCount];
		DXSharedTexture2D g_gameSceneResource;
		// End   =====================================================



		void __fastcall CreateD3D12Device();

		HRESULT __fastcall SwapChainGetBuffer(PGameDXGIRender pGameDXGI, ID3D11Texture2D** pOut);
		void __fastcall SwapChainPresent(PGameDXGIRender pGameDXGI);
		void __fastcall WaitFinish();
		void __fastcall Release();

		void __fastcall GetNewFrame();
		sl::FrameToken* __fastcall ClearFrame();
		void __fastcall Reset();
		float __fastcall Halton(int index, int base);
		void __fastcall GetJitter(int playerCount);
	};
	typedef StreamLineProcessor_t* PStreamLineProcessor;
}