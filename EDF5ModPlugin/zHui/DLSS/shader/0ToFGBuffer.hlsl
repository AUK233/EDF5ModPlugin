// ===================================================
// in c++
Texture2D<float4> InputColor : register(t0);

RWTexture2D<float4> OutputHUDless : register(u0);
RWTexture2D<float4> OutputColor : register(u1);

// ===================================================
[numthreads(16, 16, 1)]
void CS_main(uint3 threadID : SV_DispatchThreadID)
{
	uint2 pixel = threadID.xy;

	float4 inputColor = InputColor[pixel];

	OutputHUDless[pixel] = inputColor;
	OutputColor[pixel] = inputColor;
}
