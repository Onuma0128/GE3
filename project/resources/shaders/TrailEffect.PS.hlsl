#include "TrailEffect.hlsli"

struct Material
{
    float4 color;
    int xTexcoord_alpha;
    int yTexcoord_alpha;
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_Target0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    if (gMaterial.xTexcoord_alpha != 0)
    {
        textureColor.a = input.texcoord.x;
    }
    if (gMaterial.yTexcoord_alpha != 0)
    {
        textureColor.a = input.texcoord.y;
    }
    textureColor.a = clamp(textureColor.a, 0.0f, 1.0f);
    
    output.color = gMaterial.color * textureColor;
    if (output.color.a <= 0.025f)
    {
        discard;
    }
    return output;
}