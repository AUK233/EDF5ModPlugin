// ===================================================
// Constant!
static const float2 c_DefaultResolution = float2(1920, 1080);
static const float c_FontWidth = float(48.0 / 64.0);
static const float c_FontWidthDelta = float((64.0 - 48.0) / 64.0);
static const float c_FontWidthFactor = float(48.0 / 512.0);
static const float c_LastWidth = float(32.0 / 512.0);
static const float c_LastWidthFactor = 1 - c_LastWidth;
static const float c_LineFactor = float(1.0 / 2.0);
static const uint c_ColumnCount = 11;
// ===================================================

// Game's CB0, incomplete at present
// cbuffer xgl_system : register(b0)
// {
// 	row_major float4x4 g_xgl_view;
// 	row_major float4x4 g_xgl_view_inverse;
// 	row_major float4x4 g_xgl_projection;
// 	row_major float4x4 g_xgl_view_projection;
// 	row_major float4x4 g_xgl_view_projection_inverse;
// };

// use our own version
cbuffer xgl_transform : register(b1)
{
	row_major float4x4 c_xgl_view;
	row_major float4x4 c_xgl_projection;
}

// b2 is safe
cbuffer xgl_user_param : register(b2)
{
	float2 ScreenSize;
	float2 ScreenScale;
	float ScaleSpeed;
	float FadeSpeed;
	float2 pad18;
	float4 BorderColor;
};

SamplerState g_Sampler : register(s0);
Texture2D g_FontTexture : register(t0); // from AddImage
Texture2D digit_texture : register(t1);

struct VS_INPUT
{
	float2 pos : POSITION0;
	float2 uv  : TEXCOORD0;
	// x is char index, y is char total
	// z is scale factor, w is fade factor
	float4 col : COLOR0;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

// ===================================================
// vertex shader
PS_INPUT VS_main(VS_INPUT input)
{
	PS_INPUT output;
	
	output.uv.xy = input.uv;
	output.color = input.col;
	float2 out_pos = input.pos;

	// get projection matrix
	output.pos.x = (out_pos.x / ScreenSize.x) * 2.0 - 1.0;
	output.pos.y = -((out_pos.y / ScreenSize.y) * 2.0 - 1.0);
	output.pos.z = 0.0;
	output.pos.w = 1.0;
	// end

	return output;
}

// ===================================================
// pixel shader
float4 PS_main(PS_INPUT input) : SV_Target
{
	float4 texColor = g_FontTexture.Sample(g_Sampler, input.uv);
	return input.color * texColor;
}

technique11 RenderDigitsWithBorder
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_main()));
		SetPixelShader(CompileShader(ps_5_0, PS_main()));
		
		// Set Alpha Blend Mode
		SetBlendState(BS_AlphaBlend, float4(0,0,0,0), 0xFFFFFFFF);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);
	}
}
