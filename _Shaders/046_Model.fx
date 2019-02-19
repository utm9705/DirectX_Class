#include "000_Header.fx"

struct VertexOuput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
};

VertexOuput VS(VertexTextureNormalBlend input)
{
    VertexOuput output;

    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Normal = WorldNormal(input.Normal);
    output.Uv = input.Uv;

    return output;
}


///////////////////////////////////////////////////////////////////////////////

SamplerState Sampler;

float4 PS(VertexOuput input) : SV_TARGET
{
    float4 diffuse = DiffuseMap.Sample(Sampler, input.Uv);

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
}