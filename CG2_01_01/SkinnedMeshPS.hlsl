#include "SkinnedMesh.hlsli"

#define POINT 0
#define LINER 1
#define ANISOTROPIC 2

SamplerState sampler_states[3] : register(s0);
Texture2D texture_maps[4] : register(t0);

float4 main(VSOutput pin) : SV_Target
{
    float4 color = texture_maps[0].Sample(sampler_states[ANISOTROPIC], pin.texcoord);
    
    float3 N = normalize(pin.worldNormal.xyz);
    float3 L = normalize(-lightDirection.xyz);
    float3 diffuse = color.rgb * max(0, dot(N, L));
    
    return float4(diffuse, color.a/*1*/) * pin.color;
}