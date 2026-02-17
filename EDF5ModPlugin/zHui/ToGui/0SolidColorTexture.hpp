#pragma once
#include <d3d11.h>

namespace DigitRenderer {
	ID3D11Texture2D* CreateSolidColorTexture(ID3D11Device* device, DWORD color)
	{
		HRESULT hr;

		// 1. create texture description
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = 1;
		texDesc.Height = 1;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		// 2. initialise solid color data
		D3D11_SUBRESOURCE_DATA initData = {};

		// 32bit color: RGBA (0xAARRGGBB)
		UINT32 colorData = color;
		initData.pSysMem = &colorData;
		initData.SysMemPitch = sizeof(UINT32);

		// 3. create texture
		ID3D11Texture2D* texture = nullptr;
		hr = device->CreateTexture2D(&texDesc, &initData, &texture);

		if (FAILED(hr))
			return nullptr;

		return texture;
	}

	// color: RGBA, yeah, because it was reversed when read.
	ID3D11ShaderResourceView* CreateSolidColorSRV(ID3D11Device* device, DWORD color)
	{
		DWORD cur_color = _byteswap_ulong(color);

		ID3D11Texture2D* texture = CreateSolidColorTexture(device, cur_color);
		if (!texture) return nullptr;

		ID3D11ShaderResourceView* srv = nullptr;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		device->CreateShaderResourceView(texture, &srvDesc, &srv);

		texture->Release();
		return srv;
	}
}
