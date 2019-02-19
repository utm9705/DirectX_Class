#include "000_Header.fx"

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
};

VertexOutput VS(VertexTextureNormalTangent input)
{
    VertexOutput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Normal = mul(input.Normal, (float3x3) World);
    output.Uv = input.Uv;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

cbuffer VS_Bones
{
    matrix Bones[128];
}

cbuffer VS_BoneIndex
{
    int BoneIndex;
}

VertexOutput VS_Model(VertexTextureNormal input)
{
    VertexOutput output;

    matrix world = Bones[BoneIndex];
    output.Position = mul(input.Position, world);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Normal = mul(input.Normal, (float3x3) world);
    output.Uv = input.Uv;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler
{
    AddressU = Wrap;
    AddressV = Wrap;
};

float4 PS(VertexOutput input) : SV_TARGET
{
    float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);
    float intensity = dot(-LightDirection, normalize(input.Normal));

    return diffuse * intensity;
}

///////////////////////////////////////////////////////////////////////////////

technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }

    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Model()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}