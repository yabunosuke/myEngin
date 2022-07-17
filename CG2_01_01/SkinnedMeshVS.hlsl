#include "SkinnedMesh.hlsli"

VSOutput main(VSInput vin)
{
    // �{�[���̉e����^����
    vin.normal.w = 0;
    float4 blended_position = { 0, 0, 0, 1 };
    float4 blended_normal = { 0, 0, 0, 0 };
    for (int bone_index = 0; bone_index < 4; ++bone_index)
    {
        blended_position += 
        vin.bone_weights[bone_index] * mul(vin.position, bone_transforms[vin.bone_indices[bone_index]]);

        blended_normal += 
        vin.bone_weights[bone_index] * mul(vin.normal, bone_transforms[vin.bone_indices[bone_index]]);
    }
    vin.position = float4(blended_position.xyz, 1.0f);
    vin.normal = float4(blended_normal.xyz, 0.0f);
    
    
    VSOutput vout;
    vout.position = mul(vin.position, mul(world, viewProjection));
    vout.worldPosition = mul(vin.position, world);
    vin.normal.w = 0;
    vout.worldNormal = normalize(mul(vin.normal, world));
    vout.texcoord = vin.texcoord;
    
    vout.color = materialColor;
    
    //�e���x�`�F�b�N�p
    //vout.color = 0;
    //const float4 bone_colors[4] =
    //{
    //    { 1, 0, 0, 1 },
    //    { 0, 1, 0, 1 },
    //    { 0, 0, 1, 1 },
    //    { 1, 1, 1, 1 },
    //};
    //for (int boneindex = 0; boneindex < 4; ++boneindex)
    //{
    //    vout.color += bone_colors[vin.bone_indices[boneindex] % 4] * vin.bone_weights[boneindex];
    //}
    //vout.color.a = 1.0f;
    
    return vout;
}