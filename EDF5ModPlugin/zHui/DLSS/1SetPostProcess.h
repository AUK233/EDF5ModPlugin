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

		ID3D11ComputeShader* PostProcessCS;
		ID3D11SamplerState* LUTSamplerLinear;
		ID3D11ShaderResourceView* LookupTable_SRV;
		ID3D11ShaderResourceView* LookupTable_SRV_Backup; // temporarily reserve the LUT when switching

		ID3D11Texture2D* BlackMV;
		ID3D11UnorderedAccessView* MotionVectorUAV;
		ID3D11Buffer* PreviousCB_xgl_system;
		ID3D11ComputeShader* MotionVectorCS;

		// dlss fg
		ID3D11Texture2D* OutputInterp;
		ID3D11Texture2D* OutputReal;
		ID3D11Texture2D* FGHudLess; ID3D11UnorderedAccessView* FGHudLessUAV;
		ID3D11Texture2D* FGDepth; ID3D11UnorderedAccessView* FGDepthUAV;
		ID3D11ComputeShader* ToFGBufferCS;

		//
		int LUTindex;
		int PlayerCount;

		void Initialize(ID3D11Device* device, ID3D11DeviceContext* context, DXGI_SWAP_CHAIN_DESC* pChainDesc);
		void LoadComputeShader();
		void ReleaseBuffer();
		void SetBuffer(UINT Width, UINT Height, UINT DLSS_Level);
		void LoadLUTBuffer();
		void ReloadLUTBuffer();
		ID3D11ShaderResourceView* CreateLUTBuffer(int index);
		std::wstring GetLUTFilePath(int index);
	};

	typedef D3DPostProcess_t* PD3DPostProcess;
}