#pragma once
#include <d3d11.h>

namespace D3D {
	class D3DPostProcess_t {
	public:
		ID3D11Device* Device;
		ID3D11DeviceContext* Context;

		ID3D11Texture2D* OutColor[2];
		ID3D11UnorderedAccessView* OutUAV[2];

		//ID3D11Texture2D* LinearDepth[2];
		//ID3D11UnorderedAccessView* LinearDepthUAV[2];

		ID3D11SamplerState* LUTSamplerLinear;
		ID3D11ShaderResourceView* LookupTable_SRV;
		ID3D11ComputeShader* PostProcessCS;

		ID3D11Texture2D* BlackMV;
		ID3D11UnorderedAccessView* MotionVectorUAV;
		ID3D11Buffer* PreviousCB_xgl_system;
		ID3D11ComputeShader* MotionVectorCS;

		int PlayerCount;

		void Initialize(ID3D11Device* device, ID3D11DeviceContext* context, DXGI_SWAP_CHAIN_DESC* pChainDesc);
		void LoadComputeShader();
		void ReleaseBuffer();
		void SetBuffer(UINT Width, UINT Height);
		void LoadLUTBuffer();
	};

	typedef D3DPostProcess_t* PD3DPostProcess;
}