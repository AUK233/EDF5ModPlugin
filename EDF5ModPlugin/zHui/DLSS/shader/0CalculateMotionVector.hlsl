// ===================================================
// Constant!
struct Light_t
{
	float3 m_vector;
	float m_pad0;
	float3 m_color;
	float m_pad1;
	float3 m_specular_color;
	float m_pad2;
};

struct xgl_system_t {
	row_major float4x4 g_xgl_view;
	row_major float4x4 g_xgl_view_inverse;
	row_major float4x4 g_xgl_projection;
	row_major float4x4 g_xgl_view_projection;
	row_major float4x4 g_xgl_view_projection_inverse;
	uint2 g_xgl_target_dimension;
	float g_xgl_time;
	int g_xgl_id;
	
	Light_t g_xgl_light[4];
	
	float3 g_xgl_ambient_color;
	float g_pad3;
	float g_xgl_fog_mul;
	float g_xgl_fog_add;
	float g_xgl_fade_mul;
	float g_xgl_fade_add;
	float4 g_xgl_fog_color;
};

cbuffer xgl_system : register(b0)
{
	xgl_system_t currentSys;
}

cbuffer prev_system : register(b2)
{
	xgl_system_t previousSys;
}

// ===================================================
// in c++
Texture2D<float4> g_viewXYZ_id : register(t0);
RWTexture2D<float2> OutputMV : register(u0);

// ===================================================
[numthreads(16, 16, 1)]
void CS_main(uint3 threadID : SV_DispatchThreadID)
{
	uint2 pixel = threadID.xy;
	float4 viewPos = float4(g_viewXYZ_id[pixel].xyz, 1);
	
	if (abs(viewPos.z) >= 60000.0) {
		OutputMV[pixel] = 0;
		return;
	}

	float4 worldPos;
	worldPos.x = dot(viewPos, currentSys.g_xgl_view_inverse[0]);
	worldPos.y = dot(viewPos, currentSys.g_xgl_view_inverse[1]);
	worldPos.z = dot(viewPos, currentSys.g_xgl_view_inverse[2]);
	worldPos.w = 1;

	float4 ClipPos;
	ClipPos.x = dot(worldPos, currentSys.g_xgl_view_projection[0]);
	ClipPos.y = dot(worldPos, currentSys.g_xgl_view_projection[1]);
	ClipPos.z = dot(worldPos, currentSys.g_xgl_view_projection[2]);
	ClipPos.w = dot(worldPos, currentSys.g_xgl_view_projection[3]);

	if (ClipPos.w < 0.0001f) {
		OutputMV[pixel] = 0;
		return;
	}
	ClipPos.xyz /= ClipPos.w;

	float4 prevClipPos;
	prevClipPos.x = dot(worldPos, previousSys.g_xgl_view_projection[0]);
	prevClipPos.y = dot(worldPos, previousSys.g_xgl_view_projection[1]);
	prevClipPos.z = dot(worldPos, previousSys.g_xgl_view_projection[2]);
	prevClipPos.w = dot(worldPos, previousSys.g_xgl_view_projection[3]);
	prevClipPos.xyz /= prevClipPos.w;

	float2 ndc = ClipPos.xy - prevClipPos.xy;
	
	float2 ScreenSize = currentSys.g_xgl_target_dimension;
	float2 screen_pos;
	screen_pos.x = (ScreenSize.x * 0.5 * ndc.x);
	screen_pos.y = -(ScreenSize.y * 0.5 * ndc.y);

	OutputMV[pixel] = screen_pos;
}
