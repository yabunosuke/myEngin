#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 sepiaTone = (1.0f,0.8f,0.0f,1.0f);
    float4 texcolor = tex.Sample(smp, input.uv);
	float tempPixel = (texcolor.r + texcolor.g + texcolor.b) * 0.33333f;
    float4 color = tempPixel*sepiaTone;

    return color;
}