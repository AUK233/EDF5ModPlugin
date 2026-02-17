#include <windows.h>
#include <DirectXMath.h>
#include <string>
#include <cmath>
#include <d3dcompiler.h>

#include "DirectXTK/DDSTextureLoader.h"
#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include "0GetDXGI.h"

#include "1DigitRenderer.h"

#define DEBUGMODE

void DynamicDigitRenderer_t::Initialize()
{
	auto pRender = DXGI_GetGameDXGIRender();
	auto device = pRender->pD3D11Device;

	// initialize data
	g_context = pRender->pD3D11DeviceContext;
	// end

	// create point sampler
	D3D11_SAMPLER_DESC samp_desc = {};
	samp_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samp_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samp_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samp_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samp_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samp_desc.MinLOD = 0;
	samp_desc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device->CreateSamplerState(&samp_desc, &point_sampler);
	//if (FAILED(hr)) return;

	// create constant buffer
	D3D11_BUFFER_DESC cb_desc = {};
	cb_desc.Usage = D3D11_USAGE_DEFAULT;
	cb_desc.ByteWidth = sizeof(DigitConstants_t);
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.CPUAccessFlags = 0;

	using namespace DigitRenderer;
	for (int i = 0; i < MAX_CONCURRENT_DRAWS; i++) {
		hr = device->CreateBuffer(&cb_desc, nullptr, &constant_buffer[i]);

		auto p = (PCallbackData)_aligned_malloc(sizeof(CallbackData_t), 0x10);
		pCallbackData[i] = p;
	}

	// load shader resource
#if defined(DEBUGMODE)
	ID3DBlob* vs_blob = nullptr;
	ID3DBlob* ps_blob = nullptr;
	ID3DBlob* error_blob = nullptr;

	// compile shader
	D3DCompileFromFile(L"./subtitle/shader.hlsl", nullptr, nullptr, "VS_main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &vs_blob, &error_blob);
	D3DCompileFromFile(L"./subtitle/shader.hlsl", nullptr, nullptr, "PS_main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &ps_blob, &error_blob);

	device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &vertex_shader);
	device->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, &pixel_shader);

	// create input layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(ImDrawVert, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(ImDrawVert, uv), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, offsetof(ImDrawVert, col), D3D11_INPUT_PER_VERTEX_DATA, 0}, // or DXGI_FORMAT_R8G8B8A8_UNORM
	};
	device->CreateInputLayout(layout, 3, vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &input_layout);

	// load texture
	DirectX::CreateDDSTextureFromFile(device, L"./subtitle/DamageNumber.dds", nullptr, &digit_texture_srv);

	vs_blob->Release();
	vs_blob->Release();
	if (error_blob) error_blob->Release();
#endif
}

void DynamicDigitRenderer_t::Cleanup()
{
	if (point_sampler) {
		point_sampler->Release();
		point_sampler = nullptr;
	}

	for (int i = 0; i < MAX_CONCURRENT_DRAWS; i++) {
		if (constant_buffer[i]) {
			constant_buffer[i]->Release();
			constant_buffer[i] = nullptr;
		}
	}


	if (vertex_shader) {
		vertex_shader->Release();
		vertex_shader = nullptr;
	}

	if (pixel_shader) {
		pixel_shader->Release();
		pixel_shader = nullptr;
	}

	if (input_layout) {
		input_layout->Release();
		input_layout = nullptr;
	}

	if (digit_texture_srv) {
		digit_texture_srv->Release();
		digit_texture_srv = nullptr;
	}
}

void DynamicDigitRenderer_t::BeginFrame() {
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->ChannelsSplit(MAX_CONCURRENT_DRAWS);
	nextBufferIndex = 0;
}

void DynamicDigitRenderer_t::EndFrame() {
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->ChannelsMerge();
	drawList->AddCallback(ImDrawCallback_ResetRenderState, nullptr);
}

void DynamicDigitRenderer_t::SetRender(ID3D11DeviceContext* context, DigitConstants_t* pData)
{
	if (nextBufferIndex >= MAX_CONCURRENT_DRAWS) return;

	using namespace DigitRenderer;
	auto cbData = pCallbackData[nextBufferIndex];
	if (!cbData) return;

	g_context = context;
	cbData->Initialize(this, nextBufferIndex, pData);
	nextBufferIndex++;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	draw_list->ChannelsSetCurrent(nextBufferIndex);

	draw_list->AddCallback([](const ImDrawList* parent, const ImDrawCmd* cmd) {
		auto self = (PCallbackData)cmd->UserCallbackData;
		self->SetToShader();
		//_aligned_free(self);
	}, cbData);
}

void DynamicDigitRenderer_t::SetImageData()
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	union {
		// 0 as char index
		struct{
			ImU8 scaleTime; // 255 is enough
			ImU8 fadeTime; // maybe only use to 16
			ImU8 charIndex; // DigitRendererChar_
			ImU8 totalNum; // is input data size, 255 is enough, should!
		};
		ImU32 encoded;
	} charData;

	charData.scaleTime = 0;
	charData.fadeTime = 0;
	charData.charIndex = 2;
	charData.totalNum = 1;

	ImVec2 p_min(0, 0);
	ImVec2 p_max(64, 64);
	draw_list->AddImage(digit_texture_srv, p_min, p_max, ImVec2(0, 0), ImVec2(1, 1), charData.encoded);
}

void DynamicDigitRenderer_t::SetToShader(int index, const DigitConstants_t* pData)
{
	g_context->VSSetShader(vertex_shader, nullptr, 0);
	g_context->PSSetShader(pixel_shader, nullptr, 0);
	g_context->IASetInputLayout(input_layout);

	g_context->UpdateSubresource(constant_buffer[index], 0, nullptr, pData, 0, 0);
	g_context->VSSetConstantBuffers(2, 1, &constant_buffer[index]);
	g_context->PSSetConstantBuffers(2, 1, &constant_buffer[index]);

	g_context->PSSetShaderResources(0, 1, &digit_texture_srv);
	g_context->PSSetSamplers(0, 1, &point_sampler);
}

namespace DigitRenderer {
	std::vector<BYTE> StringToDigitRendererChars(const std::string& str)
	{
		std::vector<BYTE> out;
		out.reserve(str.size());
		for (char c : str) {
			if (c == '.') {
				out.push_back(DigitRendererChar_DOT);
				continue;
			}

			BYTE number = c - '0';
			// 0x3A ':' as '%'
			if (number > DigitRendererChar_PERCENT) number = 0;
		}
		return out;
	}
}
