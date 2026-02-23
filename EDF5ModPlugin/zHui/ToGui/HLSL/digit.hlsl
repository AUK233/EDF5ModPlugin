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

SamplerState sampler_point : register(s0);
Texture2D color_texture : register(t0); // from AddImage
Texture2D digit_texture : register(t1);

struct VS_INPUT
{
	float2 pos : POSITION0;
	float2 uv  : TEXCOORD0;
	// x is char index, y is char total
	// z is scale factor, w is fade factor
	uint4 col : COLOR0;
	// z is render index, w is font size
	uint4 tex1 : TEXCOORD1;
	// x is time (second)
	// y is char align factor: 0 is left, 0.5 is center, 1 is right
	float2 tex2 : TEXCOORD2;
	float4 color1 : POSITION1; // as an input position, or border color.
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 char_data : COLOR0; // x is scale time, y is fade time, z is index width, w is index height
};

// ===================================================
#ifdef _DynamicPos
	#pragma message("DynamicPos is defined!")
	float2 WorldToScreen(float4 world_pos) {
		float3 relative_pos = world_pos.xyz;

		// do not modify it
		row_major float4x4 viewProjection;
		viewProjection[0] = c_xgl_view[0] * c_xgl_projection[0].x;
		viewProjection[1] = c_xgl_view[1] * c_xgl_projection[1].y;
		viewProjection[2] = c_xgl_view[2] * c_xgl_projection[2].z;
		viewProjection[2].w += c_xgl_projection[2].w;
		viewProjection[3] = c_xgl_view[2] * c_xgl_projection[2].z;
		// otherwise it will cause problems.

		float4 clip_pos;
		clip_pos.x = dot(relative_pos.xyz, viewProjection[0].xyz) + viewProjection[0].w;
		clip_pos.y = dot(relative_pos.xyz, viewProjection[1].xyz) + viewProjection[1].w;
		clip_pos.z = dot(relative_pos.xyz, viewProjection[2].xyz) + viewProjection[2].w;
		clip_pos.w = dot(relative_pos.xyz, viewProjection[3].xyz) + viewProjection[3].w;

		if (clip_pos.w < 0.1f) return -c_DefaultResolution;
		float3 ndc = clip_pos.xyz / clip_pos.w;
		
		float2 screen_pos;
		screen_pos.x = -(ScreenSize.x * 0.5 * ndc.x) + (ScreenSize.x * 0.5);
		screen_pos.y = -(ScreenSize.y * 0.5 * ndc.y) + (ScreenSize.y * 0.5);
		return screen_pos;
	}
#else
	#pragma message("NOT defined _DynamicPos")
#endif

// ===================================================
// vertex shader
PS_INPUT VS_main(VS_INPUT input)
{
	PS_INPUT output;
	
	output.uv.xy = input.uv;
#ifdef _DynamicPos
	float2 inpos = WorldToScreen(input.color1);
#else
	float2 inpos = input.color1.xy;
#endif
	//inpos += float2(400, 200);
	
	// get fade enable
	uint2 effect_factor = input.col.zw;
	float2 effect_time = input.tex2.x * effect_factor;
	output.char_data.xy = effect_time;
	// end
	
	// calculate render index
	uint digit = input.tex1.z;
	uint2 cur_digit;
	cur_digit.x = digit % c_ColumnCount;
	cur_digit.y = digit / c_ColumnCount;
	
	float2 grid_pos = cur_digit;
	output.char_data.zw = grid_pos;
	// end
	float widthFactor = cur_digit.x >= (c_ColumnCount - 1) ? 0.5 : c_FontWidth;
	
	// get char index
	float char_index = input.col.x;
	float char_total = input.col.y;

	// calculate offset position
	float char_align = input.tex2.y * char_total;
	float char_offset = char_index - char_align;
	float scale = ScaleSpeed * effect_time.x + 1;

	float fontSize = input.tex1.w;
	char_offset *= fontSize * c_FontWidth * scale; 

	float2 out_pos = float2(char_offset, 0);

	// get scale
	float2 input_pos = input.pos + inpos;
	if (input.uv.x > 0) input_pos.x -= fontSize * c_FontWidthDelta;
	float2 originalSize = float2(fontSize * widthFactor, fontSize);
	float2 center = input_pos - (input.uv - 0.5) * originalSize;
	float2 newPos2 = center + (input.uv - 0.5) * originalSize * scale;
	out_pos += newPos2;
	// end

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
	//return float4(1, 0, 0, 1);

	float2 tex_uv = input.uv.xy;
	tex_uv.x = (input.char_data.z + input.uv.x) * c_FontWidthFactor;
	tex_uv.y = (input.char_data.w + input.uv.y) * c_LineFactor;

	// a is alpha
	float4 tex_color = color_texture.Sample(sampler_point, tex_uv);
	// r is border, g is main, a is alpha
	float4 tex_digit = digit_texture.Sample(sampler_point, tex_uv);

	// calculate border color
	float3 out_borderColor = tex_digit.r * BorderColor.rgb;

	float3 out_color = tex_digit.g * tex_color.rgb;
	out_color += out_borderColor;

	// calculate fade
	float fade = tex_digit.a * tex_color.a;
	fade += FadeSpeed * input.char_data.y;
	fade = max(fade, 0);

	return float4(out_color, fade);
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
