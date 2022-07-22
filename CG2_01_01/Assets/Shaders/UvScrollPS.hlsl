#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー


float4 main(VSOutput input) : SV_TARGET
{
	float2 scroll = { time, 0 };
	float4 texcolor = tex.Sample(smp, input.uv + scroll);
	return float4(texcolor.rgb, 1);
}