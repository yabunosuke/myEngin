#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
    float Y =
        texcolor.r * 0.29891f +
        texcolor.g * 0.58661f +
        texcolor.b * 0.11448f;

    return float4(Y,Y,Y,1);
}