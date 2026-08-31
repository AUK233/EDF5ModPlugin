#pragma once
#include <d3d11.h>
#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"
#include "lib/nvsdk_ngx_vk.h"
#include "lib/nvsdk_ngx_helpers_vk.h"
// xess
#include "lib/xess_vk.h"
#include <mutex>
#include <chrono>

//typedef struct VkInstance_T* VkInstance;
//typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
//typedef struct VkDevice_T* VkDevice;
//typedef struct VkQueue_T* VkQueue;
//
//typedef struct VkImageSubresourceRange_T* VkImageSubresourceRange;
//typedef struct VkImageLayout_T* VkImageLayout;

class IDXGIVkInteropDevice;

// from https://github.com/doitsujin/dxvk/blob/70d7508c01201ed3d4bfb33da42ba834eafe3857/src/dxgi/dxgi_interfaces.h#L262
static const GUID IID_IDXGIVkInteropSurface = { 0x5546cf8c, 0x77e7, 0x4341, { 0xb0, 0x5d, 0x8d, 0x4d, 0x50, 0x00, 0xe7, 0x7d } };
class IDXGIVkInteropSurface: public IUnknown{
public:
/**
 * \brief Retrieves device interop interfaceSlots
 *
 * Queries the device that owns the surface for
 * the \ref IDXGIVkInteropDevice interface.
 * \param [out] ppDevice The device interface
 * \returns \c S_OK on success
 */
virtual HRESULT STDMETHODCALLTYPE GetDevice(
		IDXGIVkInteropDevice * *ppDevice) = 0;

/**
 * \brief Retrieves Vulkan image info
 *
 * Retrieves both the image handle as well as the image's
 * properties. Any of the given pointers may be \c nullptr.
 *
 * If \c pInfo is not \c nullptr, the following rules apply:
 * - \c pInfo->sType \e must be \c VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO
 * - \c pInfo->pNext \e must be \c nullptr or point to a supported
 *   extension-specific structure (currently none)
 * - \c pInfo->queueFamilyIndexCount must be the length of the
 *   \c pInfo->pQueueFamilyIndices array, in \c uint32_t units.
 * - \c pInfo->pQueueFamilyIndices must point to a pre-allocated
 *   array of \c uint32_t of size \c pInfo->pQueueFamilyIndices.
 *
 * \note As of now, the sharing mode will always be
 *       \c VK_SHARING_MODE_EXCLUSIVE and no queue
 *       family indices will be written to the array.
 *
 * After the call, the structure pointed to by \c pInfo can
 * be used to create an image with identical properties.
 *
 * If \c pLayout is not \c nullptr, it will receive the
 * layout that the image will be in after flushing any
 * outstanding commands on the device.
 * \param [out] pHandle The image handle
 * \param [out] pLayout Image layout
 * \param [out] pInfo Image properties
 * \returns \c S_OK on success, or \c E_INVALIDARG
 */
virtual HRESULT STDMETHODCALLTYPE GetVulkanImageInfo(
		VkImage* pHandle,
		VkImageLayout* pLayout,
		VkImageCreateInfo* pInfo) = 0;
};

// from https://github.com/doitsujin/dxvk/blob/70d7508c01201ed3d4bfb33da42ba834eafe3857/src/dxgi/dxgi_interfaces.h#L318
// is DXVK 3.1
static const GUID IID_IDXGIVkInteropDevice = { 0xe2ef5fa5, 0xdc21, 0x4af7, { 0x90, 0xc4, 0xf6, 0x7e, 0xf6, 0xa0, 0x93, 0x23 } };
//MIDL_INTERFACE("e2ef5fa5-dc21-4af7-90c4-f67ef6a09323")
class IDXGIVkInteropDevice : public IUnknown{
public:
	/**
	 * \brief Queries Vulkan handles used by DXVK
	 *
	 * \param [out] pInstance The Vulkan instance
	 * \param [out] pPhysDev The physical device
	 * \param [out] pDevide The device handle
	 */
	virtual void STDMETHODCALLTYPE GetVulkanHandles(
		VkInstance* pInstance,
		VkPhysicalDevice* pPhysDev,
		VkDevice* pDevice) = 0;

	/**
	 * \brief Queries the rendering queue used by DXVK
	 *
	 * \param [out] pQueue The Vulkan queue handle
	 * \param [out] pQueueFamilyIndex Queue family index
	 */
	virtual void STDMETHODCALLTYPE GetSubmissionQueue(
			VkQueue* pQueue,
			uint32_t* pQueueFamilyIndex) = 0;

	/**
	 * \brief Transitions a surface to a given layout
	 *
	 * Executes an explicit image layout transition on the
	 * D3D device. Note that the image subresources \e must
	 * be transitioned back to its original layout before
	 * using it again from D3D11.
	 * \param [in] pSurface The image to transform
	 * \param [in] pSubresources Subresources to transform
	 * \param [in] OldLayout Current image layout
	 * \param [in] NewLayout Desired image layout
	 */
	virtual void STDMETHODCALLTYPE TransitionSurfaceLayout(
			IDXGIVkInteropSurface* pSurface,
	  const VkImageSubresourceRange* pSubresources,
			VkImageLayout             OldLayout,
			VkImageLayout             NewLayout) = 0;

	/**
	 * \brief Flushes outstanding D3D rendering commands
	 *
	 * Must be called before submitting Vulkan commands
	 * to the rendering queue if those commands use the
	 * backing resource of a D3D11 object.
	 */
	virtual void STDMETHODCALLTYPE FlushRenderingCommands() = 0;

	/**
	 * \brief Locks submission queue
	 *
	 * Should be called immediately before submitting
	 * Vulkan commands to the rendering queue in order
	 * to prevent DXVK from using the queue.
	 *
	 * While the submission queue is locked, no D3D11
	 * methods must be called from the locking thread,
	 * or otherwise a deadlock might occur.
	 */
	virtual void STDMETHODCALLTYPE LockSubmissionQueue() = 0;

	/**
	 * \brief Releases submission queue
	 *
	 * Should be called immediately after submitting
	 * Vulkan commands to the rendering queue in order
	 * to allow DXVK to submit new commands.
	 */
	virtual void STDMETHODCALLTYPE ReleaseSubmissionQueue() = 0;
};


// vulkan function pointer
HMODULE vulkanModule = nullptr;

PFN_vkCreateCommandPool vkCreateCommandPool = nullptr;
PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers = nullptr;
PFN_vkFreeCommandBuffers vkFreeCommandBuffers = nullptr;
PFN_vkDestroyCommandPool vkDestroyCommandPool = nullptr;
PFN_vkBeginCommandBuffer vkBeginCommandBuffer = nullptr;
PFN_vkEndCommandBuffer vkEndCommandBuffer = nullptr;
PFN_vkResetCommandBuffer vkResetCommandBuffer = nullptr;

PFN_vkQueueSubmit vkQueueSubmit = nullptr;
PFN_vkCreateFence vkCreateFence = nullptr;
PFN_vkDestroyFence vkDestroyFence = nullptr;
PFN_vkWaitForFences vkWaitForFences = nullptr;
PFN_vkResetFences vkResetFences = nullptr;

PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements = nullptr;
PFN_vkCreateImageView vkCreateImageView = nullptr;
PFN_vkDestroyImageView vkDestroyImageView = nullptr;

bool LoadVulkanLibrary() {
	vulkanModule = LoadLibraryA("vulkan-1.dll");
	if (!vulkanModule) {
		return false;
	}

	vkCreateCommandPool = (PFN_vkCreateCommandPool)GetProcAddress(vulkanModule, "vkCreateCommandPool");
	vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)GetProcAddress(vulkanModule, "vkAllocateCommandBuffers");
	vkFreeCommandBuffers = (PFN_vkFreeCommandBuffers)GetProcAddress(vulkanModule, "vkFreeCommandBuffers");
	vkDestroyCommandPool = (PFN_vkDestroyCommandPool)GetProcAddress(vulkanModule, "vkDestroyCommandPool");
	vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer)GetProcAddress(vulkanModule, "vkBeginCommandBuffer");
	vkEndCommandBuffer = (PFN_vkEndCommandBuffer)GetProcAddress(vulkanModule, "vkEndCommandBuffer");
	vkResetCommandBuffer = (PFN_vkResetCommandBuffer)GetProcAddress(vulkanModule, "vkResetCommandBuffer");

	vkQueueSubmit = (PFN_vkQueueSubmit)GetProcAddress(vulkanModule, "vkQueueSubmit");
	vkCreateFence = (PFN_vkCreateFence)GetProcAddress(vulkanModule, "vkCreateFence");
	vkDestroyFence = (PFN_vkDestroyFence)GetProcAddress(vulkanModule, "vkDestroyFence");
	vkWaitForFences = (PFN_vkWaitForFences)GetProcAddress(vulkanModule, "vkWaitForFences");
	vkResetFences = (PFN_vkResetFences)GetProcAddress(vulkanModule, "vkResetFences");

	vkGetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements)GetProcAddress(vulkanModule, "vkGetImageMemoryRequirements");
	vkCreateImageView = (PFN_vkCreateImageView)GetProcAddress(vulkanModule, "vkCreateImageView");
	vkDestroyImageView = (PFN_vkDestroyImageView)GetProcAddress(vulkanModule, "vkDestroyImageView");


	FreeLibrary(vulkanModule);
	return true;
}

bool NVSDK_NGX_Resource_VK_Get(VkDevice device, ID3D11Texture2D* pTexture, NVSDK_NGX_Resource_VK* pResource) {
	IDXGIVkInteropSurface* pVkSurface = nullptr;
	HRESULT hr = pTexture->QueryInterface(IID_IDXGIVkInteropSurface, (void**)&pVkSurface);
	if (!pVkSurface) return false;

	VkImage vkImage = VK_NULL_HANDLE;
	VkImageLayout vkLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	VkImageCreateInfo vkInfo{};
	vkInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	hr = pVkSurface->GetVulkanImageInfo(&vkImage, &vkLayout, &vkInfo);
	pVkSurface->Release();
	if (FAILED(hr)) return false;
	/*
	pResource->Resource.BufferInfo.Buffer = (VkBuffer)pTexture;
	pResource->Resource.BufferInfo.SizeInBytes = 1920 * 1080 * 4;
	pResource->Type = NVSDK_NGX_RESOURCE_VK_TYPE_VK_BUFFER;
	pResource->ReadWrite = true;
	return true;*/

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
		/*
		if (vkInfo.format == VK_FORMAT_D24_UNORM_S8_UINT || vkInfo.format == VK_FORMAT_D32_SFLOAT_S8_UINT) {
			viewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}*/
	} else {
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = vkInfo.mipLevels;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = vkInfo.arrayLayers;
	VkImageView vkImageView;
	VkResult result = vkCreateImageView(device, &viewInfo, nullptr, &vkImageView);
	if (result != VK_SUCCESS) return false;
	

	pResource->Resource.ImageViewInfo.ImageView = vkImageView;
	pResource->Resource.ImageViewInfo.Image = vkImage;
	pResource->Resource.ImageViewInfo.SubresourceRange = viewInfo.subresourceRange;
	pResource->Resource.ImageViewInfo.Format = vkInfo.format;
	pResource->Resource.ImageViewInfo.Width = vkInfo.extent.width;
	pResource->Resource.ImageViewInfo.Height = vkInfo.extent.height;
	pResource->Type = NVSDK_NGX_RESOURCE_VK_TYPE_VK_IMAGEVIEW;
	pResource->ReadWrite = true;

	/*
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, vkImage, &memRequirements);
	pResource->Resource.BufferInfo.Buffer = (VkBuffer)vkImage;
	pResource->Resource.BufferInfo.SizeInBytes = memRequirements.size;
	pResource->Type = NVSDK_NGX_RESOURCE_VK_TYPE_VK_BUFFER;
	pResource->ReadWrite = true;
	*/

	return true;
}

void NVSDK_NGX_Resource_VK_Destroy(VkDevice device, NVSDK_NGX_Resource_VK* pResource){
	auto vkImageView = pResource->Resource.ImageViewInfo.ImageView;
	if (vkImageView) {
		vkDestroyImageView(device, vkImageView, nullptr);
		pResource->Resource.ImageViewInfo.ImageView = VK_NULL_HANDLE;
	}
	// end
}

typedef struct DLSS_VKResource_t{
	ID3D11Texture2D* DXBuffer;
	NVSDK_NGX_Resource_VK vkResource;
}*PDLSS_VKResource;

void DLSS_VKResource_Set(VkDevice device, ID3D11Texture2D* pTexture, PDLSS_VKResource pVKR) {
	if (pVKR->DXBuffer == pTexture) return;

	NVSDK_NGX_Resource_VK_Destroy(device, &pVKR->vkResource);

	pVKR->DXBuffer = pTexture;
	NVSDK_NGX_Resource_VK_Get(device, pTexture, &pVKR->vkResource);
}

static std::ofstream g_XeSSLogFile;
static std::mutex g_XeSSLogMutex;

void XeSSLoggingCallback(const char* message, xess_logging_level_t loggingLevel) {
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);

	std::lock_guard<std::mutex> lock(g_XeSSLogMutex);

	g_XeSSLogFile.open("Z:/TEMP/xess_debug.log", std::ios::out | std::ios::app);

	g_XeSSLogFile << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
		<< " [" << loggingLevel << "] "
		<< (message ? message : "(null message)")
		<< std::endl;

	g_XeSSLogFile.flush();
	g_XeSSLogFile.close();

	/*static int count = 0;
	if (!count) {
		MessageBoxW(NULL, L"Done!", L"info", MB_OK);
		count++;
	}*/
}
