#include "SkinnedMesh.hlsli"

VSOutput main(VSInput vin)
{
    VSOutput vout;
    vout.position = mul(vin.position, mul(world, viewProjection));
    vout.worldPosition = mul(vin.position, world);
    vin.normal.w = 0;
    vout.worldNormal = normalize(mul(vin.normal, world));
    vout.texcoord = vin.texcoord;
    
#if 0
    vout.color = materialColor;
#else
    vout.color = 0;
    const float4 bone_colors[4] =
    {
        { 1, 0, 0, 1 },
        { 0, 1, 0, 1 },
        { 0, 0, 1, 1 },
        { 1, 1, 1, 1 }
    };
    for (int bone_index = 0; bone_index < 4; ++bone_index)
    {
        vout.color += bone_colors[vin.bone_indices[bone_index] % 4] * vin.bone_weights[bone_index];
    }
#endif
    
    return vout;
}