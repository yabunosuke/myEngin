struct VSInput{
    float4 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_POSITION;
};

cbuffer OBJECT_CONSTANT_BUFFER : register(b0) {
    row_major float4x4 world_;
    float4 color_;
}