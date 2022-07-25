
#include "Lambert.hlsli"

// サンプリング
#define POINT 0
#define LINER 1
#define ANISOTROPIC 2



Texture2D base : register(t0);
Texture2D normal : register(t1);
//Texture2D ms : register(t2);

SamplerState sampler_states[3] : register(s0);

PSOutput main(VSOutput input)
{
	PSOutput output;	// 出力データ


	float4 base_textuer = base.Sample(sampler_states[ANISOTROPIC], input.texcoord) /** input.color*/;
	float4 normal_textuer = normal.Sample(sampler_states[ANISOTROPIC], input.texcoord);

	output.target0 = base_textuer;
	//output.target1 = float4(1 - color.rgb, 1);	//反転
	output.target1 = normal_textuer;	//単色

	return output;
}

