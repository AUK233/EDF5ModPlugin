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
#include "0SolidColorTexture.hpp"

#include "1DigitRenderer.h"

#define DEBUGMODE


namespace DigitRenderer {

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

	CreateSolidColorTextures(device);

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
	DirectX::CreateDDSTextureFromFile(device, L"./subtitle/DamageUINumber.dds", nullptr, &digit_texture_srv);

	vs_blob->Release();
	vs_blob->Release();
	if (error_blob) error_blob->Release();
#endif
}

void DynamicDigitRenderer_t::CreateSolidColorTextures(ID3D11Device* device)
{
	color_texture_srv[DigitRendererColor_Red] = CreateSolidColorSRV(device, 0xFF0000FF);
	color_texture_srv[DigitRendererColor_RedNA] = CreateSolidColorSRV(device, 0xFF000000);
	color_texture_srv[DigitRendererColor_White] = CreateSolidColorSRV(device, 0xFFFFFFFF);
	color_texture_srv[DigitRendererColor_WhiteNA] = CreateSolidColorSRV(device, 0xFFFFFF00);
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

	for (int i = 0; i < DigitRendererColor_ALL; i++) {
		if (color_texture_srv[i]) {
			color_texture_srv[i]->Release();
			color_texture_srv[i] = nullptr;
		}
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

void DynamicDigitRenderer_t::SetRender(ID3D11DeviceContext* context, PDigitText pText)
{
	if (nextBufferIndex >= MAX_CONCURRENT_DRAWS) return;

	using namespace DigitRenderer;
	auto cbData = pCallbackData[nextBufferIndex];
	if (!cbData) return;

	g_context = context;
	cbData->Initialize(this, nextBufferIndex, &pText->cbData);
	nextBufferIndex++;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	draw_list->ChannelsSetCurrent(nextBufferIndex);

	draw_list->AddCallback([](const ImDrawList* parent, const ImDrawCmd* cmd) {
		auto self = (PCallbackData)cmd->UserCallbackData;
		self->SetToShader();
		//_aligned_free(self);
	}, cbData);
}

void __vectorcall DynamicDigitRenderer_t::SetImageData(const DigitTextByte& pText, PDigitFontControl pFont, __m128 BasePos, int colorTexIndex)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	DigitFontControl_t fontControl;
	memcpy(&fontControl, pFont, sizeof(DigitFontControl_t));

	__m128 v_pos = BasePos;
	ImVec2 pos[2];
	_mm_storeu_ps(&pos[0].x, v_pos);

	int textSize = pText.size();
	for (int i = 0; i < textSize; i++) {
		auto renderIndex = pText[i];
		fontControl.renderIndex = renderIndex;
		fontControl.charIndex = i;
		auto colData = GetImageCharData(&fontControl);

		draw_list->AddImage(color_texture_srv[colorTexIndex], pos[0], pos[1], ImVec2(0, 0), ImVec2(1, 1), colData);

		if (renderIndex == DigitRendererChar_DOT) {
			float dotOffset = fontControl.fontSize * -0.5;
			__m128 v_subDot = _mm_load_ss(&dotOffset);
			v_subDot = _mm_shuffle_ps(v_subDot, v_subDot, MY_SHUFFLE(0, 3, 0, 3));

			v_pos = _mm_add_ps(v_pos, v_subDot);
			_mm_storeu_ps(&pos[0].x, v_pos);
		}
	}


	/*ImVec2 p_min(0, 0);
	ImVec2 p_max(FontSize, FontSize);

	fontControl.renderIndex = 2;
	fontControl.charIndex = 0;
	ImU32 colData = GetImageCharData(&fontControl);
	draw_list->AddImage(digit_texture_srv, p_min, p_max, ImVec2(0, 0), ImVec2(1, 1), colData);

	fontControl.renderIndex = DigitRendererChar_DOT;
	fontControl.charIndex = 1;
	colData = GetImageCharData(&fontControl);
	p_min.y += 1;
	p_min.y += 1;
	draw_list->AddImage(digit_texture_srv, p_min, p_max, ImVec2(0, 0), ImVec2(1, 1), colData);

	fontControl.renderIndex = 4;
	fontControl.charIndex = 2;
	colData = GetImageCharData(&fontControl);
	p_min.y += 1;
	p_min.y += 1;
	draw_list->AddImage(digit_texture_srv, p_min, p_max, ImVec2(0, 0), ImVec2(1, 1), colData);*/
}

ImU32 DynamicDigitRenderer_t::GetImageCharData(PDigitFontControl pData)
{
	union {
		// 0 as char index
		struct {
			ImU8 effectTime; // 255 is enough
			ImU8 charIndex; // 1-7bit is char index, 8bit is fade enable
			ImU8 renderIndex; // DigitRendererChar_
			ImU8 alpha; // always 1, because it can not be modified
		};
		ImU32 encoded;
	} charData;

	charData.effectTime = pData->effectTime;
	charData.charIndex = (pData->charIndex & 0x7F) | (pData->fadeEnable ? 0x80 : 0);
	charData.renderIndex = pData->renderIndex;
	charData.alpha = 1; 

	return charData.encoded;
}

void DynamicDigitRenderer_t::SetToShader(int index, const DigitConstants_t* pData)
{
	g_context->VSSetShader(vertex_shader, nullptr, 0);
	g_context->PSSetShader(pixel_shader, nullptr, 0);
	g_context->IASetInputLayout(input_layout);

	g_context->UpdateSubresource(constant_buffer[index], 0, nullptr, pData, 0, 0);
	g_context->VSSetConstantBuffers(2, 1, &constant_buffer[index]);
	g_context->PSSetConstantBuffers(2, 1, &constant_buffer[index]);

	g_context->PSSetShaderResources(1, 1, &digit_texture_srv); // to t1
	g_context->PSSetSamplers(0, 1, &point_sampler);
}
// end
}
