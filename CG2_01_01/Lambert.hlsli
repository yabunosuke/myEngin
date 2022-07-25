struct VSinput
{
	float4 position     : POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 texcoord		: TEXCOORD;
	float4 color		: COLOR;
	float4 boneWeights : WEIGHTS;
	uint4  boneIndices : BONES;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal	: NROMAL;
	float4 color    : COLOR;

};

// マルチレンダリング
struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
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