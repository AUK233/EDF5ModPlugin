#pragma once
#include "0SL_common.h"
#include "Base/g_DXresource.h"
#include "Base/g_system.h"

namespace D3D{
	typedef struct AddPostProcessRes_t {
		UINT Width, Height;
	} *PAddPostProcessRes;

	class AddPostProcess_t {
	public:
		AddPostProcessRes_t m_resolution;
		DXSharedTexture2D m_ColorRes[2]; ID3D11UnorderedAccessView* m_ColorUAV[2];

		ID3D11ComputeShader* m_PostProcessCS; ID3D11SamplerState* m_LUTSamplerLinear;
		ID3D11ShaderResourceView* m_LookupTable_SRV;
		ID3D11ShaderResourceView* m_LookupTable_SRV_Backup; // temporarily reserve the LUT when switching

		// DLSS
		DXSharedTexture2D m_MotionVector; ID3D11UnorderedAccessView* m_MotionVectorUAV;
		ID3D11Buffer* CB_Previous_xgl_system; // this is used to calculate the motion vector
		ID3D11ComputeShader* m_MotionVectorCS;
		ID3D11ComputeShader* m_ToFGBufferCS; // FG
		// DLSS end

		// DX12 or VK
		DXSharedTexture2D m_DepthRes[2]; ID3D11UnorderedAccessView* m_DepthUAV[2];
		DXSharedTexture2D m_MidColorRes[2]; ID3D11ShaderResourceView* m_MidColorSRV[2]; // Used to link DX11 and DX12/VK resources
		DXSharedTexture2D m_FGColorRes[2]; ID3D11UnorderedAccessView* m_FGColorUAV[2]; // 0 is output interp color, 1 is color without HUD 
		// DX12 or VK end

		int m_LUTindex, m_playerCount;

		void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

		void Buffer_Create(ID3D11Device* device, const AddPostProcessRes_t& resolution, ID3D12Device* device12, UINT DLSS_Level);
		void Buffer_VKCreate(PslVulkanAPI deviceVK);
		void Buffer_Release(PslVulkanAPI deviceVK);

		void Execute(Pg_D3D11DeviceInfo pD3D, int playerIndex, const AddPostProcessRes_t& ThreadGroupCount, ID3D11ShaderResourceView** ppColor, ID3D11ShaderResourceView** ppDepth);
		void CalculateMV(Pg_D3D11DeviceInfo pD3D, PXGS_System_Player player, const AddPostProcessRes_t& ThreadGroupCount, bool clearMV);
		void CopyBuffer(Pg_D3D11DeviceInfo pD3D, const AddPostProcessRes_t& ThreadGroupCount, Pg_D3D_ResourceInfo p1color, Pg_D3D_ResourceInfo p2color);

		void LUTBuffer_Load(ID3D11Device* device);
		void LUTBuffer_Reload(ID3D11Device* device);
		ID3D11ShaderResourceView* LUTBuffer_Create(ID3D11Device* device, int index);
		std::wstring GetLUTFilePath(int index);
	};
	typedef AddPostProcess_t* PAddPostProcess;
}
