#pragma once
#include <d3d11_4.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d11on12.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;

#include "0SL_vulkanAPI.hpp"

//#define EnableDXDebugInformation

namespace D3D{
	typedef struct VKResource_t {
		VkImage vkImage;
		VkImageView vkImageView;
		VkImageLayout vkLayout;
	} *PVKResource;

	class DXSharedTexture2D {
	public:
		ID3D11Texture2D* d11;
		union {
			ID3D12Resource* d12;
			PVKResource vk;
		};
		

		void D3D_Create(ID3D11Device* device, D3D11_TEXTURE2D_DESC* pDesc, ID3D12Device* device12);
		void D3D_Release();
		void D3D_Reload(ID3D11Device* device, D3D11_TEXTURE2D_DESC* pDesc, ID3D12Device* device12);

		void VK_CreateFromD11(PslVulkanAPI deviceVK);
		// don't call D3D_Release()
		void VK_Release(PslVulkanAPI deviceVK);


		void Release(PslVulkanAPI deviceVK);
	};

	void __fastcall D3DResourceCommonRelease(IUnknown* pIn);
}
