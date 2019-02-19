#include "000_Header.fx"

cbuffer VS_Bones
{
    matrix Bones[128];   
}

cbuffer VS_BoneIndex
{
    int BoneIndex;
}

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
};

VertexOutput VS(VertexTextureNormal input)
{
    VertexOutput output;

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

float4 PS(VertexOutput input) : SV_TARGET
{
    float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);

    return diffuse;
}

//-----------------------------------------------------------------------------
// Techniques
//-----------------------------------------------------------------------------
technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}