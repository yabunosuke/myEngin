#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    //float4 texcolor = tex.Sample(smp, input.uv);
    
    float x1pix = 1.0f / 1280.0f;
    float y1pix = 1.0f / 720.0f;
    float4 color = { 0, 0, 0, 0 };
    
    //縦を先に計算(7マス)
    for (int i = -3; i <= 3; i++)
    {
        color = color + tex.Sample(smp, input.uv + float2(0, y1pix * i));
    }
    //横に7マス
    for (int j = -3; j <= 3; j++)
    {
        color = color + tex.Sample(smp, input.uv + float2(x1pix * j, 0));
    }
    
    return float4(color.rgb / 14.0f , 1);
}