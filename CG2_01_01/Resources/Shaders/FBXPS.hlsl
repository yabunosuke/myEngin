#include "FBX.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//�e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv);
	//float4 shadecolor = { 0,0,0,0 };

	//// ���s����
	//for (int i = 0; i < DIRLIGHT_NUM; i++)
	//{
	//	if (!dirLights[i].active)
	//	{
	//		continue;
	//	}
	//Lambert����
    float3 light = normalize( /*dirLights[i].lightv*/float3(1, -1, 1)); //���C�g�̌���
    float diffuse = saturate(dot(-light, input.normal));
    float brightness = diffuse + 0.3f;

	// �S�ĉ��Z����
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	//}
	
	
	//�A�e�ƃe�N�X�`���̐F������
	return shadecolor * texcolor;
}