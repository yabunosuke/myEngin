
#include "Lambert.hlsli"

// サンプリング
#define POINT 0
#define LINER 1
#define ANISOTROPIC 2

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

Texture2D base : register(t0);
//Texture2D normal : register(t1);
//Texture2D ms : register(t2);

SamplerState sampler_states[3] : register(s0);

PSOutput main(VSOutput pin)
{
	PSOutput output;

	float4 color = base.Sample(sampler_states[ANISOTROPIC], pin.texcoord) * pin.color;
	output.target0 = color;
	//output.target1 = float4(1 - color.rgb, 1);	//反転
	output.target1 = float4(1,0,0,1);	//単色

	return output;
}

