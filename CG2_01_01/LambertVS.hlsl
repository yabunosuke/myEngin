
#include "Lambert.hlsli"

VSOutput main(
	float4 position     : POSITION,
	float3 normal		: NORMAL,
	float3 tangent		: TANGENT,
	float3 texcoord		: TEXCOORD,
	float4 color		: COLOR,
	float4 boneWeights	: WEIGHTS,
	uint4  boneIndices	: BONES
)
{
	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };
	// ボーンの影響は四つまで
	for (int i = 0; i < 4; i++)
	{
		p += (boneWeights[i] * mul(position, boneTransforms[boneIndices[i]])).xyz;
		n += (boneWeights[i] * mul(float4(normal.xyz, 0), boneTransforms[boneIndices[i]])).xyz;
	}

	VSOutput vout;
	vout.position = mul(float4(p, 1.0f), viewProjection);

	float3 N = normalize(n);
	float3 L = normalize(-lightDirection.xyz);
	float d = dot(L, N);
	float power = max(0, d) * 0.5f + 0.5f;
	vout.color.rgb = color.rgb * materialColor.rgb * power;
	vout.color.a = color.a * materialColor.a;
	vout.texcoord = texcoord.xy;

	return vout;
}
