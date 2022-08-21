#include "DeferredShader.hlsli"


Texture2D color_texture_2d				: register(t0);
Texture2D normal_texture_2d				: register(t1);
Texture2D metal_roughness_texture_2d	: register(t2);
Texture2D depth_texture_2d				: register(t3);
Texture2D position_texture_2d			: register(t4);
Texture2D id_texture_2d					: register(t5);

SamplerState texture_sampler : register(s0);


///////////////////////////////////////////
// �֐��錾
///////////////////////////////////////////
float3 CalcLambertDiffuse(float3 light_direction, float3 light_color, float3 normal);
float3 CalcPhongSpecular(float3 light_direction, float3 light_color, float3 world_pos, float3 normal, float3 eye_position);

float4 main(VSOutput input) : SV_TARGET
{
	float4 output_color;

	float4 color_tex =				color_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 normal_tex =				normal_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 metal_roughness_tex =	metal_roughness_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 depth_tex =				depth_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 position_tex =			position_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 id_tex =					id_texture_2d.Sample(texture_sampler, input.texcoord);

	/// ���v�Z
	if (id_tex.r != 0) {

		// Light�̌v�Z

		float3 diffuse, specular;
		for(int i =0;i<LIGHT_MAX;++i)
		{
			if(light[i].is_active == 0.0f)
			{
				break;
			}


			// PointLight
			if (light[i].light_type.z != 0)
			{
				// ���̓��ˊp�����߂�
				float3 light_dir = position_tex.xyz - light[i].position.xyz;
				light_dir = normalize(light_dir);

				// �����Ȃ���Lambert�g�U�����v�Z
				float3 diffuse_point = CalcLambertDiffuse(
					light_dir,
					light[i].color.xyz,
					normalize(normal_tex.xyz)
				);

				// �����Ȃ���Phon���ʔ��ˌ����v�Z
				float3 specular_point = CalcPhongSpecular(
					light_dir,
					light[i].color.xyz,
					position_tex.xyz,
					normal_tex.xyz,
					light[i].position.xyz
				);

				// �|�C���g���C�g�Ƃ̋������v�Z����
				float distance = length(position_tex. xyz - light[i].position.xyz);
				// �e�����̌v�Z
				float affect = 1.0f - distance / light[i].range;
				if (affect < 0.0f)
				{
					affect = 0.0f;
				}
				// �w���֐���
				affect = pow(affect, 3);

				diffuse_point *= affect;
				specular_point *= affect;

				// ���Z
				diffuse += diffuse_point * light[i].intensity;
				specular += specular_point * light[i].intensity;
			}
		}

		




		// ���̃��C�g
		float3 dir_light = normalize(-float3(1, -1, 0));

		// �O��
		float d = dot(dir_light, normalize(normal_tex.rgb));
		float power = max(0, d) * 0.8f + 0.2f;

		float3 dir_diffuse = float3(0.8, 0.8, 0.8) * float3(1, 1, 1) * saturate(power);





		float3 light = dir_diffuse+diffuse + specular + float3(0.02f, 0.02f, 0.02f);

		output_color = float4(color_tex.rgb * light, color_tex.w);
		

		return output_color;
	}

	return float4(1, 1, 0, 1);
}


/// <summary>
/// Lambert�g�U���ˌ��̌v�Z
/// </summary>
float3 CalcLambertDiffuse(float3 light_direction, float3 light_color, float3 normal)
{
	// �s�N�Z���̖@���ƃ��C�g�̕����̓��ς��v�Z����
	float t = saturate(dot(normal, light_direction) * -1.0f);

	// ���ς̒l��0�ȏ�̒l�ɂ���
	t = max(0.0f, t);

	// �g�U���ˌ����v�Z����
	return light_color * t;
	
}

/// <summary>
/// Phong�g�U���ˌ��̌v�Z
/// </summary>
float3 CalcPhongSpecular(float3 light_direction, float3 light_color, float3 world_pos, float3 normal,float3 eye_position)
{
	// ���˃x�N�g�������߂�
	float3 reflection_vec = reflect(light_direction, normal);

	// �������������T�[�t�F�C�X���王�_�ɐL�т�x�N�g�������߂�
	float3 to_eye = eye_position - world_pos;
	to_eye = normalize(to_eye);

	// ���ʔ��˂̋��������߂�
	float t = dot(reflection_vec, to_eye);

	// ���ʔ��˂̋�����0�ȏ�̐��l�ɂ���
	t = max(0.0f, t);

	// ���ʔ��˂̋������i��
	t = pow(t, 5.0f);

	// ���ʔ��ˌ������߂�
	return light_color * t;
}
