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

#include "0SL_common.h"

namespace D3D {
	void DXSharedTexture2D::D3D_Create(ID3D11Device* device, D3D11_TEXTURE2D_DESC* pDesc, ID3D12Device* device12){
		if (device12) {
			pDesc->MiscFlags = D3D11_RESOURCE_MISC_SHARED | D3D11_RESOURCE_MISC_SHARED_NTHANDLE;
		}

		auto hr = device->CreateTexture2D(pDesc, 0, &d11);
		if (!device12 || FAILED(hr)) {
			d12 = nullptr;
			return;
		}

		ComPtr<IDXGIResource1> dxgiRes;
		d11->QueryInterface(IID_PPV_ARGS(&dxgiRes));

		HANDLE sharedHandle = nullptr;
		dxgiRes->CreateSharedHandle(nullptr, DXGI_SHARED_RESOURCE_READ, nullptr, &sharedHandle);

		hr = device12->OpenSharedHandle(sharedHandle, IID_PPV_ARGS(&d12));
		CloseHandle(sharedHandle);
	}

	void DXSharedTexture2D::D3D_Release(){
		D3DResourceCommonRelease(d11);
		D3DResourceCommonRelease(d12);
	}

	void DXSharedTexture2D::D3D_Reload(ID3D11Device* device, D3D11_TEXTURE2D_DESC* pDesc, ID3D12Device* device12){
		D3D_Release();
		D3D_Create(device, pDesc, device12);
	}

	void DXSharedTexture2D::VK_CreateFromD11(PslVulkanAPI deviceVK) {
		if(!d11) return;

		IDXGIVkInteropSurface* pVkSurface = nullptr;
		HRESULT hr = d11->QueryInterface(IID_IDXGIVkInteropSurface, (void**)&pVkSurface);
		if (!pVkSurface) return;

		vk = (PVKResource)_aligned_malloc(sizeof(VKResource_t), 16);
		if(!vk) return;
		ZeroMemory(vk, sizeof(VKResource_t));

		VkImage vkImage = VK_NULL_HANDLE;
		VkImageLayout vkLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		VkImageCreateInfo vkInfo{};
		vkInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		hr = pVkSurface->GetVulkanImageInfo(&vkImage, &vkLayout, &vkInfo);
		pVkSurface->Release();
		if (FAILED(hr)) return;

		// create image view
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.pNext = nullptr;
		viewInfo.image = vkImage;
		viewInfo.flags = 0;
		viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		if (vkInfo.arrayLayers > 1) {
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
		} else {
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		}
		viewInfo.format = vkInfo.format;
		
		// check is depth format
		if (vkInfo.format == VK_FORMAT_D16_UNORM || vkInfo.format == VK_FORMAT_D32_SFLOAT ||
			vkInfo.format == VK_FORMAT_D24_UNORM_S8_UINT || vkInfo.format == VK_FORMAT_D32_SFLOAT_S8_UINT) {
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		} else {
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = vkInfo.mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = vkInfo.arrayLayers;
		VkImageView vkImageView;
		VkResult result = deviceVK->vkCreateImageView(deviceVK->m_vkDevice, &viewInfo, nullptr, &vkImageView);
		if (result != VK_SUCCESS) return;

		vk->vkImage = vkImage;
		vk->vkImageView = vkImageView;
		vk->vkLayout = vkLayout;
	}

	void DXSharedTexture2D::VK_Release(PslVulkanAPI deviceVK) {
		D3DResourceCommonRelease(d11);
		if (!vk) return;

		auto vkImageView = vk->vkImageView;
		if (vkImageView) {
			deviceVK->vkDestroyImageView(deviceVK->m_vkDevice, vkImageView, nullptr);
		}

		_aligned_free(vk);
		vk = nullptr;
	}

	void DXSharedTexture2D::Release(PslVulkanAPI deviceVK){
		if (!d11) return;

		if (!deviceVK) {
			D3D_Release();
		}else{
			VK_Release(deviceVK);
		}
	}

	void __fastcall D3DResourceCommonRelease(IUnknown* pIn){
		if (pIn) {
			pIn->Release();
			pIn = nullptr;
		}
	}
// end
}
