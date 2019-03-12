#include "000_Header_N.fx"

uint VisibleBump = 0;

struct VertexOuput
{
    float4 Position : SV_Position;
    float3 wPosition : Position1;
    float3 Normal : Normal0;
    float3 Tangent : Tangent0;
    float2 Uv : Uv0;
};

VertexOuput VS(VertexTextureNormalTangent input)
{
    VertexOuput output;

    output.Position = WorldPosition(input.Position);
    output.wPosition = output.Position;

    output.Position = ViewProjection(output.Position);
    output.Normal = WorldNormal(input.Normal);
    output.Tangent = WorldTangent(input.Tangent);
    output.Uv = input.Uv;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

float3 SpecularMapping(float3 mapIntensity, float3 normal, float3 viewDirection)
{
    float3 reflection = reflect(LightDirection, normal);
    float intensity = saturate(dot(reflection, viewDirection));
    float specular = pow(intensity, Shininess);

    return Specular.rgb * mapIntensity * specular;
}

SamplerState Sampler;
float4 PS(VertexOuput input) : SV_TARGET
{
    //float4 diffuseMap = DiffuseMap.Sample(Sampler, input.Uv);
    float4 diffuseMap = float4(1, 0, 0, 1);

    float intensity = 1;
    float3 normal = normalize(input.Normal);
    if (VisibleBump == 1 || VisibleBump == 2)
    {
        float4 normalMap = NormalMap.Sample(Sampler, input.Uv);
        float3 bump = TangentSpace(normalMap.rgb, normal, input.Tangent);
        intensity = saturate(dot(bump, -LightDirection));
    }

    float3 specular = 0;
    float4 specularMap = 0;
    if(VisibleBump == 2)
    {
        specularMap = SpecularMap.Sample(Sampler, input.Uv);
        float3 view = ViewPosition(input.wPosition);
        specular = SpecularMapping(specularMap.rgb, normal, view);
    }    
    
    return (diffuseMap * intensity) + float4(specular, 1);
    //return float4(specular, 1);
}

///////////////////////////////////////////////////////////////////////////////

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}