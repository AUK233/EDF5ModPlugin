#pragma once
#include <d3d11.h>
#include <vector>
#include "0DigitCommon.h"

namespace DigitRenderer{
    class CallbackData_t;

    // load which shader
    enum DigitRendererShader_ : int {
        DigitRendererShader_Fixed, // screen position is input by us.
        DigitRendererShader_Dynamic, // input unit position.
        DigitRendererShader_ALL,
    };

    class DynamicDigitRenderer_t {
    public:
        // used to configure our solid-colour textures
        enum DigitRendererColor_ : int {
            DigitRendererColor_Red, // 255, 0, 0, 255
			DigitRendererColor_Green, // 0, 255, 0, 255
			DigitRendererColor_Blue, // 0, 0, 255, 255
			DigitRendererColor_White, // 255, 255, 255, 255
            DigitRendererColor_ALL,
        };
        // yeah, should enough.
        static const int MAX_CONCURRENT_DRAWS = 64;
    private:
	    ID3D11DeviceContext* g_context; // for rendering, set in Render()
        DigitTextByte v_data_digit_texture;
        DigitTextByte v_data_shader_vs[DigitRendererShader_ALL];
        DigitTextByte v_data_shader_ps[DigitRendererShader_ALL];
        // DX11
        ID3D11Buffer* constant_buffer[MAX_CONCURRENT_DRAWS];
        DigitRenderer::CallbackData_t* pCallbackData[MAX_CONCURRENT_DRAWS];

        ID3D11PixelShader* pixel_shader[DigitRendererShader_ALL];
        ID3D11VertexShader* vertex_shader[DigitRendererShader_ALL];
        ID3D11InputLayout* input_layout[DigitRendererShader_ALL];
        ID3D11ShaderResourceView* digit_texture_srv = nullptr;
        ID3D11SamplerState* point_sampler = nullptr;

        ID3D11ShaderResourceView* color_texture_srv[DigitRendererColor_ALL];

	    //DigitConstants_t g_constants2; // in b2
        int nextBufferIndex = 0;
    public:
        void Initialize();
        void CreateSolidColorTextures(ID3D11Device* device);
        void Cleanup();

        ~DynamicDigitRenderer_t() { Cleanup(); }

        void BeginFrame();
        void EndFrame();

        void SetRender(ID3D11DeviceContext* context, const PDigitConstants pData, int shader_index);
        void __vectorcall SetImageDataInFixedPos(const DigitTextByte& pText, __m128 BasePos, PDigitFontControl pFont, int colorTexIndex);
        void __vectorcall AddImageData(ImDrawList* draw_list, PDigitFontControl pFont, __m128 inputPos);

        void SetToShader(int shader_index, int cb_index, const PDigitConstants pData);

        void RenderEnd() { nextBufferIndex = 0; }
    };
    typedef DynamicDigitRenderer_t* PDynamicDigitRenderer;

    class CallbackData_t {
    private:
        PDynamicDigitRenderer pDigitRenderer;
        int shaderIndex, bufferIndex;
        DigitConstants_t data;
    public:
        void Initialize(PDynamicDigitRenderer p, int shader_index, int cb_index, const PDigitConstants in) {
            pDigitRenderer = p;
            shaderIndex = shader_index;
            bufferIndex = cb_index;
            memcpy(&data, in, sizeof(DigitConstants_t));
        }

        void SetToShader() { pDigitRenderer->SetToShader(shaderIndex, bufferIndex, &data); }
    };
    typedef CallbackData_t* PCallbackData;
// end
}
