#pragma once
#include <d3d11.h>
#include <vector>
#include "Base/SSE.hpp"

namespace DigitRenderer{
    class CallbackData_t;
}

class DynamicDigitRenderer_t {
public:
    // yeah, should enough.
    static const int MAX_CONCURRENT_DRAWS = 64;

    // constant buffer data structure
    struct alignas(16) DigitConstants_t {
        float ScreenSize[2];
        float OutPos[2];
        float Color[4];
    };
private:
    // DX11
    ID3D11Buffer* constant_buffer[MAX_CONCURRENT_DRAWS];
    DigitRenderer::CallbackData_t* pCallbackData[MAX_CONCURRENT_DRAWS];

    ID3D11PixelShader* pixel_shader = nullptr;
    ID3D11VertexShader* vertex_shader = nullptr;
    ID3D11InputLayout* input_layout = nullptr;
    ID3D11ShaderResourceView* digit_texture_srv = nullptr;
    ID3D11SamplerState* point_sampler = nullptr;

    // set texture info
    static constexpr int CHARS_PER_ROW = 11;
    static constexpr int TOTAL_ROWS = 1; 
    static constexpr float TEXTURE_WIDTH = 512.0f;
    static constexpr float TEXTURE_HEIGHT = 128.0f * TOTAL_ROWS;
    static constexpr float CHAR_WIDTH = 1.0f / CHARS_PER_ROW; 
    static constexpr float CHAR_HEIGHT = 1.0f / TOTAL_ROWS; 

	ID3D11DeviceContext* g_context; // for rendering, set in Render()
	//DigitConstants_t g_constants2; // in b2
    int nextBufferIndex = 0;
public:
    void Initialize();
    void Cleanup();

    ~DynamicDigitRenderer_t() { Cleanup(); }

    void BeginFrame();
    void EndFrame();

    void SetRender(ID3D11DeviceContext* context, DigitConstants_t* pData);
    void SetImageData();
    void SetToShader(int index, const DigitConstants_t* pData);

    void RenderEnd() { nextBufferIndex = 0; }
};
typedef DynamicDigitRenderer_t* PDynamicDigitRenderer;

namespace DigitRenderer {
    class CallbackData_t {
    private:
        PDynamicDigitRenderer pDigitRenderer;
        int bufferIndex;
        DynamicDigitRenderer_t::DigitConstants_t data;

    public:
        void Initialize(PDynamicDigitRenderer p, int index, DynamicDigitRenderer_t::DigitConstants_t* in) {
            pDigitRenderer = p;
            bufferIndex = index;
            memcpy(&data, in, sizeof(DynamicDigitRenderer_t::DigitConstants_t));
        }

        void SetToShader() {
            pDigitRenderer->SetToShader(bufferIndex, &data);
		}
    };
    typedef CallbackData_t* PCallbackData;

    enum DigitRendererChar_ : int {
        DigitRendererChar_0,
        DigitRendererChar_1,
        DigitRendererChar_2,
        DigitRendererChar_3,
        DigitRendererChar_4,
        DigitRendererChar_5,
        DigitRendererChar_6,
        DigitRendererChar_7,
        DigitRendererChar_8,
        DigitRendererChar_9,
        DigitRendererChar_DOT,
        // second line
        DigitRendererChar_PERCENT,
    };

	std::vector<BYTE> StringToDigitRendererChars(const std::string& str);
}
