#include "Primitive.hlsli"

struct TransformationMatrix
{
    float4x4 WVP;
};

ConstantBuffer<TransformationMatrix> gWorldViewProjection : register(b0);

struct VS_Input
{
    float4 position : POSITION0;
};

VS_Output main(VS_Input input)
{
    VS_Output output;

    output.position = mul(input.position,gWorldViewProjection.WVP);

    return output;
}