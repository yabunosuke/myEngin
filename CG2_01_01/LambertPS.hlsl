
#include "Lambert.hlsli"

// �T���v�����O
#define POINT 0
#define LINER 1
#define ANISOTROPIC 2



Texture2D base : register(t0);
Texture2D normal : register(t1);
//Texture2D ms : register(t2);

SamplerState sampler_states[3] : register(s0);

PSOutput main(VSOutput input)
{
	PSOutput output;	// �o�̓f�[�^


	float4 base_textuer = base.Sample(sampler_states[ANISOTROPIC], input.texcoord) /** input.color*/;
	float4 normal_textuer = normal.Sample(sampler_states[ANISOTROPIC], input.texcoord);



	// �@���x�N�g�����v�Z
	float3 N = normalize(( normal_textuer.xyz * 2.0f - 1.0f) * input.normal);
	
	// ���C�g�v�Z
	float3 L = normalize(-lightDirection.xyz);
	// �O��
	float d = dot(L, N);

	float power = max(0, d) * 0.5f + 0.5f;

	float3 diffuse = float3(0.8,0.8,0.8) * float3(1,1,1) * saturate(power);


	output.target0 = float4(base_textuer.rgb * diffuse, 1);

	// �@���x�N�g�����v�Z
	N = input.normal;
	// ���C�g�v�Z
	L = normalize(-lightDirection.xyz);
	// �O��
	d = dot(L, N);

	power = max(0, d) * 0.5f + 0.5f;
	diffuse = float3(0.8, 0.8, 0.8) * float3(1, 1, 1) * saturate(power);

	//output.target1 = float4(1 - color.rgb, 1);	//���]
	output.target1 = float4(base_textuer.rgb * diffuse, 1);
	//output.target1 = normal_textuer;
	return output;
}

