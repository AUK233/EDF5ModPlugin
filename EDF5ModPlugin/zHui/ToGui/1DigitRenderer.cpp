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
#include "utiliy.h"

#include "1DigitRenderer.h"

//#define DEBUGMODE

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

	// create constant buffer 0
	D3D11_BUFFER_DESC cb_desc0 = {};
	cb_desc0.Usage = D3D11_USAGE_DEFAULT;
	cb_desc0.ByteWidth = sizeof(xgl_system_CB_t);
	cb_desc0.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc0.CPUAccessFlags = 0;
	device->CreateBuffer(&cb_desc0, nullptr, &constant_buffer0);

	// create constant buffer 1
	D3D11_BUFFER_DESC cb_desc1 = {};
	cb_desc1.Usage = D3D11_USAGE_DEFAULT;
	cb_desc1.ByteWidth = sizeof(xgl_transform_customize_t);
	cb_desc1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc1.CPUAccessFlags = 0;
	device->CreateBuffer(&cb_desc1, nullptr, &constant_buffer1);

	// create constant buffer 2
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
	// end

	CreateSolidColorTextures(device);
	ZeroMemory(&g_constants0, sizeof(xgl_system_CB_t));
	ZeroMemory(&g_constants1, sizeof(xgl_transform_customize_t));

	// init input layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(ImDrawVert, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(ImDrawVert, uv), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, offsetof(ImDrawVert, col_u), D3D11_INPUT_PER_VERTEX_DATA, 0}, // or DXGI_FORMAT_R8G8B8A8_UNORM
		{"TEXCOORD", 1, DXGI_FORMAT_R8G8B8A8_UINT, 0, offsetof(ImDrawVert, tex1), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(ImDrawVert, tex2), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"POSITION", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(ImDrawVert, pos1), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// load shader resource
#ifdef DEBUGMODE
	ID3DBlob* vs_blob = nullptr;
	ID3DBlob* ps_blob = nullptr;
	ID3DBlob* vs_blob2 = nullptr;
	ID3DBlob* ps_blob2 = nullptr;
	ID3DBlob* error_blob = nullptr;

	const D3D_SHADER_MACRO defines[] =
	{
		"_DynamicPos", "1",
		{nullptr, nullptr}
	};

	// compile shader
	D3DCompileFromFile(L"./subtitle/digit.hlsl", nullptr, nullptr, "VS_main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &vs_blob, &error_blob);
	D3DCompileFromFile(L"./subtitle/digit.hlsl", nullptr, nullptr, "PS_main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &ps_blob, &error_blob);
	device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &vertex_shader[0]);
	device->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, &pixel_shader[0]);
	//
	D3DCompileFromFile(L"./subtitle/digit.hlsl", defines, nullptr, "VS_main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &vs_blob2, &error_blob);
	D3DCompileFromFile(L"./subtitle/digit.hlsl", defines, nullptr, "PS_main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &ps_blob2, &error_blob);
	device->CreateVertexShader(vs_blob2->GetBufferPointer(), vs_blob2->GetBufferSize(), nullptr, &vertex_shader[1]);
	device->CreatePixelShader(ps_blob2->GetBufferPointer(), ps_blob2->GetBufferSize(), nullptr, &pixel_shader[1]);


	// create input layout
	device->CreateInputLayout(layout, _countof(layout), vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &input_layout[0]);
	device->CreateInputLayout(layout, _countof(layout), vs_blob2->GetBufferPointer(), vs_blob2->GetBufferSize(), &input_layout[1]);

	// load texture
	DirectX::CreateDDSTextureFromFile(device, L"./subtitle/DamageUINumber.dds", nullptr, &digit_texture_srv);

	vs_blob->Release();
	ps_blob->Release();
	vs_blob2->Release();
	ps_blob2->Release();
	if (error_blob) error_blob->Release();
#else
	//LoadEmbeddedResource(v_data_digit_texture, L"Resource\\DamageUINumber.dds", L"Texture"); // no, this cannot be found.
	LoadEmbeddedResource(v_data_digit_texture, MAKEINTRESOURCEW(IDR_DamageUINumber), L"Texture");
	LoadEmbeddedResource(v_data_shader_vs[0], MAKEINTRESOURCEW(IDR_vs_digitFixed), L"Shader");
	LoadEmbeddedResource(v_data_shader_ps[0], MAKEINTRESOURCEW(IDR_ps_digitFixed), L"Shader");
	LoadEmbeddedResource(v_data_shader_vs[1], MAKEINTRESOURCEW(IDR_vs_digitDynamic), L"Shader");
	LoadEmbeddedResource(v_data_shader_ps[1], MAKEINTRESOURCEW(IDR_ps_digitDynamic), L"Shader");


	for (int i = 0; i < DigitRendererShader_ALL; i++) {
		// load shader
		device->CreatePixelShader(v_data_shader_ps[i].data(), v_data_shader_ps[i].size(), nullptr, &pixel_shader[i]);
		device->CreateVertexShader(v_data_shader_vs[i].data(), v_data_shader_vs[i].size(), nullptr, &vertex_shader[i]);

		// create input layout
		device->CreateInputLayout(layout, _countof(layout), v_data_shader_vs[i].data(), v_data_shader_vs[i].size(), &input_layout[i]);
	}
	// load texture
	DirectX::CreateDDSTextureFromMemory(device, v_data_digit_texture.data(), v_data_digit_texture.size(), nullptr, &digit_texture_srv);
#endif
}

void DynamicDigitRenderer_t::CreateSolidColorTextures(ID3D11Device* device)
{
	color_texture_srv[DigitRendererColor_Red] = CreateSolidColorSRV(device, 0xFF0000FF);
	color_texture_srv[DigitRendererColor_Green] = CreateSolidColorSRV(device, 0x00FF00FF);
	color_texture_srv[DigitRendererColor_Blue] = CreateSolidColorSRV(device, 0x0000FFFF);
	color_texture_srv[DigitRendererColor_White] = CreateSolidColorSRV(device, 0xFFFFFFFF);
}

void DynamicDigitRenderer_t::ReloadDynamicPosShader()
{
#ifdef DEBUGMODE
	auto pRender = DXGI_GetGameDXGIRender();
	auto device = pRender->pD3D11Device;

	ID3DBlob* vs_blob = nullptr;
	ID3DBlob* error_blob = nullptr;
	const D3D_SHADER_MACRO defines[] =
	{
		"_DynamicPos", "1",
		{nullptr, nullptr}
	};
	auto hr = D3DCompileFromFile(L"./subtitle/digit.hlsl", defines, nullptr, "VS_main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &vs_blob, &error_blob);
	if (FAILED(hr)) {
		MessageBoxW(NULL, L"Failed to compile vertex shader", L"error", MB_OK);
		return;
	} else {
		MessageBoxW(NULL, L"compile vertex shader", L"done", MB_OK);
	}

	ID3D11VertexShader* newVS = nullptr;
	device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &newVS);

	ID3D11InputLayout* newLayout = nullptr;
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(ImDrawVert, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(ImDrawVert, uv), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, offsetof(ImDrawVert, col_u), D3D11_INPUT_PER_VERTEX_DATA, 0}, // or DXGI_FORMAT_R8G8B8A8_UNORM
		{"TEXCOORD", 1, DXGI_FORMAT_R8G8B8A8_UINT, 0, offsetof(ImDrawVert, tex1), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(ImDrawVert, tex2), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"POSITION", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(ImDrawVert, pos1), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	device->CreateInputLayout(layout, _countof(layout), vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &newLayout);

	if (vertex_shader[1]) vertex_shader[1]->Release();
	if (input_layout[1]) input_layout[1]->Release();

	vertex_shader[1] = newVS;
	input_layout[1] = newLayout;

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

	for (int i = 0; i < DigitRendererShader_ALL; i++) {
		if (vertex_shader[i]) {
			vertex_shader[i]->Release();
			vertex_shader[i] = nullptr;
		}

		if (pixel_shader[i]) {
			pixel_shader[i]->Release();
			pixel_shader[i] = nullptr;
		}

		if (input_layout[i]) {
			input_layout[i]->Release();
			input_layout[i] = nullptr;
		}
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

void DynamicDigitRenderer_t::SetRender(ID3D11DeviceContext* context, const PDigitConstants pData, int shader_index)
{
	if (nextBufferIndex >= MAX_CONCURRENT_DRAWS) return;

	using namespace DigitRenderer;
	auto cbData = pCallbackData[nextBufferIndex];
	if (!cbData) return;

	g_context = context;
	cbData->Initialize(this, shader_index, nextBufferIndex, pData);
	nextBufferIndex++;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->ChannelsSetCurrent(nextBufferIndex);

	draw_list->AddCallback([](const ImDrawList* parent, const ImDrawCmd* cmd) {
		auto self = (PCallbackData)cmd->UserCallbackData;
		self->SetToShader();
		//_aligned_free(self);
	}, cbData);
}

void __vectorcall DynamicDigitRenderer_t::SetImageDataInFixedPos(const DigitTextByte& pText, __m128 BasePos, PDigitFontControl pFont, int colorTexIndex)
{
	int textSize = pText.size();
	if (!textSize) return;

	__m128 v_pos = BasePos;
	DigitFontControl_t fontControl;
	memcpy(&fontControl, pFont, sizeof(DigitFontControl_t));
	fontControl.charTotal = textSize;

	// draw
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImTextureRef tex_ref = color_texture_srv[colorTexIndex];
	draw_list->PushTexture(tex_ref);
	draw_list->PrimReserve(6 * textSize, 4 * textSize);

	for (int i = 0; i < textSize; i++) {
		auto renderIndex = pText[i];
		fontControl.renderIndex = renderIndex;
		fontControl.charIndex = i;

		AddImageData(draw_list, &fontControl, v_pos);

		if (renderIndex == DigitRendererChar_DOT) {
			float dotOffset = fontControl.f_fontSize * -0.4;
			__m128 v_subDot = _mm_load_ss(&dotOffset);
			v_subDot = _mm_shuffle_ps(v_subDot, v_subDot, MY_SHUFFLE(0, 3, 0, 3));

			v_pos = _mm_add_ps(v_pos, v_subDot);
		}
		// end
	}

	draw_list->PopTexture();
}

void __vectorcall DynamicDigitRenderer_t::SetImageDataInDynamicPos(const DigitTextByte& pText, __m128 BasePos, PDigitFontControl pFont, int colorTexIndex)
{
	int textSize = pText.size();
	if (!textSize) return;

	__m128 v_pos = BasePos;
	DigitFontControl_t fontControl;
	memcpy(&fontControl, pFont, sizeof(DigitFontControl_t));
	fontControl.charTotal = textSize;

	// draw
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImTextureRef tex_ref = color_texture_srv[colorTexIndex];
	draw_list->PushTexture(tex_ref);
	draw_list->PrimReserve(6 * textSize, 4 * textSize);

	for (int i = 0; i < textSize; i++) {
		auto renderIndex = pText[i];
		fontControl.renderIndex = renderIndex;
		fontControl.charIndex = i;

		AddImageData(draw_list, &fontControl, v_pos);
		// end
	}

	draw_list->PopTexture();
}

void __vectorcall DynamicDigitRenderer_t::AddImageData(ImDrawList* draw_list, PDigitFontControl pFont, __m128 inputPos) {
	// calculate vertex data
	ImDrawIdx idx = (ImDrawIdx)draw_list->_VtxCurrentIdx;
	draw_list->_IdxWritePtr[0] = idx;
	draw_list->_IdxWritePtr[1] = (ImDrawIdx)(idx + 1);
	draw_list->_IdxWritePtr[2] = (ImDrawIdx)(idx + 2);
	draw_list->_IdxWritePtr[3] = idx;
	draw_list->_IdxWritePtr[4] = (ImDrawIdx)(idx + 2);
	draw_list->_IdxWritePtr[5] = (ImDrawIdx)(idx + 3);

	float fontSize = pFont->f_fontSize;
	// set vertex data
	struct {
		ImDrawVertCol_t u4;
		ImDrawVertTex1_t tex1;
		ImDrawVertTex2_t tex2;
	} inData;
	static_assert(sizeof(inData) == 16);
	inData.u4.charInfo[0] = pFont->charIndex;
	inData.u4.charInfo[1] = pFont->charTotal;
	inData.u4.fadeFactor = pFont->fadeFactor;
	inData.u4.scaleFactor = pFont->scaleFactor;
	inData.tex1.renderIndex = pFont->renderIndex;
	inData.tex1.fontSize = pFont->i_fontSize;
	inData.tex2.time = pFont->time;
	inData.tex2.charAlignFactor = pFont->charAlignFactor;
	// p0
	auto temp = &draw_list->_VtxWritePtr[0];
	ZeroMemory(&temp->pos.x, 16);
	memcpy(&temp->col, &inData, 16);
	_mm_storeu_ps(temp->pos1, inputPos);
	// p1
	temp = &draw_list->_VtxWritePtr[1];
	temp->pos.x = fontSize;
	temp->pos.y = 0;
	temp->uv.x = 1;
	temp->uv.y = 0;
	memcpy(&temp->col, &inData, 16);
	_mm_storeu_ps(temp->pos1, inputPos);
	// p2
	temp = &draw_list->_VtxWritePtr[2];
	temp->pos.x = fontSize;
	temp->pos.y = fontSize;
	temp->uv.x = 1;
	temp->uv.y = 1;
	memcpy(&temp->col, &inData, 16);
	_mm_storeu_ps(temp->pos1, inputPos);
	// p3
	temp = &draw_list->_VtxWritePtr[3];
	temp->pos.x = 0;
	temp->pos.y = fontSize;
	temp->uv.x = 0;
	temp->uv.y = 1;
	memcpy(&temp->col, &inData, 16);
	_mm_storeu_ps(temp->pos1, inputPos);
	// end
	draw_list->_VtxWritePtr += 4;
	draw_list->_VtxCurrentIdx += 4;
	draw_list->_IdxWritePtr += 6;
}

void DynamicDigitRenderer_t::SetToShader(int shader_index, int cb_index, const PDigitConstants pData)
{
	g_context->VSSetShader(vertex_shader[shader_index], nullptr, 0);
	g_context->PSSetShader(pixel_shader[shader_index], nullptr, 0);
	g_context->IASetInputLayout(input_layout[shader_index]);

	//g_context->UpdateSubresource(constant_buffer0, 0, nullptr, &g_constants0, 0, 0);
	g_context->UpdateSubresource(constant_buffer1, 0, nullptr, &g_constants1, 0, 0);
	g_context->UpdateSubresource(constant_buffer[cb_index], 0, nullptr, pData, 0, 0);

	//g_context->VSSetConstantBuffers(0, 1, &constant_buffer0);
	g_context->VSSetConstantBuffers(1, 1, &constant_buffer1);
	g_context->VSSetConstantBuffers(2, 1, &constant_buffer[cb_index]);
	// ok ps no cb1
	//g_context->PSSetConstantBuffers(0, 1, &constant_buffer0);
	g_context->PSSetConstantBuffers(2, 1, &constant_buffer[cb_index]);


	g_context->PSSetShaderResources(1, 1, &digit_texture_srv); // to t1
	g_context->PSSetSamplers(0, 1, &point_sampler);
}

// end
}
