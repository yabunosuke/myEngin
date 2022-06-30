cbuffer cbuff0 : register(b0) {
	matrix mat;		//3d�ϊ��s��
}

cbuffer cbuff1 : register(b1) {
	float3 m_ambient : packoffset(c0);	//�A���r�G�C�g�W��
	float3 m_diffuse : packoffset(c1);	//�f�B�t���[�Y�W��
	float3 m_specular : packoffset(c2);	//�X�y�L�����[�W��
	float m_alpha : packoffset(c2.w);	//�A���t�@
}

struct VSOutput {
	float4 svpos : SV_POSITION;	//�V�X�e���p���_���W
	float3 normal : NORMAL;	//�@���x�N�g��
	float2 uv : TEXCOORD;	//uv�l
};