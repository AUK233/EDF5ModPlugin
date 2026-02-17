// shader.hlsl - 支持双行纹理布局和黑边Alpha输出
// 纹理布局：512x128，两行
// 第一行(0-64px): 索引0-10 (0-9和.)
// 第二行(64-128px): 索引11 (%)

cbuffer DigitConstants : register(b0)
{
    float4 canvas_size;      // x=宽度, y=高度, z=1/宽度, w=1/高度
    float4 digit_params;     // x=字符宽度(归一化), y=字符高度(归一化), z=字符间距, w=行数
    float4 time_params;      // x=时间, y=增量, z=缩放系数, w=脉动频率
    float4 color_params;     // x=黑边强度, y=黑边阈值, z=辉光强度, w=保留
    float4 border_color;     // RGB=黑边颜色, A=黑边透明度
};

// 纹理采样器 - 点采样保持清晰边缘
SamplerState sampler_point : register(s0);
Texture2D digit_texture : register(t0);

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
    float4 col : COLOR0;
    float4 char_data : TEXCOORD1;  // x=字符索引, y=起始X位置, z=缩放因子, w=行号
    float4 screen_pos : TEXCOORD2; // xy=原始屏幕位置, zw=保留
};

// 顶点着色器
PS_INPUT VS(float3 pos : POSITION, float2 uv : TEXCOORD0, float4 col : COLOR0)
{
    PS_INPUT output;
    output.pos = float4(pos, 1.0f);
    output.uv = uv;
    output.col = col;
    output.screen_pos = float4(pos.x, pos.y, 0, 0);
    
    // 从颜色alpha解码字符数据
    // 使用两个字节编码: 高位字节=字符索引, 低位字节=缩放种子
    uint encoded = (uint)(col.w * 65535.0f);
    uint char_index = (encoded >> 8) & 0xFF;
    uint scale_seed = encoded & 0xFF;
    
    // 计算行号：0-10在第一行，11在第二行
    uint row = (char_index == 11) ? 1 : 0;
    
    output.char_data = float4(
        (float)char_index,
        pos.x,
        (float)scale_seed,
        (float)row
    );
    
    return output;
}

// 带黑边的Alpha混合输出
float4 PS(PS_INPUT input) : SV_Target
{
    // 解码字符信息
    int char_index = (int)input.char_data.x;
    float base_x = input.char_data.y;
    float scale_seed = input.char_data.z;
    int row = (int)input.char_data.w;
    
    // 动态缩放计算
    float time = time_params.x;
    float delta = time_params.y;
    float scale_factor = time_params.z;
    float frequency = time_params.w;
    
    // 每个字符独立的脉动效果
    float phase = scale_seed * 0.1f + base_x * 0.005f + time * frequency;
    float pulse = (sin(phase) * 0.5f + 0.5f) * 0.3f + 0.7f; // 0.7-1.0范围
    float scale = pulse * (1.0f + delta * 0.2f) * scale_factor;
    
    // 计算纹理UV - 双行布局
    float char_width = digit_params.x;      // 1/11 ≈ 0.0909 (第一行11个字符)
    float char_height = digit_params.y;     // 1/2 = 0.5
    float cols_first_row = 11.0f;           // 第一行11个字符
    
    float2 tex_uv = input.uv;
    
    // 根据行号计算UV
    if (row == 0) {
        // 第一行: 0-10 (0-9和.)
        tex_uv.x = (char_index + input.uv.x) * char_width;
        tex_uv.y = input.uv.y * char_height;
    } else {
        // 第二行: 索引11 (百分号)
        tex_uv.x = (0 + input.uv.x) * char_width;  // 第一列
        tex_uv.y = 0.5f + input.uv.y * char_height; // 第二行
    }
    
    // 采样数字纹理
    float4 tex_color = digit_texture.Sample(sampler_point, tex_uv);
    
    // 透明度裁剪
    float alpha = tex_color.a;
    clip(alpha - 0.05f);
    
    // 屏幕位置动态计算
    float2 screen_pos = input.screen_pos.xy;
    float2 center_offset = float2(
        0.5f * char_width * canvas_size.x * scale,
        0.5f * char_height * canvas_size.y * scale
    );
    
    // === 黑边效果处理 ===
    float black_edge_strength = color_params.x;
    float black_edge_threshold = color_params.y;
    float glow_strength = color_params.z;
    float4 black_color = border_color;
    
    // 方法1: 基于距离场的黑边
    float2 edge_distance = min(input.uv.x, 1.0f - input.uv.x);
    edge_distance = min(edge_distance, min(input.uv.y, 1.0f - input.uv.y));
    float edge_factor = 1.0f - smoothstep(0.0f, black_edge_threshold, edge_distance);
    
    // 方法2: 基于纹理alpha梯度的黑边
    float2 dx = float2(char_width / canvas_size.x, 0);
    float2 dy = float2(0, char_height / canvas_size.y);
    float alpha_grad_x = abs(tex_color.a - digit_texture.Sample(sampler_point, tex_uv + dx).a);
    float alpha_grad_y = abs(tex_color.a - digit_texture.Sample(sampler_point, tex_uv + dy).a);
    float gradient = max(alpha_grad_x, alpha_grad_y);
    float edge_mask = smoothstep(0.1f, 0.3f, gradient);
    
    // 合并黑边效果
    float black_mask = max(edge_factor * 0.8f, edge_mask * 0.5f);
    black_mask = saturate(black_mask * black_edge_strength);
    
    // 内部辉光效果
    float glow = 1.0f - edge_distance * 2.0f;
    glow = saturate(glow * glow_strength * pulse);
    
    // === 颜色合成 ===
    float3 base_color = input.col.rgb;
    float3 final_rgb;
    float final_alpha;
    
    // 模式1: 黑边 + 内部颜色 (复古数码管风格)
    if (black_mask > 0.01f) {
        // 边缘使用黑边颜色
        final_rgb = lerp(base_color, black_color.rgb, black_mask * black_color.a);
        final_alpha = alpha * input.col.a;
    } else {
        // 内部使用原始颜色 + 辉光
        final_rgb = base_color * (1.0f + glow * 0.3f);
        final_alpha = alpha * input.col.a;
    }
    
    // 模式2: 纯黑边风格 (完全黑色边缘)
    // 取消注释以启用
    /*
    if (edge_distance < black_edge_threshold) {
        final_rgb = black_color.rgb;
        final_alpha = alpha * input.col.a * black_color.a;
    } else {
        final_rgb = base_color;
        final_alpha = alpha * input.col.a;
    }
    */
    
    // 模式3: 发光边缘风格
    // 取消注释以启用
    /*
    float glow_intensity = pow(1.0f - edge_distance, 2.0f) * 0.8f;
    final_rgb = base_color + float3(0.3f, 0.3f, 0.5f) * glow_intensity;
    final_alpha = alpha * input.col.a;
    */
    
    return float4(final_rgb, final_alpha);
}

technique11 RenderDigitsWithBorder
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
        
        // 设置Alpha混合模式
        SetBlendState(BS_AlphaBlend, float4(0,0,0,0), 0xFFFFFFFF);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);
    }
}