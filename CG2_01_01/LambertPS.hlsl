
#include "Lambert.hlsli"

// ƒTƒ“ƒvƒŠƒ“ƒO
#define POINT 0
#define LINER 1
#define ANISOTROPIC 2


Texture2D base : register(t0);
//Texture2D normal : register(t1);
//Texture2D ms : register(t2);

SamplerState sampler_states[3] : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	return base.Sample(sampler_states[ANISOTROPIC], pin.texcoord) * pin.color;
}

