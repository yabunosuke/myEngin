#include "SkinnedMesh.hlsli"

float4 main(VSOutput pin) : SV_Target
{
    float3 N = normalize(pin.worldNormal.xyz);
    float3 L = normalize(-lightDirection.xyz);
    float3 diffuse = max(0, dot(N, L));
    
    return float4(diffuse, 1) * pin.color;
}