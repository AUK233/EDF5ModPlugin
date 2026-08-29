// ===================================================================
// Constant Buffers
// ===================================================================

cbuffer xgl_system : register(b0)
{
	row_major float4x4 g_xgl_view;                    // Offset: 0    Size: 64  [unused]
	row_major float4x4 g_xgl_view_inverse;            // Offset: 64   Size: 64
	row_major float4x4 g_xgl_projection;              // Offset: 128  Size: 64  [unused]
	row_major float4x4 g_xgl_view_projection;         // Offset: 192  Size: 64  [unused]
	row_major float4x4 g_xgl_view_projection_inverse; // Offset: 256  Size: 64  [unused]
	uint2 g_xgl_target_dimension;           // Offset: 320  Size: 8   [unused]
	float g_xgl_time;                       // Offset: 328  Size: 4   [unused]
	int g_xgl_id;                           // Offset: 332  Size: 4   [unused]
	
	struct Light
	{
		float3 m_vector;          // Offset: 336
		float m_pad0;             // Offset: 348
		float3 m_color;           // Offset: 352
		float m_pad1;             // Offset: 364
		float3 m_specular_color;  // Offset: 368
		float m_pad2;             // Offset: 380
	} g_xgl_light[4];             // Offset: 336  Size: 192
	
	float3 g_xgl_ambient_color;   // Offset: 528  Size: 12
	float g_pad3;                 // Offset: 540  Size: 4   [unused]
	float g_xgl_fog_mul;          // Offset: 544  Size: 4
	float g_xgl_fog_add;          // Offset: 548  Size: 4
	float g_xgl_fade_mul;         // Offset: 552  Size: 4   [unused]
	float g_xgl_fade_add;         // Offset: 556  Size: 4   [unused]
	float4 g_xgl_fog_color;       // Offset: 560  Size: 16
};

cbuffer xgl_additional_light_setting : register(b2)
{
	struct ShadowSetting
	{
		row_major float4x4 m_vp_to_view;      // Offset: 0
		row_major float4x4 m_proj;            // Offset: 64
		float m_distance;           // Offset: 128
		float m_shadow_adjust;      // Offset: 132
		float m_shadow_smooth;      // Offset: 136
		float m_pad;                // Offset: 140
	} g_shadow_setting[2];          // Offset: 0    Size: 288
	
	float g_shadow_far_fade;        // Offset: 288  Size: 4
	float g_shadow_far_fade_mul;    // Offset: 292  Size: 4
	float2 g_light_setting_pad;     // Offset: 296  Size: 8   [unused]
	uint g_light_count;             // Offset: 304  Size: 4
	uint g_point_light_count;       // Offset: 308  Size: 4
	float g_bg_fade_near;           // Offset: 312  Size: 4
	float g_bg_fade_distance;       // Offset: 316  Size: 4
	float4 g_bg_color;              // Offset: 320  Size: 16
	row_major float4x4 g_light_view_proj;     // Offset: 336  Size: 64
};

cbuffer LocalCubemapWorldAABB : register(b10)
{
	float3 g_min_point;                     // Offset: 0    Size: 12
	uint g_debug_is_used_local_cubemap;     // Offset: 12   Size: 4
	float3 g_unit_size;                     // Offset: 16   Size: 12
	uint g_debug_preview_env;               // Offset: 28   Size: 4
	uint3 g_cubemap_grid_divide_count;      // Offset: 32   Size: 12
};

// ===================================================================
// Textures
// ===================================================================

Texture2D<float3> g_albedoRGB : register(t0);
Texture2D<float3> g_specularRGB : register(t1);
Texture2D<float3> g_normalXYZ : register(t2);
Texture2D<float3> g_smooth_reflection_specpow : register(t3);
Texture2D<float2> g_metallic_roughness : register(t4);
Texture2D<float4> g_viewXYZ_id : register(t5);
Texture2D<float3> g_light_accumulate : register(t6);
Texture2D<float3> g_specular_accumulate : register(t7);
TextureCubeArray<float4> g_local_cubemap_array : register(t8);
Texture2D<float2> g_ambient_occlusion : register(t9);

// ===================================================================
// Structured Buffers
// ===================================================================

struct LocalEnvGrid {
	uint m_cubemap_index[8];
	uint2 m_coner_index;
	uint m_count;
	float pad;
};
StructuredBuffer<LocalEnvGrid> g_local_cubemap_grid : register(t10);

struct PointLight {
	float3 m_position;
	float m_radius;
	float3 m_color;
	float m_inv_radius;
	float3 m_vector;
	uint m_type;
	float m_min_z;
	float m_max_z;
	float m_smooth;
	float m_pad;
};
StructuredBuffer<PointLight> g_xgl_compute_point_light_array : register(t11);

struct PointLightMask {
	uint m_light_mask[64];
	bool m_is_anything_passed;
};
StructuredBuffer<PointLightMask> g_point_light_mask_x : register(t12);
StructuredBuffer<PointLightMask> g_point_light_mask_y : register(t13);
StructuredBuffer<PointLightMask> g_point_light_mask_z : register(t14);
#ifdef _HasShadow
Texture2DArray<float> g_shadow_depth : register(t15);
#endif

SamplerState g_xgl_compute_environment_map_sampler : register(s10);
SamplerState g_ambient_occlusion_sampler : register(s11);

RWTexture2D<float4> g_color_out : register(u0);

//===========================================================
#ifdef _HasShadow
float CalculateShadow(float4 viewPos) {
	if (abs(viewPos.z) >= g_shadow_setting[1].m_distance) {
		return 1.0;
	}

	int shadowIndex = abs(viewPos.z) < g_shadow_setting[0].m_distance ? 0 : 1;

	float4 shadowPosView;
	shadowPosView.x = dot(viewPos, g_shadow_setting[shadowIndex].m_vp_to_view[0]);
	shadowPosView.y = dot(viewPos, g_shadow_setting[shadowIndex].m_vp_to_view[1]);

	float shadowViewZ = dot(viewPos, g_shadow_setting[shadowIndex].m_vp_to_view[2]);
	shadowPosView.z = shadowViewZ + g_shadow_setting[shadowIndex].m_shadow_adjust;
	shadowPosView.w = 1;

	float3 shadowPosProj;
	shadowPosProj.x = dot(shadowPosView, g_shadow_setting[shadowIndex].m_proj[0]);
	shadowPosProj.y = dot(shadowPosView, g_shadow_setting[shadowIndex].m_proj[1]);
	shadowPosProj.z = dot(shadowPosView, g_shadow_setting[shadowIndex].m_proj[2]);

	float4 shadowPosViewDepthLower = shadowPosView;
	shadowPosViewDepthLower.z = shadowViewZ - g_shadow_setting[shadowIndex].m_shadow_smooth;
	float receiverDepth = dot(shadowPosViewDepthLower, g_shadow_setting[shadowIndex].m_proj[2]) - shadowPosProj.z;

	uint2 shadowDim;
	uint shadowArg3;
	g_shadow_depth.GetDimensions(shadowDim.x, shadowDim.y, shadowArg3);

	float2 shadowUV = shadowPosProj.xy * float2(0.5, -0.5) + 0.5;
	shadowUV *= shadowDim;
	float2 fracUV = frac(shadowUV);
	int4 shadowIndex4;
	shadowIndex4.xy = int2(shadowUV);
	shadowIndex4.z = shadowIndex;
	shadowIndex4.w = 0;

	float4 depths;
	depths.x = g_shadow_depth.Load(shadowIndex4);
	depths.y = g_shadow_depth.Load(shadowIndex4 + int4(1,0,0,0));
	depths.z = g_shadow_depth.Load(shadowIndex4 + int4(0,1,0,0));
	depths.w = g_shadow_depth.Load(shadowIndex4 + int4(1,1,0,0));

	float4 diffDepth = depths - shadowPosProj.z;
	diffDepth /= receiverDepth;
	diffDepth = saturate(diffDepth + 1);

	float2 diff2 = diffDepth.yw - diffDepth.xz;
	diff2 = diff2 * fracUV.x + diffDepth.xz;
	diff2.y -= diff2.x;
	diff2.x = diff2.y * fracUV.y + diff2.x;
	diff2.y = abs(viewPos.z) - g_shadow_far_fade;
	diff2.y = saturate(diff2.y * g_shadow_far_fade_mul);

	float shadow = diff2.y * (1 - diff2.x) + diff2.x;
	return shadow;
}
#endif

float3 NoAOColor(uint2 pixel) {
	// line 358-361
	float3 albedo = g_albedoRGB[pixel];
	float3 lightAccum = g_light_accumulate[pixel];
	float3 specAccum = g_specular_accumulate[pixel];
	return albedo * lightAccum + specAccum;
}

float3 AddBlackBorder(float3 color, uint2 id, float EdgeSensitivity, float EdgeStrength, float EdgeWidth) {
	int radius = max(1, int(EdgeWidth * 2));

	float3 spmRGB = g_specularRGB[id.xy];
	float spmDot = dot(spmRGB, spmRGB);
	if (spmDot < 0.0001) return color;

	float3 originalColor = color;

	float minDepth = 1e10;
	float maxDepth = -1e10;
	
	
	for (int y = -radius; y <= radius; y++)
	{
		for (int x = -radius; x <= radius; x++)
		{
			if (x == 0 && y == 0) continue;
			
			float depth = g_ambient_occlusion[id.xy + int2(x, y)].g;
			minDepth = min(minDepth, depth);
			maxDepth = max(maxDepth, depth);
		}
	}
	
	float centerDepth = g_ambient_occlusion[id.xy].g;
	float depthRange = maxDepth - minDepth;
	
	bool nearMin = abs(centerDepth - minDepth) < depthRange * 0.2;
	bool nearMax = abs(centerDepth - maxDepth) < depthRange * 0.2;
	
	float edgeStrength = 0;
	if ((nearMin || nearMax) && depthRange > EdgeSensitivity)
	{
		edgeStrength = min(0.6f, depthRange / (EdgeSensitivity * 3));
		edgeStrength = edgeStrength * EdgeStrength;
	}
	
#ifdef _HasShadow
	float3 edgeColor = 0;
#else
	float3 edgeColor = 1.25;
	edgeStrength *= 0.1;
#endif
	// if (g_outline_color > 0.01) {
	// 	edgeColor = 1.5;
	// 	edgeStrength *= g_outline_color;
	// } else {
	// 	edgeColor = 0;
	// }
	
	return lerp(originalColor, edgeColor, edgeStrength);
}

//===========================================================
// PBR code
//===========================================================
float3 FresnelSchlick(float cosTheta, float3 F0) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(float3 N, float3 H, float roughness) {
	// float roughness2 = roughness; //  * roughness
	// float roughness4 = roughness2 * roughness2;
	// float NdotH = max(dot(N, H), 0.001);
	// float NdotH2 = NdotH * NdotH;
	
	// float denom = NdotH2 * (roughness2 - 1.0) + 1.0;
	// denom = 3.14159265 * denom * denom;
	// return roughness4 / max(denom, 0.0001);

	float a2 = roughness * roughness;
	float NdotH = max(dot(N, H), 0.001);
	float NdotH2 = NdotH * NdotH;
	
	float denom = NdotH2 * (a2 - 1.0) + 1.0;
	denom = 3.14159265 * denom * denom;
	return a2 / max(denom, 0.0001);
}

float GeometrySchlickGGX(float NdotV, float roughness) {
	// float r = roughness + 1.0;
	// float k = (r * r) / 8.0;
	// float denom = NdotV * (1.0 - k) + k;

	float roughness4 = pow(roughness, 4);
	float specBias = roughness4 * 0.5;
	float denom = NdotV * (1.0 - specBias) + specBias;

	return NdotV / max(denom, 0.0001);
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness) {
	float NdotV = max(dot(N, V), 0);
	float NdotL = max(dot(N, L), 0);
	
	float ggx1 = GeometrySchlickGGX(NdotV, roughness);
	float ggx2 = GeometrySchlickGGX(NdotL, roughness);
	
	return ggx1 * ggx2;
}

//===========================================================
// entry point
//===========================================================
[numthreads(16, 16, 1)]
void CS_main(uint3 threadID : SV_DispatchThreadID) {
	// line 0-5
	uint2 texDim;
	g_albedoRGB.GetDimensions(texDim.x, texDim.y);
	if (threadID.x >= texDim.x || threadID.y >= texDim.y) {
		return;
	}
	
	uint2 pixel = threadID.xy;
	
	// line 8, 16
	float4 viewPos = float4(g_viewXYZ_id[pixel].xyz, 1);
	
	// line 9-15
	if (abs(viewPos.z) >= 60000.0) {
		// old is 65534, but actual max is 65504, so now is 60k
		g_color_out[pixel] = float4(g_bg_color.xyz, 0.0);
		return;
	}
	
	// line 17-19
	float3 worldPos;
	worldPos.x = dot(viewPos, g_xgl_view_inverse[0]);
	worldPos.y = dot(viewPos, g_xgl_view_inverse[1]);
	worldPos.z = dot(viewPos, g_xgl_view_inverse[2]);

	float3 cameraPos = float3(g_xgl_view_inverse[0].w, g_xgl_view_inverse[1].w, g_xgl_view_inverse[2].w);
	float3 viewDir = worldPos - cameraPos;
	float distance = length(viewDir);

	float fade = distance * g_xgl_fade_mul + g_xgl_fade_add;
	if (fade >= 1.0) {
		g_color_out[pixel] = float4(g_bg_color.xyz, 0.0);
		return;
	}

	float3 normal = g_normalXYZ[pixel];
	float normalLen = length(normal);
	
	float fog = saturate(distance * g_xgl_fog_mul + g_xgl_fog_add);
	float fogWeight = fog * g_xgl_fog_color.w;

	// line 45-47, r7.w
	float fadeInv = 1.0 - saturate(fade);
	
	float3 color;
	
	// line 47-48
	if (normalLen > 0.001) {
		// line 49-53
		normal /= normalLen;
		viewDir /= distance;
		float3 reflectDir = reflect(viewDir, normal);
		

		// line 54-62
		float2 aoUV = float2(pixel) / float2(texDim);
		float ao = g_ambient_occlusion.SampleLevel(g_ambient_occlusion_sampler, aoUV, 0).x;
		float aoStrength = ao * normalLen;
		float3 ambientColor = aoStrength * g_xgl_ambient_color;
		
		float3 specularColor = g_specularRGB[pixel];
		float3 srSpecpowColor = g_smooth_reflection_specpow[pixel];
		float2 metallicRoughness = g_metallic_roughness[pixel];

		//metallicRoughness.x *= srSpecpowColor.y;
		//metallicRoughness.x = srSpecpowColor.y;

		//float metallic = saturate(metallicRoughness.x);
		float metallic = saturate(srSpecpowColor.y);
		float roughness = saturate(metallicRoughness.y);


		// line 66-68
		float roughnessClamped = max(roughness, 0.04);
		// Fresnel Reflectance
		float F0 = metallic * 0.96 + 0.04;
		float3 specular_color = F0 * specularColor;
		
#ifdef _HasShadow
		float shadow = CalculateShadow(viewPos);
#else
		float shadow = 1;
#endif
		
		float3 diffuseAccum = ambientColor;
		bool subLight = false;
		float3 specularAccum = 0;
		
		for (uint i = 0; i < g_light_count; i++) {
			if (shadow > 0.0) {
			#ifndef _NoPBR
				// float angle = g_xgl_time;
				// float3 tempDir = float3(sin(angle), 0, cos(angle));
				// float3 lightDir = normalize(tempDir);
				float3 lightDir = g_xgl_light[i].m_vector;

				float NdotLight = saturate(dot(-normal, lightDir));
				NdotLight = NdotLight * (1 - srSpecpowColor.x) + srSpecpowColor.x;
				NdotLight = max(NdotLight, 0.0001);
				//if (NdotLight > 0.001) {
				float3 halfDir = viewDir + lightDir;
				//float3 halfDir = viewDir - lightDir;
				float halfDirLen = length(halfDir);
				if (halfDirLen > 0.000001) {
					halfDir = halfDir / halfDirLen;
				}

				float VdotN = saturate(dot(-normal, viewDir));

				float denom = DistributionGGX(-normal, halfDir, roughnessClamped);
				float G = GeometrySmith(-normal, VdotN, NdotLight, roughnessClamped);
				float3 SF = FresnelSchlick( saturate(dot(halfDir, viewDir)), specular_color);

				float specBRDF = (G * denom) / max(VdotN * NdotLight * 4.0, 0.0001);
				float lightFactor = NdotLight * 3.14159265;
				float3 specular = SF * specBRDF * lightFactor;
				specular += specular * srSpecpowColor.z * 0.01;

				float reflectionMask = lerp(metallicRoughness.x, 1.0, metallic);
				specular *= reflectionMask;

				if (subLight) {
					specular = min(specular, 1);
				}
				
				diffuseAccum += g_xgl_light[i].m_color * NdotLight * shadow;
				specularAccum += g_xgl_light[i].m_specular_color * specular * shadow;
				
			#else
				float3 lightDir = g_xgl_light[i].m_vector;
				float NdotLight = dot(normal, lightDir);
				NdotLight = srSpecpowColor.x - NdotLight * (1 - srSpecpowColor.x);
				if (NdotLight > 0) {
					diffuseAccum += g_xgl_light[i].m_color * NdotLight * shadow;
					float RdotLight = dot(reflectDir, lightDir);
					RdotLight = max(-RdotLight, 0.0001) * shadow;
					RdotLight = pow(RdotLight, max(srSpecpowColor.z, 0.0001));
					specularAccum += g_xgl_light[i].m_specular_color * RdotLight;
				}
			#endif
			}
			shadow = 1;
			subLight = true;
		}
		
		float nonMetallic = 1.0 - metallic;
		float3 diffuse_accum = diffuseAccum * normalLen * nonMetallic;
		float3 specular_accum = specularAccum * normalLen;
		
		float4 worldPosH = float4(worldPos, 1.0);
		float lightDepth = dot(worldPosH, g_light_view_proj[2]) / dot(worldPosH, g_light_view_proj[3]);
		float3 f_lightUVW = float3(aoUV, lightDepth) * 128;
		uint3 lightUVW = uint3(f_lightUVW);
		
		float nonRoughness = 1.0 - metallicRoughness.y;
		float2 nonRoughness2 = float2(0.2, 200) * nonRoughness;
		float2 specRoughness = nonRoughness < 0 ? float2(0, 0.000001) : float2(nonRoughness2.y, nonRoughness);
		
		// line 189-193
		bool maskX = g_point_light_mask_x[lightUVW.x].m_is_anything_passed;
		bool maskY = g_point_light_mask_y[lightUVW.y].m_is_anything_passed;
		bool maskZ = g_point_light_mask_z[lightUVW.z].m_is_anything_passed;
		// line 194
		if (maskX && maskY && maskZ) {
			// line 195-198
			uint numGroups = (g_point_light_count + 31) / 32;
			float3 pointDiffuse = diffuse_accum; // r10.yzw
			float3 pointSpecular = 0; // r11.xyz
			// line 200
			for (uint group = 0; group < numGroups; group++) {
				uint lightOffset = group * 32; // r9.w
				// line 204-208
				uint g_maskX = g_point_light_mask_x[lightUVW.x].m_light_mask[group];
				uint g_maskY = g_point_light_mask_y[lightUVW.y].m_light_mask[group];
				uint g_maskZ = g_point_light_mask_z[lightUVW.z].m_light_mask[group];
				uint g_mask = g_maskX & g_maskY & g_maskZ;
				// line 209-213
				uint lightIndex = firstbitlow(g_mask);
				float3 pointDiffAccum = pointDiffuse; // r12.xyz
				float3 pointSpecAccum = pointSpecular; // r13.xyz
				uint tempMask = g_mask; // r9.y
				uint currentLight = lightIndex; // r11.w
				// line 214-216
				while (currentLight < 32) {
					// line 217-224
					tempMask &= ~(1u << currentLight);
					uint globalIndex = lightOffset + currentLight;
					PointLight light = g_xgl_compute_point_light_array[globalIndex];
					float3 toLight = worldPos - light.m_position;
					float dist = length(toLight);
					// line 225
					[branch]
					if (dist < light.m_radius) {
						//line 227-228
						float attenZ = light.m_smooth;
						// line 229
						toLight /= dist;
						// line 230-231
						float spotAtten = attenZ - (dot(toLight, normal) * (1 - attenZ));
						// line 232
						[branch]
						if (spotAtten > 0.0) {
							// line 234-239
							float atten = 1.0 - dist * light.m_inv_radius;
							float finalAtten = atten * saturate(spotAtten);
							pointDiffAccum += light.m_color * finalAtten;
							
							// line 243-251, Phong
							float RdotL = dot(toLight, reflectDir);
							float specBase = max(-RdotL, 0.0001);
							float specIntensity = pow(pow(atten, 0.1) * specBase, specRoughness.x);
							pointSpecAccum += specIntensity * light.m_color;
						}
						// end
					}
					// end
				
					// line 254
					currentLight = firstbitlow(tempMask);
				}
				
				// line 256-257
				pointDiffuse = pointDiffAccum;
				pointSpecular = pointSpecAccum;
			}
			// line 260-261
			diffuse_accum = pointDiffuse;
			specular_accum += pointSpecular * specRoughness.y;
		}
		
		// line 263-266
		float3 albedo = g_albedoRGB[pixel];
		float3 lightAccum = g_light_accumulate[pixel];
		float3 finalDiffuse = diffuse_accum + lightAccum;
		finalDiffuse *= albedo;
		
		// line 267-270
		uint c_cube3[3], cubemapSize;
		g_local_cubemap_array.GetDimensions(0, c_cube3[0], c_cube3[1], c_cube3[2], cubemapSize);
		
		float roughnessFactor = max(-nonRoughness * nonRoughness + 1, 0);
		float mipLevel = cubemapSize * roughnessFactor;
		
		// line 271-278
		float3 gridPos = worldPos - g_min_point;
		float3 gridCoord = gridPos / g_unit_size;
		uint3 gridIndex = uint3(gridCoord);
		uint3 maxGridIndex = g_cubemap_grid_divide_count - 1;
		gridIndex = min(gridIndex, maxGridIndex);

		uint g_tempY = gridIndex.z * g_cubemap_grid_divide_count.y;
		uint g_tempZ = gridIndex.y * g_cubemap_grid_divide_count.x;
		uint gridIdx = g_tempY * g_cubemap_grid_divide_count.x + g_tempZ;
		gridIdx += gridIndex.x;
		// line 279-283
		LocalEnvGrid envGrid = g_local_cubemap_grid[gridIdx];
		
		// line 284
		float3 envColor; // r6.yzw
		uint envCount = envGrid.m_count;
		if (envCount == 1) {
			// line 285-288
			uint cubemapIndex = envGrid.m_cubemap_index[0];
			envColor = g_local_cubemap_array.SampleLevel(
				g_xgl_compute_environment_map_sampler,
				float4(reflectDir, cubemapIndex),
				mipLevel).rgb;
		}
		else {
			// line 289-293
			float3 gridOffset = -float3(gridIndex) * g_unit_size + gridPos;
			float3 gridFrac = gridOffset / g_unit_size;
			float3 gridInvFrac = 1 - gridFrac;

			// line 294-302
			float gr4w = gridInvFrac.x * gridInvFrac.y;
			float gr9y = saturate(gr4w * gridInvFrac.z);
			float2 gr3xy = gridInvFrac.yx * gridFrac.xy;
			float2 gr9zw = saturate(gr3xy * gridInvFrac.z);
			float gr5x = gridFrac.x * gridFrac.y;
			float gr3z = saturate(gr5x * gridInvFrac.z);
			gr4w = saturate(gr4w * gridFrac.z);
			gr3xy = saturate(gr3xy * gridFrac.z);
			gr5x = saturate(gr5x * gridFrac.z);
			
			uint shift[8];
			for (uint getIDX = 0; getIDX < 4; getIDX++){
				shift[getIDX] = (envGrid.m_coner_index.x >> (getIDX * 8)) & 0xFF;
				shift[getIDX + 4] = (envGrid.m_coner_index.y >> (getIDX * 8)) & 0xFF;
			}
			
			float3 weightedColor = 0;
			float totalWeight = 0;
			float3 envColors[8];
			for (uint j = 0; j < envCount; j++) {
				uint cubeIndex = envGrid.m_cubemap_index[j];
				envColors[j] = g_local_cubemap_array.SampleLevel(
								g_xgl_compute_environment_map_sampler,
								float4(reflectDir, cubeIndex),
								mipLevel).rgb;
			}

			float weight[8];
			weight[0] = gr9y;
			weight[1] = gr9zw.x;
			weight[2] = gr9zw.y;
			weight[3] = gr3z;
			weight[4] = gr4w;
			weight[5] = gr3xy.x;
			weight[6] = gr3xy.y;
			weight[7] = gr5x;
			for (uint weightIdx = 0; weightIdx < 8; weightIdx++) {
				weightedColor += envColors[shift[weightIdx]] * weight[weightIdx];
				totalWeight += weight[weightIdx];
			}
			
			envColor = weightedColor / totalWeight;
		}
		
		// line 348-349
		//float metallic2 = metallicRoughness.x * metallicRoughness.x;
		float metallic2 = srSpecpowColor.y * srSpecpowColor.y;
		float specMetal = metallic2 * -nonRoughness2.x + nonRoughness2.x;
		
		// line 350-356
		float3 finalColor = albedo * aoStrength * envColor - finalDiffuse;
		finalColor = finalColor * metallic2 + finalDiffuse;
		//finalColor += envColor * specMetal;

		//specular_accum *= srSpecpowColor.z * specMetal + envColor * aoStrength;
		//specular_accum *= srSpecpowColor.z * specMetal * envColor * aoStrength * 2.5;
		
#ifdef _HasShadow
		specular_accum *= specMetal * envColor * specRoughness.x;
		specular_accum = min(specular_accum, 2);
#else
		specular_accum = min(specular_accum, 1);
#endif
		float3 specAccum = g_specular_accumulate[pixel] + specular_accum;
		//specAccum *= 1.25;
		color = finalColor + specAccum;

		color = AddBlackBorder(color, pixel, 0.75, 2, 0.5);
	} else {
		// line 358-361
		color = NoAOColor(pixel);
	}

	// line 363-367
	color = lerp(color, g_xgl_fog_color.xyz, fogWeight);
	g_color_out[pixel] = float4(color, fadeInv);
}
