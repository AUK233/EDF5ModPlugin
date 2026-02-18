// ===================================================
// Constant!
static const float2 c_DefaultResolution = float2(1920, 1080);
static const float c_FontWidth = float(48.0 / 64.0);
static const float c_FontWidthFactor = float(48.0 / 512.0);
static const float c_LastWidth = float(32.0 / 512.0);
static const float c_LastWidthFactor = 1 - c_LastWidth;
static const float c_LineFactor = float(1.0 / 2.0);
static const uint c_ColumnCount = 11;
// ===================================================

// b2 is safe
cbuffer xgl_user_param : register(b2)
{
	float2 ScreenSize;
	float2 OutPos;
	float4 Color;
	float FontSize;
};

SamplerState sampler_point : register(s0);
Texture2D color_texture : register(t0); // from AddImage
Texture2D digit_texture : register(t1);

struct VS_INPUT
{
	float2 pos : POSITION;
	float2 uv  : TEXCOORD0;
    uint4 col : COLOR; // x is effect time, y is char index (7bit, 8bit is fade enable), z is render index, w is unavailable
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
    float4 uv : TEXCOORD0; // z is width factor, w is reserve
	float4 char_data : COLOR0; // x is scale time, y is fade time, z is index width, w is index height
};

// ===================================================
// vertex shader
PS_INPUT VS_main(VS_INPUT input)
{
	PS_INPUT output;
	
	output.uv.xy = input.uv;
	
	// get char index and fade enable
	uint char_index = input.col.y & 0x7F;
	
    bool fade_enable = (input.col.y & 0x80) != 0;
    float2 effect_time = fade_enable ? float2(0, input.col.x) : float2(input.col.x, 0);
    output.char_data.xy = effect_time;
	// end
	
	// calculate render index
	uint digit = input.col.z;
    uint2 cur_digit;
    cur_digit.x = digit % c_ColumnCount;
    cur_digit.y = digit / c_ColumnCount;
	
    float2 grid_pos = cur_digit * float2(1, c_LineFactor);
    output.char_data.zw = grid_pos;
	
    float widthFactor = cur_digit.x >= (c_ColumnCount - 1) ? c_LastWidthFactor : 0;
	output.uv.z = widthFactor;
	// end
	
	// calculate offset position
    float char_offset = char_index * FontSize * c_FontWidth;
    float2 out_pos = float2(char_offset, 0);
	
	// get output position
    float2 input_pos = input.pos * float2(c_FontWidth, 1.0);
    out_pos += input_pos;

	out_pos *= 1.25;

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
	float2 tex_uv = input.uv.xy;
    tex_uv.x = (input.char_data.z + input.uv.x) * c_FontWidthFactor;
    tex_uv.y = (input.char_data.w + input.uv.y) * c_LineFactor;

	// w as border alpha
	float4 tex_color = color_texture.Sample(sampler_point, tex_uv);
	// r is border, g is main, a is alpha
	float4 tex_digit = digit_texture.Sample(sampler_point, tex_uv);

	float3 out_borderColor = tex_digit.r * tex_color.w;
	float3 out_color = tex_digit.g * tex_color.rgb;
	out_color += out_borderColor;

	return float4(out_color, tex_digit.a);
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
