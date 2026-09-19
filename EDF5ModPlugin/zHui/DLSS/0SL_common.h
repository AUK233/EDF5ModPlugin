#pragma once
#include <d3d11_4.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d11on12.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;

//#define EnableDXDebugInformation

namespace D3D{
	class DXSharedTexture2D {
	public:
		ID3D11Texture2D* d11;
		union {
			ID3D12Resource* d12;
			void* vk;
		};
		

		void D3D_Create(ID3D11Device* device, D3D11_TEXTURE2D_DESC* pDesc, ID3D12Device* device12);
		void D3D_Release();
		void D3D_Reload(ID3D11Device* device, D3D11_TEXTURE2D_DESC* pDesc, ID3D12Device* device12);


		void Release(void* deviceVK);
	};

	void __fastcall D3DResourceCommonRelease(IUnknown* pIn);
}
