#include "DeferredShader.hlsli"


Texture2D color_texture_2d				: register(t0);
Texture2D normal_texture_2d				: register(t1);
Texture2D metal_roughness_texture_2d	: register(t2);
Texture2D depth_texture_2d				: register(t3);
Texture2D position_texture_2d			: register(t4);
Texture2D id_texture_2d					: register(t5);

SamplerState texture_sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 output_color;

	float4 color_tex =				color_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 normal_tex =				normal_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 metal_roughness_tex =	metal_roughness_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 depth_tex =				depth_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 position_tex =			position_texture_2d.Sample(texture_sampler, input.texcoord);
	float4 id_tex =					id_texture_2d.Sample(texture_sampler, input.texcoord);

	/// 仮計算
	if (id_tex.r != 0) {
		//// 仮のライト
		//float3 dir_light = normalize(-float3(1, -1, 0));
		//// 外積
		//float d = dot(dir_light, normalize(normal_tex.rgb));
		//float power = max(0, d) * 0.8f + 0.2f;

		//float3 diffuse = float3(0.8, 0.8, 0.8) * float3(1, 1, 1) * saturate(power);

		//output_color = float4(color_tex.rgb * diffuse, 1);

		return /*pow(output_color, 1.0f / 2.2f)*/color_tex;
	}

	return float4(0, 1, 0, 1);
}