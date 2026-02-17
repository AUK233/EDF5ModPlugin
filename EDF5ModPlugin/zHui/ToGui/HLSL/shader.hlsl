// b2 is safe
cbuffer xgl_user_param : register(b2)
{
    float2 ScreenSize;
    float2 OutPos;
    float4 Color;
};

SamplerState sampler_point : register(s0);
Texture2D digit_texture : register(t0);

struct VS_INPUT
{
    float2 pos : POSITION;
    float2 uv  : TEXCOORD0;
    uint4 col : COLOR; // x is char index, y is index in data
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
    uint2 char_data : COLOR0;
};

// ===================================================
// vertex shader
PS_INPUT VS_main(VS_INPUT input)
{
    PS_INPUT output;

    float2 out_pos = input.pos + OutPos;

    // get projection matrix
    output.pos.x = (out_pos.x / ScreenSize.x) * 2.0 - 1.0;
    output.pos.y = -((out_pos.y / ScreenSize.y) * 2.0 - 1.0);
    output.pos.z = 0.0;
    output.pos.w = 1.0;

    output.uv = input.uv;
    output.char_data = input.col.xy;
    return output;
}

// 
float4 PS_main(PS_INPUT input) : SV_Target
{
    float2 tex_uv = input.uv;
    tex_uv.x = (input.char_data.x + input.uv.x) * 0.092;

    float4 tex_color = digit_texture.Sample(sampler_point, tex_uv);

    float3 red_color = tex_color.r * Color.rgb;

    return float4(red_color, tex_color.w);
}

technique11 RenderDigitsWithBorder
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_main()));
        SetPixelShader(CompileShader(ps_5_0, PS_main()));
        
        // 设置Alpha混合模式
        SetBlendState(BS_AlphaBlend, float4(0,0,0,0), 0xFFFFFFFF);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);
    }
}
