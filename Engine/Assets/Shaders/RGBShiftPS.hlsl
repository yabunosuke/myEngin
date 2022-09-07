#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float x1pix = 1.0f / 1280.0f;
    float y1pix = 1.0f / 720.0f;
    
    float4 texcolor = tex.Sample(smp, input.uv);
    float2 smppoint = input.uv;
    float shift = 10.0f;
    
    smppoint.x = input.uv.x + x1pix * shift;
    texcolor.r = tex.Sample(smp, smppoint).r;
    smppoint.y = input.uv.y + y1pix * shift;
    texcolor.g = tex.Sample(smp, smppoint).g;
    smppoint.x = input.uv.x + x1pix * -shift;
    texcolor.b = tex.Sample(smp, smppoint).b;
    
    return float4(texcolor.rgb, 1);
}