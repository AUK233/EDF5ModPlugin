#pragma once
#include "sllib/sl.h"

namespace D3D{
	typedef struct StreamlineAPI_t
	{
		HMODULE hInterposer = nullptr;

		sl::Result(*fnInit)(const sl::Preferences& pref, uint64_t sdkVersion) = nullptr;
		sl::Result(*fnShutdown)() = nullptr;
		sl::Result(*fnIsFeatureSupported)(sl::Feature feature,
			const sl::AdapterInfo& adapterInfo) = nullptr;
		sl::Result(*fnIsFeatureLoaded)(sl::Feature feature, bool& loaded) = nullptr;
		sl::Result(*fnSetFeatureLoaded)(sl::Feature feature, bool loaded) = nullptr;
		sl::Result(*fnEvaluateFeature)(sl::Feature feature,
			const sl::FrameToken& frame,
			const sl::BaseStructure** inputs,
			uint32_t numInputs,
			sl::CommandBuffer* cmdBuffer) = nullptr;
		sl::Result(*fnAllocateResources)(sl::CommandBuffer* cmdBuffer,
			sl::Feature feature,
			const sl::ViewportHandle& viewport) = nullptr;
		sl::Result(*fnFreeResources)(sl::Feature feature,
			const sl::ViewportHandle& viewport) = nullptr;
		sl::Result(*fnSetTagForFrame)(const sl::FrameToken& frame,
			const sl::ViewportHandle& viewport,
			const sl::ResourceTag* tags,
			uint32_t numTags,
			sl::CommandBuffer* cmdBuffer) = nullptr;
		sl::Result(*fnGetFeatureRequirements)(sl::Feature feature,
			sl::FeatureRequirements& requirements) = nullptr;
		sl::Result(*fnGetFeatureVersion)(sl::Feature feature,
			sl::FeatureVersion& version) = nullptr;
		sl::Result(*fnUpgradeInterface)(void** baseInterface) = nullptr;
		sl::Result(*fnSetConstants)(const sl::Constants& values,
			const sl::FrameToken& frame,
			const sl::ViewportHandle& viewport) = nullptr;
		sl::Result(*fnGetNativeInterface)(void* proxyInterface,
			void** baseInterface) = nullptr;
		sl::Result(*fnGetFeatureFunction)(sl::Feature feature,
			const char* functionName,
			void*& function) = nullptr;
		sl::Result(*fnGetNewFrameToken)(sl::FrameToken*& token,
			const uint32_t* frameIndex) = nullptr;
		sl::Result(*fnSetD3DDevice)(void* d3dDevice) = nullptr;

		sl::ViewportHandle myViewport[2] = { {0}, {1} };
		std::wstring DirStr;

		explicit StreamlineAPI_t(LPCWSTR dllPath) {
			hInterposer = ::LoadLibraryW(dllPath);
			if (!hInterposer) {
				return;
			}

	#define SL_GET(exportName, member)                                          \
				member = reinterpret_cast<decltype(member)>(                            \
					::GetProcAddress(hInterposer, #exportName));                        \
				if (!member) { return; }

			SL_GET(slInit, fnInit);
			SL_GET(slShutdown, fnShutdown);
			SL_GET(slIsFeatureSupported, fnIsFeatureSupported);
			SL_GET(slIsFeatureLoaded, fnIsFeatureLoaded);
			SL_GET(slSetFeatureLoaded, fnSetFeatureLoaded);
			SL_GET(slEvaluateFeature, fnEvaluateFeature);
			SL_GET(slAllocateResources, fnAllocateResources);
			SL_GET(slFreeResources, fnFreeResources);
			SL_GET(slSetTagForFrame, fnSetTagForFrame);
			SL_GET(slGetFeatureRequirements, fnGetFeatureRequirements);
			SL_GET(slGetFeatureVersion, fnGetFeatureVersion);
			SL_GET(slUpgradeInterface, fnUpgradeInterface);
			SL_GET(slSetConstants, fnSetConstants);
			SL_GET(slGetNativeInterface, fnGetNativeInterface);
			SL_GET(slGetFeatureFunction, fnGetFeatureFunction);
			SL_GET(slGetNewFrameToken, fnGetNewFrameToken);
			SL_GET(slSetD3DDevice, fnSetD3DDevice);

	#undef SL_GET
		}

		~StreamlineAPI_t() {
			if (hInterposer)
			{
				::FreeLibrary(hInterposer);
				hInterposer = nullptr;
			}
		}

		bool IsValid() const
		{
			return hInterposer != nullptr && fnInit != nullptr;
		}
	} *PStreamlineAPI;
}