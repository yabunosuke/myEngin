
#include "Lambert.hlsli"

VSOutput main(VSinput input)
{
	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	// �{�[���̉e���͎l�܂�
	for (int i = 0; i < 4; i++)
	{
		p += (input.boneWeights[i] * mul(input.position, boneTransforms[input.boneIndices[i]])).xyz;
		n += (input.boneWeights[i] * mul(float4(input.normal.xyz, 0), boneTransforms[input.boneIndices[i]])).xyz;
	}

	VSOutput vout;
	// �{�[���̉e�����l�������|�W�V������Ԃ�
	vout.position = mul(float4(p, 1.0f), viewProjection);

	// �{�[���̉e�����l�������m�[�}����Ԃ�
	vout.normal = normalize(n);
	float3 N = normalize(n);
	float3 L = normalize(-lightDirection.xyz);
	float d = dot(L, N);
	float power = max(0, d) * 0.5f + 0.5f;
	vout.color.rgb = input.color.rgb * materialColor.rgb * power;
	vout.color.a = input.color.a * materialColor.a;
	vout.texcoord = input.texcoord.xy;


	return vout;
}
