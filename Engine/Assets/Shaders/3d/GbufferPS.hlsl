#include "GBuffer.hlsli"

Texture2D diffuse_t : register(t0);
Texture2D normal_t : register(t1);
Texture2D metal_roughness_t : register(t2);

SamplerState texture_sampler : register(s0);

PSOutput main(VSOutput input)
{
	PSOutput pout;

	// �g�U���ˌ��̐F
	float4 albedo = diffuse_t.Sample(texture_sampler, input.texcoord);
	// ���^���Ȃ߂炩��
	float2 ms = metal_roughness_t.Sample(texture_sampler, input.texcoord).ra;
	// �@��
	float3 normal = normal_t.Sample(texture_sampler, input.texcoord).rgb;

	// �e�N�X�`���Ȃ��̉摜��������}�e���A���J���[�ɕύX
	if (0 == albedo.r + albedo.g + albedo.b + albedo.a)
	{
		albedo = input.color;
	}
	albedo.a = 1.0f;

	// �@���}�b�s���O�ƃe�N�X�`���`�F�b�N
	if (0 != normal.r + normal.g + normal.b)
	{
		// ��]�s��
		float3x3 world_normal_matrix = {
			normalize(input.tangent),
			normalize(input.binormal),
			normalize(input.normal)
		};
		// ���[�J����Ԃɖ@�������[���h�֕ϊ�
		normal = normalize(mul(normal * 2.0f - 1.0f, world_normal_matrix));
	}
	else
	{
		normal = input.normal;
	}

	// �[�x
	float depth = input.viewprojection_position.z / input.viewprojection_position.w;

	albedo = pow(albedo, 2.2f);
	pout.color = albedo;
	pout.normal = float4(normal, 1.0f);
	pout.metal_roughness = float4(ms.x, 1.0f - ms.y, 1, 1);
	pout.depth = float4(depth, depth, 1.0f, 1.0f);	// �����F�ɂȂ�
	pout.position = input.world_position.xyzw;
	pout.id = float4(1, 0, 0, 1);	// ����ID���ߑł�

	return pout;
}