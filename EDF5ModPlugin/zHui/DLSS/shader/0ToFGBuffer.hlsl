// ===================================================
// in c++
Texture2D<float4> InputColor : register(t0);
Texture2D<float> DepthMap : register(t1);

RWTexture2D<float4> OutputColor : register(u0);
RWTexture2D<float> OutputDepth : register(u1);

// ===================================================
[numthreads(16, 16, 1)]
void CS_main(uint3 threadID : SV_DispatchThreadID)
{
	uint2 pixel = threadID.xy;

	OutputColor[pixel] = InputColor[pixel];
	OutputDepth[pixel] = DepthMap[pixel];
}
