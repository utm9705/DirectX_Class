#include "000_Header.fx"

cbuffer VS_Bones
{
    matrix Bones[128];
}

cbuffer VS_BoneIndex
{
    int BoneIndex;
}

struct VertexOuput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
};

VertexOuput VS(VertexTextureNormal input)
{
    VertexOuput output;

    World = Bones[BoneIndex];

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Normal = mul(input.Normal, (float3x3) World);

    output.Uv = input.Uv;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler;

float4 PS(VertexOuput input) : SV_TARGET
{
    float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);

    float3 normal = normalize(input.Normal);
    float NdotL = dot(-LightDirection, normal);

    return diffuse * NdotL;
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