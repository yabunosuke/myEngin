#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float4 sepiaTone = (1.0f,0.8f,0.0f,1.0f);
    float4 texcolor = tex.Sample(smp, input.uv);
	float tempPixel = (texcolor.r + texcolor.g + texcolor.b) * 0.33333f;
    float4 color = tempPixel*sepiaTone;

    return color;
}