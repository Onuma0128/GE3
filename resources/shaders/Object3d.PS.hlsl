#include "object3d.hlsli"

struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
};
struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};
struct PointLIght
{
    float4 color;
    float3 position;
    float intensity;
    float radius;
    float decay;
};
struct Camera
{
    float3 worldPosition;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<PointLIght> gPointLight : register(b3);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{    
    PixelShaderOutput output;
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    float3 normal = normalize(input.normal);
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    // DirectionalLight
    float3 lightDirectionalLight = normalize(gDirectionalLight.direction);
    float3 reflectLight = reflect(-lightDirectionalLight, normal);
    float RdotE = max(dot(reflectLight, toEye), 0.0);
    float specularPow = pow(RdotE, gMaterial.shininess);
    // PointLight
    float3 pointLightDirecion = normalize(gPointLight.position - input.worldPosition);
    reflectLight = reflect(-pointLightDirecion, normal);
    RdotE = max(dot(reflectLight, toEye), 0.0);
    float pointPow = pow(RdotE, gMaterial.shininess);
    
    if (gMaterial.enableLighting != 0)
    {
        // DirectionalLightの処理
        float NdotL = (dot(normal, lightDirectionalLight));
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float3 diffuseDirectionalLIght = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        float3 specularDirectionalLIght = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow;
        // PointLightの処理
        NdotL = (dot(normal, pointLightDirecion));
        cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float distance = length(gPointLight.position - input.worldPosition);
        float factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);
        float3 diffusePointLight = gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cos * gPointLight.intensity * factor;
        float3 specularPointLight = gPointLight.color.rgb * gPointLight.intensity * pointPow * factor;
        // ライトの処理を合算
        output.color.rgb = 
        diffuseDirectionalLIght + specularDirectionalLIght +
        diffusePointLight + specularPointLight;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }

    if (textureColor.a == 0.0f || textureColor.a <= 0.5f || output.color.a == 0.0f)
    {
        discard;
    }
    
    return output;
}