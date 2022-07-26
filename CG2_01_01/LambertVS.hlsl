
#include "Lambert.hlsli"

VSOutput main(VSinput input)
{
	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	// ボーンの影響は四つまで
	for (int i = 0; i < 4; i++)
	{
		p += (input.boneWeights[i] * mul(input.position, boneTransforms[input.boneIndices[i]])).xyz;
		n += (input.boneWeights[i] * mul(float4(input.normal.xyz, 0), boneTransforms[input.boneIndices[i]])).xyz;
	}

	VSOutput vout;
	// ボーンの影響を考慮したポジションを返す
	vout.position = mul(float4(p, 1.0f), viewProjection);

	// ボーンの影響を考慮したノーマルを返す
	vout.normal = normalize(n);

	vout.color.rgb = input.color.rgb * materialColor.rgb;
	vout.color.a = input.color.a * materialColor.a;
	vout.texcoord = input.texcoord.xy;


	return vout;
}
