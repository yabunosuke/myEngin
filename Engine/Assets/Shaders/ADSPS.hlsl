#include "PostEffectTest.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv) * color;
    float3 diffuse = { texcolor.rgb };
    float3 ambient = diffuse - 0.3f;
    
    return float4(constColor.rgb, texcolor.a);
    //return texcolor;

}