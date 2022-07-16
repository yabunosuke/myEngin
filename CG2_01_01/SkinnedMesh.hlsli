//バーテックスバッファーの入力
struct VSInput
{
    float4 position     : POSITION;     // 位置
    float4 normal       : NORMAL;       // 頂点法線
    float2 texcoord     : TEXCOORD;     // テクスチャー座標
    float4 bone_weights : WEIGHTS;      // ボーン影響度
    uint4 bone_indices  : BONES;        // ボーンインデックス
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 position     : SV_POSITION;  // システム用頂点座標
    float4 worldPosition: POSITION;     // ワールド座標
    float4 worldNormal  : NORMAL;       // 法線
    float2 texcoord     : TEXCOORD;     // uv値
    float4 color        : COLOR;        // 色
};

cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
    float4 materialColor;
};

cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 viewProjection;
    float4 lightDirection;
    float4 cameraPosition;
    
}
