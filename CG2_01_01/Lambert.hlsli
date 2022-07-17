struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color    : COLOR;
};

cbuffer SCENE_CONSTANT_BUFFER : register(b0)
{
	row_major float4x4	viewProjection;
	float4				lightDirection;
};

#define MAX_BONES 256
cbuffer MESH_CONSTANT_BUFFER : register(b1)
{
	row_major float4x4	boneTransforms[MAX_BONES];
};

cbuffer SUBSET_CONSTANT_BUFFER : register(b2)
{
	float4				materialColor;
};