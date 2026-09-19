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

	void DXSharedTexture2D::Release(void* deviceVK){
		if (!deviceVK) {
			D3D_Release();
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
