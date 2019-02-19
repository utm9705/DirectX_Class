#include "000_Header.fx"

struct VertexOuput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
};

VertexOuput VS(VertexTextureNormalTangent input)
{
    VertexOuput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Normal = mul(input.Normal, (float3x3) World);
    output.Uv = input.Uv;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler
{
    Filter = MIN_MAG_MIP_LINEAR;
};

float4 PS(VertexOuput input) : SV_TARGET
{
    float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);

    return diffuse;
}


SamplerState Sampler2
{
    Filter = MIN_MAG_MIP_POINT;
};

float4 PS2(VertexOuput input) : SV_TARGET
{
    float4 diffuse = DiffuseMap.Sample(Sampler2, input.Uv);

    return diffuse;
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
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS2()));
    }
}